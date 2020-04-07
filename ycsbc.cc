//
//  ycsbc.cc
//  YCSB-C
//
//  Created by Jinglei Ren on 12/19/14.
//  Copyright (c) 2014 Jinglei Ren <jinglei@ren.systems>.
//

#include <cstring>
#include <string>
#include <iostream>
#include <vector>
#include <future>
#include "core/utils.h"
#include "core/timer.h"
#include "core/client.h"
#include "core/core_workload.h"
#include "db/db_factory.h"
#include<boost/timer.hpp>
#include<memory>
using namespace std;

void UsageMessage(const char *command);
bool StrStartWith(const char *str, const char *pre);
string ParseCommandLine(int argc, const char *argv[], utils::Properties &props);
bool end_flag_ = false;
utils::Properties *props_ptr = NULL;
size_t DelegateClient(ycsbc::DB *db, ycsbc::CoreWorkload *wl, const size_t num_ops,
    bool is_loading) {
  if(!end_flag_){
    db->Init();
  }
  size_t ops[3] = {0,0,0};
  double durations[] = {0,0,0};
  ycsbc::Client client(*db, *wl);
  size_t oks = 0;
  std::string out_string;
  int skipratio_inload = wl->skipratio_inload;
  size_t offset = 0;
  struct timeval start_insert_time,end_insert_time,res_time;
  gettimeofday(&start_insert_time,NULL);
  for (size_t i = 0; i < num_ops + offset; ++i) {
    if (is_loading) {
      if(skipratio_inload&&i%skipratio_inload!=0){
	//cout<<"before DoInsert(false)"<<endl;
        client.DoInsert(false);
        continue;
      }
      //cout<<"before DoInsert()"<<endl;
      oks += client.DoInsert(true);
    } else {
      if( i < offset){
        oks += client.DoTransaction(ops,durations,false);
        continue;
      }
      oks += client.DoTransaction(ops,durations,true);
    }
    /*if(i>0 && i%10000 == 0){
      if(is_loading && i%100000 == 0){
	cerr<<"operation count:"<<i<<"\r";
      }else if(!is_loading){
	cerr<<"operation count:"<<i<<"\r";
      }
    }*/
  }
  gettimeofday(&end_insert_time,NULL);
  timersub(&end_insert_time,&start_insert_time,&res_time);
  cout<<"oks: "<<oks<<", res_time: "<<res_time.tv_sec<<" s"<<endl;
   
  cout<<endl;
  if(!is_loading){
    cout<<"Write numbers: "<<ops[ycsbc::Operation::INSERT] <<", WRITE latency: "<<durations[ycsbc::Operation::INSERT]/ops[ycsbc::Operation::INSERT]<<" us"<<",  ";
    cout<<"Write ops: "<<ops[ycsbc::Operation::INSERT]/(durations[ycsbc::Operation::INSERT]*1e-6)<<" ops/s"<<endl;
    cout<<"Read numbers: "<<ops[ycsbc::Operation::READ]<<", READ latency: "<<durations[ycsbc::Operation::READ]/ops[ycsbc::Operation::READ]<<" us"<<",  ";
    cout<<"Read ops: "<<ops[ycsbc::Operation::READ]/(durations[ycsbc::Operation::READ]*1e-6)<<" ops/s"<<endl;
    cout<<"Scan numbers: "<<ops[ycsbc::Operation::SCAN]<<", SCAN latency: "<<durations[ycsbc::Operation::SCAN]/ops[ycsbc::Operation::SCAN]<<" us"<<",  ";
    cout<<"Scan ops: "<<ops[ycsbc::Operation::SCAN]/(durations[ycsbc::Operation::SCAN]*1e-6)<<" ops/s"<<endl;
    cout<<"Zero-result lookup: "<<endl;
    cout<<" Zero-result ops: "<<ops[2]<<", total time: "<<durations[2]<<" us"<<endl;
    cout<<" Zero-result throughput: "<<ops[2]/(durations[2]*1e-6)<<" ops/s, "<<"latency: "<<durations[2]/ops[2]<<"us"<<endl;
    //cout<<durations[2]/ops[2]<<"us"<<" Zero-result ops: "<<ops[2]<<endl;
    
    //cerr<<"oks = " << oks<<endl;
    cout<<"oks = " << oks<<endl;
    cout<<"Total time of operate: "<<res_time.tv_sec * 1000000 + res_time.tv_usec<<" us"<<",  ";
    cout<<"Per operate time: "<<(res_time.tv_sec * 1000000 + res_time.tv_usec)*1.0/oks<<" us"<<endl;
    cout<<"throughput: "<<(oks+ops[2])/(res_time.tv_sec + res_time.tv_usec*1e-6)<<" ops/s"<<endl;
    db->doSomeThing("printStats");
    
    /*if(!end_flag_){
      end_flag_ = true;
      ycsbc::CoreWorkload nwl;
      nwl.Init(*props_ptr);
      cout<<"Adjust bloom filter accroding to access frequencies"<<endl;
      //      db->doSomeThing();
      return DelegateClient(db,&nwl,num_ops,is_loading);
    }*/
  }else{
    //cerr<<"oks = " << oks<<endl;
    cout<<"oks = " << oks<<endl;
    cout<<"Total time of insert: "<<res_time.tv_sec * 1000000 + res_time.tv_usec<<" us"<<",  ";
    cout<<"Per insert time: "<<(res_time.tv_sec * 1000000 + res_time.tv_usec)*1.0/oks<<" us"<<endl;
    db->doSomeThing("printStats");
  }
  //db->doSomeThing("printStats");
  //  end_flag_ = true;
  db->Close();
  return oks;
}

