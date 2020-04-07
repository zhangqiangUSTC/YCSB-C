//
//  client.h
//  YCSB-C
//
//  Created by Jinglei Ren on 12/10/14.
//  Copyright (c) 2014 Jinglei Ren <jinglei@ren.systems>.
//

#ifndef YCSB_C_CLIENT_H_
#define YCSB_C_CLIENT_H_

#include <string>
#include <boost/timer.hpp>
#include <boost/smart_ptr.hpp>
#include "db.h"
#include "core_workload.h"
#include "utils.h"
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/time.h>
#include <fstream>
extern bool end_flag_;

using namespace std;

namespace ycsbc {
static const char iops_filename_[2][50]={"write_iops.txt","read_iops.txt"};
static FILE *f_ops[2];
static size_t last_ops[2]={0,0};

int ZeroFlag = 1;

//std::ofstream outLoad; 
//std::ofstream outfW; 

void *ReportInLoop(void *arg){
  size_t *ops = static_cast<size_t*>(arg);
  while(!end_flag_){
    for(int i = 0 ; i < 2 ; i++){
      if(f_ops[i]){
	size_t current_ops = ops[i];
	fprintf(f_ops[i],"%lu,",current_ops-last_ops[i]);
	last_ops[i] = current_ops;
      }
    }
    usleep(100000);
  }
  for(int i = 0 ; i < 2 ; i++){
    if(f_ops[i]){
      fclose(f_ops[i]);
    }
  }
  return NULL;
}

/* static void __init_file(int *ops){ */
/*   //f_wop = fopen(write_iops_filename,"w"); */
/*   static pthread_t t; */
/*   f_ops[0] = NULL; */
/*   f_ops[1] = fopen(iops_filename_[1],"w"); */
/*   pthread_create(&t,NULL, &ReportInLoop, ops); */
/* } */

class WallTimer{
public:
        static const unsigned long long S2US;
	void Start(){
		    gettimeofday(&start_time_,NULL);
	}
	static unsigned long long inline  timevalTous(struct timeval &res) {
		    return res.tv_sec * S2US + res.tv_usec;
	}
	unsigned long long elapsed(){
	    gettimeofday(&end_time_,NULL);
	    timersub(&end_time_,&start_time_,&res_);
	    return timevalTous(res_); 
	}
private:
	struct timeval start_time_;
	 struct timeval end_time_,res_;
};
const unsigned long long WallTimer::S2US = 1000000;
class Client {
 public:
  Client(DB &db, CoreWorkload &wl) : db_(db), workload_(wl) { 
//    outLoad.open("loadRecord.txt", ios::app);/////////////
//    outfW.open("operateRecord.txt",ios::app);/////////////
	if(wl.with_timestamp_){
	    timestamp_trace_fp_ = wl.timestamp_trace_fp_;
	}else{
	    timestamp_trace_fp_ = NULL;
	}
	if(wl.with_operation_){
	  printf("operation from trace!\n");
	    current_operation_ = boost::make_shared<ycsbc::Operation>(ycsbc::Operation::READ);
	}else{
	    current_operation_ = nullptr;
	}
	if(wl.with_latency_filename_){
	    printf("output latency to filename :%s \n",wl.LATENCYFILENAME_PROPERTY.c_str());
	    latency_fp_ = wl.latency_fp_;
	    std::string nlf_filename_str = "nlf" + std::to_string(wl.initCount) + ".txt"; 	    
	    nlatency_fp_ = fopen(nlf_filename_str.c_str(),"w");
	    if(nlatency_fp_ == NULL){
	      perror("client init:");
	    }
	}else{
	    latency_fp_ = NULL;
	    nlatency_fp_ = NULL;
	}
	line_ = NULL;
	first_do_transaction = false;
  }
  
  virtual bool DoInsert(bool insert_real=true);
  virtual bool DoTransaction(size_t ops[],double durations[],bool do_real=true);
  
  virtual ~Client() { 
//    outLoad.close();
//    outfW.close();
      if(latency_fp_){
	fflush(latency_fp_);
	fflush(nlatency_fp_);
      }
 }
  
 protected:
  
