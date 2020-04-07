#include "unikv_db.h"
#include <cstring>
#include"basic_config.hh"
#include<iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <memory>
#include <vector>
#include <unikv/cache.h>
#include<time.h>
using namespace std;

namespace ycsbc {
UniKVDB::UniKVDB(const char* dbfilename,const char* configPath)
{
    leveldb::Options options;
    LevelDB_ConfigMod::getInstance().setConfigPath(configPath);//basic_config.c
    std::string bloom_filename;
    int bloom_bits = LevelDB_ConfigMod::getInstance().getBloom_bits();;
    int max_open_files = LevelDB_ConfigMod::getInstance().getMax_open_files();
    int max_File_sizes = LevelDB_ConfigMod::getInstance().getMax_file_size();
    int bloom_type = LevelDB_ConfigMod::getInstance().getBloomType();
    bool seek_compaction_flag = LevelDB_ConfigMod::getInstance().getSeekCompactionFlag();
    size_t block_cache_size = LevelDB_ConfigMod::getInstance().getBlockCacheSize();
    size_t memTableSize=LevelDB_ConfigMod::getInstance().getMemTableSize();
    int size_ratio = LevelDB_ConfigMod::getInstance().getSizeRatio();

    bool compression_Open = LevelDB_ConfigMod::getInstance().getCompression_flag();
    bool directIO_flag = LevelDB_ConfigMod::getInstance().getDirectIOFlag();
    /*if(directIO_flag){
	   options.opEp_.no_cache_io_ = true;
	   fprintf(stderr,"directIO\n");
	   //    leveldb::setDirectIOFlag(directIO_flag);
    }*/
    if(bloom_type == 1){
    }else if(bloom_type == 0){
	    options.filter_policy = leveldb::NewBloomFilterPolicy(bloom_bits);
    }else if(bloom_type == 2){
    }else{
	    fprintf(stderr,"Wrong filter type!\n");
    }

    options.create_if_missing = true;
    options.compression = compression_Open?leveldb::kSnappyCompression:leveldb::kNoCompression; //compression is disabled.

    options.write_buffer_size=memTableSize;//67108864;
    options.block_size=4096;
    //options.max_file_size = max_File_sizes;
    options.max_open_files = max_open_files;
    //options.opEp_.seek_compaction_ = seek_compaction_flag;
    options.block_cache = leveldb::NewLRUCache(block_cache_size);
    //options.opEp_.size_ratio = size_ratio;
    fprintf(stderr,"block_cache_size %lu, max_open_files:%d,  size_ratio: %d \n",options.block_cache,options.max_open_files,size_ratio);
    fprintf(stderr,"bloom_bits:%d,seek_compaction_flag:%d\n",bloom_bits,seek_compaction_flag);
    //if(LevelDB_ConfigMod::getInstance().getStatisticsOpen()){
    //  options.opEp_.stats_ = leveldb::CreateDBStatistics();
   // }
    leveldb::Status status = leveldb::DB::Open(options,dbfilename, &db_);
    if(!status.ok()){
	fprintf(stderr,"can't open leveldb\n");
	cerr<<status.ToString()<<endl;
	exit(0);
    }
}
bool  UniKVDB::hasRead = false;
int UniKVDB::Read(const string& table, const string& key, const vector< string >* fields, vector< DB::KVPair >& result)
{
    std::string value;
    leveldb::Status s = db_->Get(leveldb::ReadOptions(), key, &value);
    if(s.IsNotFound()){
	// fprintf(stderr,"not found!\n");
	return DB::kErrorNoData;
    }
    if(!s.ok()){
         cerr<<s.ToString()<<endl;
	 fprintf(stderr,"read error\n");
	 exit(0);
    }
    hasRead = true;
    return DB::kOK;
}

int UniKVDB::Insert(const string& table, const string& key, vector< DB::KVPair >& values)
{
    leveldb::Status s;
    int count = 0;
    for(KVPair &p : values){
	//cout<<p.second.length()<<endl;
    /*leveldb::WriteOptions write_options_;
    write_options_=leveldb::WriteOptions();
    write_options_.sync = true;*/
    clock_t start_time=clock();
	s = db_->Put(leveldb::WriteOptions(), key, p.second);
    //s = db_->Put(write_options_, key, p.second);
    clock_t end_time=clock();
    //outW<<static_cast<double>(end_time-start_time)/CLOCKS_PER_SEC*1000<<endl;
	count++;
	if(!s.ok()){
	   fprintf(stderr,"insert error!\n");
	   cout<<s.ToString()<<endl;
	   exit(0);
	}
    }
    if(count != 1){
	  fprintf(stderr,"insert error\n");
	  exit(0);
    }
    return DB::kOK;
}

int UniKVDB::Delete(const string& table, const string& key)
{
    vector<DB::KVPair> values;
    return Insert(table,key,values);
}

int UniKVDB::Scan(const string& table, const string& key, int len, const vector< string >* fields, vector< vector< DB::KVPair > >& result)
{
    vector< DB::KVPair > scanValue;
	std::string value;
    int t = db_->NewIterator(leveldb::ReadOptions(), (char*)key.c_str(), len);
    if(t>0){
        return DB::kOK;
    }else{
        return DB::kErrorNoData;
    }
}

int UniKVDB::Update(const string& table, const string& key, vector< DB::KVPair >& values)
{
    return Insert(table,key,values);
}

void UniKVDB::analysisTableKey(){
	//db_->AnalysisTableKeys(leveldb::ReadOptions());
}

void UniKVDB::Close()
{
    // cerr<<"close is called"<<endl;
    // cerr<<"hasRead value: "<<hasRead<<endl;
    // if(hasRead){
    // 	printAccessFreq();
    // }
}

void UniKVDB::printAccessFreq()
{
    int fd[7],i;
    int levels = 0;
    char buf[100];
    std::string num_files_str;
    snprintf(buf,sizeof(buf),"leveldb.num-files-at-level%d",levels);
    while(levels == 0 || (db_->GetProperty(buf,&num_files_str) && std::stoi(num_files_str)!=0) ){
	levels++;
	snprintf(buf,sizeof(buf),"leveldb.num-files-at-level%d",levels);
    }

    std::string acc_str;
    for(i = 0 ; i <= levels ; i++){
	    snprintf(buf,sizeof(buf),"level%d_access_frequencies.txt",i);
	    fd[i] = open(buf,O_RDWR|O_CREAT);
	    if(fd[i] < 0){
		perror("open :");
	    }
	    snprintf(buf,sizeof(buf),"leveldb.files-access-frequencies%d",i);
	    db_->GetProperty(buf,&acc_str);
	    if(write(fd[i],acc_str.c_str(),acc_str.size()) != (ssize_t)acc_str.size()){
		perror("write :");
	    }
	    close(fd[i]);
    }
}

void UniKVDB::printFilterCount()
{
    int fd[7],i;
    int levels = 0;
    char buf[100];
    static int call_count = 0;
    std::string num_files_str;
    snprintf(buf,sizeof(buf),"leveldb.num-files-at-level%d",levels);
    while(levels == 0 || (db_->GetProperty(buf,&num_files_str) && std::stoi(num_files_str)!=0) ){
	levels++;
	snprintf(buf,sizeof(buf),"leveldb.num-files-at-level%d",levels);
    }

    std::string filter_str;
    for(i = 0 ; i < levels ; i++){
            snprintf(buf,sizeof(buf),"level%d_filter_count_%d.txt",i,call_count);
	    fd[i] = open(buf,O_RDWR|O_CREAT);
	    if(fd[i] < 0){
		perror("open :");
	    }
	    snprintf(buf,sizeof(buf),"leveldb.file_filter_size%d",i);
	    db_->GetProperty(buf,&filter_str);
	    if(write(fd[i],filter_str.c_str(),filter_str.size()) != (ssize_t)filter_str.size()){
		perror("write :");
	    }
	    close(fd[i]);
    }
    call_count++;
}

void UniKVDB::doSomeThing(const char* thing_str)
{
  /*if(flag){
    db_->AnalysisTableKeys(leveldb::ReadOptions());
    }*/
    
  if(strncmp(thing_str,"adjust_filter",strlen("adjust_filter")) == 0){
 
  }else if(strncmp(thing_str,"printFilterCount",strlen("printFilterCount")) == 0){
    printFilterCount();
  }else if(strncmp(thing_str,"printStats",strlen("printStats")) == 0){
    std::string stat_str;
    bool flag = db_->GetProperty("unikv.stats",&stat_str);
    cout<<stat_str<<endl;
     /*if(!db_->GetProperty("leveldb.sstables",&stat_str)){
      stat_str="(failed)";
    }
    cout<<stat_str<<endl;*/
  }

  //////////////////
  cout<<"writeDataSize: "<<db_->writeDataSize<<endl;
  cout<<"writeDataSizeActual: "<<db_->writeDataSizeActual<<endl;
  cout<<"readDataSize: "<<db_->readDataSize<<endl;
  cout<<"readDataSizeActual: "<<db_->readDataSizeActual<<endl;
  cout<<"db_->tableCacheNum: "<<db_->tableCacheNum<<endl;
  cout<<"db_->blockCacheNum: "<<db_->blockCacheNum<<endl;
  cout<<"db_->readIn0: "<<db_->readIn0<<endl;
  cout<<"db_->readIn1: "<<db_->readIn1<<endl;
  //cout<<"db_->readHotMem: "<<db_->readHotMem<<endl;
  cout<<"db_->readMem: "<<db_->readMem<<endl;
  cout<<"db_->readImm: "<<db_->readImm<<endl;
  cout<<"db_->LevelRead: "<<db_->cacheRead<<endl;
  //cout<<" db_->reduceIOInComp: "<< db_->reduceIOInComp<<endl;
  //cout<<" db_->totalIOInComp: "<< db_->totalIOInComp<<endl;
  //cout<<" db_->totalDropKeyN: "<< db_->totalDropKeyN<<endl;
  //////////////////
}

void UniKVDB::openStatistics(){
    std::string stat_str;
    db_->GetProperty("leveldb.stats",&stat_str);
    cout<<"--------------------------- Before Do Transaction -----------------------------------------"<<endl;
    cout<<stat_str<<endl;
    cout<<"----------------------------Transaction Output------------------"<<endl;
}

UniKVDB::~UniKVDB()
{
    delete db_;
}

}