int main(const int argc, const char *argv[]) {
  utils::Properties props;
  ycsbc::WallTimer loadRunTimer,totalTimer;
  double loadTime;
  
  totalTimer.Start();
  string file_name = ParseCommandLine(argc, argv, props);
  //fprintf(stderr,"before ycsbc::DBFactory::CreateDB\n");
  ycsbc::DB *db = ycsbc::DBFactory::CreateDB(props);
  //fprintf(stderr,"after ycsbc::DBFactory::CreateDB\n");
  if (!db) {
    cout << "Unknown database name " << props["dbname"] << endl;
    exit(0);
  }
 // cout<<"before wl.Init:"<<endl;
  ycsbc::CoreWorkload wl;
  wl.Init(props);
  props_ptr = &props;
 //  cout<<"before skipLoad"<<endl;
  const size_t num_threads = static_cast<size_t>(stoi(props.GetProperty("threadcount", "1")));
  bool skipLoad = utils::StrToBool(props.GetProperty("skipLoad",
						   "false"));
  // Loads data
 // cout<<"skipLoad:"<<skipLoad<<endl;
  //ycsbc::WallTimer loadRunTimer;
  //loadRunTimer.Start();
  loadRunTimer.Start();
  vector<future<size_t>> actual_ops;
  size_t total_ops = 0;
  sscanf(props[ycsbc::CoreWorkload::RECORD_COUNT_PROPERTY].c_str(),"%zu",&total_ops);
  size_t sum = 0;
  //ycsbc::Client client(*db, wl);
 // cout<<"total_ops:"<<total_ops<<endl;
  if(!skipLoad){/////////////////////////////////////////
	for (size_t i = 0; i < num_threads; ++i) {
	//  cout<<"before load database: "<<total_ops<<endl;
	    actual_ops.emplace_back(async(launch::async,
		DelegateClient, db, &wl, total_ops / num_threads, true));
    //DelegateClient, &client, db, &wl, total_ops / num_threads, true));
	}
	assert(actual_ops.size() == num_threads);
	for (auto &n : actual_ops) {
	    assert(n.valid());
	    sum += n.get();
	}
  loadTime=loadRunTimer.elapsed();
	//cerr << "# Loading records:\t" << sum << ",  ";
  printf("# Loading records:\t:%d, load time: %lf us\n",sum,loadTime);
	////cerr << "load time: " << loadRunTimer.elapsed() <<"us"<<endl;
  //cerr << "load time: " << loadTime <<"us"<<endl;
    
    //loadRunTimer.restart();
    // Peforms transactions
	actual_ops.clear();
   }
  total_ops = stoi(props[ycsbc::CoreWorkload::OPERATION_COUNT_PROPERTY]);
  ycsbc::WallTimer timer;
  //db->openStatistics();
  timer.Start();
  for (size_t i = 0; i < num_threads; ++i) {
    actual_ops.emplace_back(async(launch::async,
        DelegateClient, db, &wl, total_ops / num_threads, false));
        //DelegateClient, &client, db, &wl, total_ops / num_threads, false));
  }
//  db->analysisTableKey();
  assert(actual_ops.size() == num_threads);

  sum = 0;
  for (auto &n : actual_ops) {
    assert(n.valid());
    sum += n.get();
  }
  double duration = timer.elapsed();

  printf("\n# Transaction throughput (KTPS)\n");
  printf("%lf\n",total_ops / (duration/1000000) / 1000);
  printf("%s\t %s \t %d\n",props["dbname"],file_name.c_str(),num_threads);
  printf("run time: %lf us, total run time: %lf\n",timer.elapsed(),totalTimer.elapsed()-loadTime);

  /*cerr<< endl;
  cerr << "# Transaction throughput (KTPS)" << endl;
  cerr << total_ops / (duration/1000000) / 1000 << endl;
  cerr << props["dbname"] << '\t' << file_name << '\t' << num_threads << '\t'<<endl;;
  cerr << "run time: " << timer.elapsed() <<"us"<<", total run time:"<<totalTimer.elapsed()-loadTime<<endl;
*/
  //db->analysisTableKey();
  db->Close();
  delete db;
}