  virtual int TransactionRead(bool do_real);
  virtual int TransactionReadModifyWrite();
  virtual int TransactionScan();
  virtual int TransactionUpdate();
  virtual int TransactionInsert();
  void getCurrentTimeStamp();
  DB &db_;
  CoreWorkload &workload_;
  char *line_;
  boost::shared_ptr<ycsbc::Operation> current_operation_;
  FILE *timestamp_trace_fp_;
  WallTimer timer_;
  bool first_do_transaction;
  double current_time_;
  double current_timestamp_;
  FILE *latency_fp_;
  FILE *nlatency_fp_;
};

inline bool Client::DoInsert(bool insert_real) {
  std::string key = workload_.NextSequenceKey();
  std::vector<DB::KVPair> pairs;
  workload_.BuildValues(pairs);
  if(insert_real){
     /*for(DB::KVPair &p : pairs){
        //printf("in DoInsert()\n");
	outLoad<<"i "<<key.substr(4,key.length())<<" "<<p.second.length()<<std::endl;
     }*/
    return (db_.Insert(workload_.NextTable(), key, pairs) == DB::kOK);
  }
  workload_.NextTable();
  return DB::kOK;
}


inline bool Client::DoTransaction(size_t ops[],double durations[],bool do_real) {
  int status = -1;
  unsigned long long latency;
  if(!first_do_transaction){
	timer_.Start();
	first_do_transaction = true;	
	//	__init_file(ops);
  }
  if(timestamp_trace_fp_){
	 current_time_ = timer_.elapsed();
	 getCurrentTimeStamp();
	double delay =  current_timestamp_ - current_time_;
	if(delay > 0){
	    usleep((unsigned int)delay);
	    printf("current_time_:%.2lf current_timestamp_:%.2lf delay:%.2lf \n",current_time_,current_timestamp_,delay);
	}
  }
 WallTimer transaction_timer;
 transaction_timer.Start();
  ycsbc::Operation operations = current_operation_ == nullptr ?workload_.NextOperation():*current_operation_;
  switch (operations) {
    case READ:
      status = TransactionRead(do_real);
      latency = transaction_timer.elapsed();
        if(status == DB::kOK){
	    //printf("--------- find data!!\n");
	    //outfW<<"-find data!!"<<std::endl;
        }else if(status == DB::kErrorNoData){
            //printf("---------Not find data!!\n");
            //outfW<<"-Not find data!!"<<std::endl;
            //fprintf(nlatency_fp_,"%llu,",latency);
            ops[2]++;
            durations[2] += latency;
          }

      if(do_real){
        durations[READ] += latency;
        if(latency_fp_  != NULL){
          if(status == DB::kOK){
            //printf("--------- find data!!\n");
            //outfW<<"-find data!!"<<std::endl;
            fprintf(latency_fp_,"%llu,",latency);
          }else if(status == DB::kErrorNoData){
            //printf("---------Not find data!!\n");
            //outfW<<"-Not find data!!"<<std::endl;
            fprintf(nlatency_fp_,"%llu,",latency);
            ops[2]++;
            durations[2] += latency;
          }
        }
        ops[READ]++;
      }
      break;
    case UPDATE:
      status = TransactionUpdate();
      durations[INSERT] += (transaction_timer.elapsed());
      ops[INSERT]++;
      break;
    case INSERT:
      status = TransactionInsert();
      durations[INSERT] += (transaction_timer.elapsed());
      ops[INSERT]++;
      break;
    case SCAN:
      status = TransactionScan();
      break;
    case READMODIFYWRITE:
      status = TransactionReadModifyWrite();
      break;
    default:
      throw utils::Exception("Operation request is not recognized!");
  }
  assert(status >= 0);
  return (status == DB::kOK);
}

inline int Client::TransactionRead(bool do_real) {
  const std::string &table = workload_.NextTable();

/*if(ZeroFlag){
  const std::string &key = workload_.NextAbsentKey();
  ZeroFlag = 0;
  std::vector<DB::KVPair> result;
  if (!workload_.read_all_fields()) {
    std::vector<std::string> fields;
    fields.push_back("field" + workload_.NextFieldName());
    if(do_real){
      return db_.Read(table, key, &fields, result);
    }
  } else {
    if(do_real){
      return db_.Read(table, key, NULL, result);
    }
  }
}else{*/
  const std::string &key = workload_.NextTransactionKey();
  //const std::string &key = workload_.NextAbsentKey();
  ZeroFlag = 1;
  std::vector<DB::KVPair> result;
  if (!workload_.read_all_fields()) {
    std::vector<std::string> fields;
    fields.push_back("field" + workload_.NextFieldName());
    if(do_real){
      return db_.Read(table, key, &fields, result);
    }
  } else {
    if(do_real){
      return db_.Read(table, key, NULL, result);
    }
  }
//}
  return DB::kOK;
}

inline int Client::TransactionReadModifyWrite() {
  const std::string &table = workload_.NextTable();
  const std::string &key = workload_.NextTransactionKey();
  std::vector<DB::KVPair> result;

  if (!workload_.read_all_fields()) {
    std::vector<std::string> fields;
    fields.push_back("field" + workload_.NextFieldName());
//    outfW<<"r  "<<key.substr(4,key.length())<<std::endl;
    db_.Read(table, key, &fields, result);
  } else {
//    outfW<<"r "<<key.substr(4,key.length())<<std::endl;
    db_.Read(table, key, NULL, result);
  }

  std::vector<DB::KVPair> values;
  if (workload_.write_all_fields()) {
    workload_.BuildValues(values);
  } else {
    workload_.BuildUpdate(values);
  }
  /*for(DB::KVPair &p : values){
	//outfW<<"WRITE: "<<key<<", "<<p.second<<std::endl;
	outfW<<"u "<<key.substr(4,key.length())<<" "<<p.second.length()<<std::endl;
     }*/
   
  return db_.Update(table, key, values);
}

inline int Client::TransactionScan() {////////////////////////////////////////
  const std::string &table = workload_.NextTable();
  const std::string &key = workload_.NextTransactionKey();
  int len = workload_.NextScanLength();
  std::vector<std::vector<DB::KVPair>> result;
  if (!workload_.read_all_fields()) {
    std::vector<std::string> fields;
    fields.push_back("field" + workload_.NextFieldName());
    //outfW<<"s "<<key.substr(4,key.length())<<" "<<len<<std::endl;
    return db_.Scan(table, key, len, &fields, result);
  } else {
    //outfW<<"s "<<key.substr(4,key.length())<<" "<<len<<std::endl;
    return db_.Scan(table, key, len, NULL, result);
  }
}

inline int Client::TransactionUpdate() {
  const std::string &table = workload_.NextTable();
  const std::string &key = workload_.NextTransactionKey();
  std::vector<DB::KVPair> values;
  if (workload_.write_all_fields()) {
    workload_.BuildValues(values);
  } else {
    workload_.BuildUpdate(values);
  }
    /*for(DB::KVPair &p : values){
	//outfW<<"WRITE: "<<key<<", "<<p.second<<std::endl;
	outfW<<"u "<<key.substr(4,key.length())<<" "<<p.second.length()<<std::endl;
     }*/
 // outfW<<"UPDATE: "<<key<<", "<<values<<std::endl;
  return db_.Update(table, key, values);
}

inline int Client::TransactionInsert() {
  const std::string &table = workload_.NextTable();
  const std::string &key = workload_.NextSequenceKey();
  std::vector<DB::KVPair> values;
  workload_.BuildValues(values);
    /*for(DB::KVPair &p : values){
	outfW<<"i "<<key.substr(4,key.length())<<" "<<p.second.length()<<std::endl;
     }*/
   //outfW<<"WRITE: "<<key<<", "<<values<<std::endl;
  return db_.Insert(table, key, values);
} 

inline void Client::getCurrentTimeStamp(){
	size_t len = 0;
	char operation;
	getline(&line_,&len,timestamp_trace_fp_);
	if(current_operation_){
	  if(sscanf(line_,"%lf,%c",&current_timestamp_,&operation)!=2){
	    fprintf(stderr,"error in sscanf");
	    exit(0);
	  }
	    *current_operation_ = operation == 'w' ? ycsbc::Operation::UPDATE:ycsbc::Operation::READ;
	}else{
	  if(sscanf(line_,"%lf,",&current_timestamp_)!=1){
	    fprintf(stderr,"error in sscanf");
	    exit(0);
	  }
	}
}

} // ycsbc

#endif // YCSB_C_CLIENT_H_