string ParseCommandLine(int argc, const char *argv[], utils::Properties &props) {
  int argindex = 1;
  string filename;
  string latency_filename;
  while (argindex < argc && StrStartWith(argv[argindex], "-")) {
    if (strcmp(argv[argindex], "-threads") == 0) {
      argindex++;
      if (argindex >= argc) {
        UsageMessage(argv[0]);
        exit(0);
      }
      props.SetProperty("threadcount", argv[argindex]);
      argindex++;
    } else if (strcmp(argv[argindex], "-db") == 0) {
      argindex++;
      if (argindex >= argc) {
        UsageMessage(argv[0]);
        exit(0);
      }
      props.SetProperty("dbname", argv[argindex]);
      argindex++;
    } else if (strcmp(argv[argindex], "-host") == 0) {
      argindex++;
      if (argindex >= argc) {
        UsageMessage(argv[0]);
        exit(0);
      }
      props.SetProperty("host", argv[argindex]);
      argindex++;
    } else if (strcmp(argv[argindex], "-port") == 0) {
      argindex++;
      if (argindex >= argc) {
        UsageMessage(argv[0]);
        exit(0);
      }
      props.SetProperty("port", argv[argindex]);
      argindex++;
    } else if (strcmp(argv[argindex], "-slaves") == 0) {
      argindex++;
      if (argindex >= argc) {
        UsageMessage(argv[0]);
        exit(0);
      }
      props.SetProperty("slaves", argv[argindex]);
      argindex++;
    } else if (strcmp(argv[argindex], "-P") == 0) {
      argindex++;
      if (argindex >= argc) {
        UsageMessage(argv[0]);
        exit(0);
      }
      filename.assign(argv[argindex]);
      ifstream input(argv[argindex]);
      try {
        props.Load(input);
      } catch (const string &message) {
        cout << message << endl;
        exit(0);
      }
      input.close();
      argindex++;
    } else if(strcmp(argv[argindex],"-dbfilename") == 0){
	argindex++;
	if(argindex >= argc){
	    UsageMessage(argv[0]);
	    exit(0);
	}
	props.SetProperty("dbfilename", argv[argindex]);
        argindex++;
    }else if(strcmp(argv[argindex],"-configpath") == 0){
	argindex++;
	if(argindex >= argc){
	    UsageMessage(argv[0]);
	    exit(0);
	}
	props.SetProperty("configpath", argv[argindex]);
        argindex++;
    }else if(strcmp(argv[argindex],"-skipLoad") == 0){
	argindex++;
	if(argindex >= argc){
	    UsageMessage(argv[0]);
	    exit(0);
	}
	props.SetProperty("skipLoad", argv[argindex]);
        argindex++;
    }else if(strcmp(argv[argindex],"-requestdistribution") == 0){
	argindex++;
	if(argindex >= argc){
	    UsageMessage(argv[0]);
	    exit(0);
	}
	props.SetProperty("requestdistribution", argv[argindex]);
        argindex++;
   }else if(strcmp(argv[argindex],"-zipfianconst") == 0){
	argindex++;
	if(argindex >= argc){
	    UsageMessage(argv[0]);
	    exit(0);
	}
	props.SetProperty("zipfianconst", argv[argindex]);
        argindex++;
   }else {
      cout << "Unknown option '" << argv[argindex] << "'" << endl;
      exit(0);
    }
  }

  if (argindex == 1 || argindex != argc) {
    UsageMessage(argv[0]);
    exit(0);
  }

  return filename;
}

void UsageMessage(const char *command) {
  cout << "Usage: " << command << " [options]" << endl;
  cout << "Options:" << endl;
  cout << "  -threads n: execute using n threads (default: 1)" << endl;
  cout << "  -db dbname: specify the name of the DB to use (default: basic)" << endl;
  cout << "  -P propertyfile: load properties from the given file. Multiple files can" << endl;
  cout << "                   be specified, and will be processed in the order specified" << endl;
}

inline bool StrStartWith(const char *str, const char *pre) {
  return strncmp(str, pre, strlen(pre)) == 0;
}

