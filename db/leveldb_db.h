#ifndef YCSB_C_LEVELDB_H
#define YCSB_C_LEVELDB_H
#include "core/db.h"
#include <iostream>
#include <string>
#include "core/properties.h"
#include <leveldb/db.h>
#include <leveldb/env.h>
#include<leveldb/filter_policy.h>
//#include<leveldb/statistics.h>
#include <fstream>
using std::cout;
using std::endl;

namespace ycsbc {
class LevelDB : public DB{
public :
    LevelDB(const char *dbfilename,const char* configPath);
    int Read(const std::string &table, const std::string &key,
           const std::vector<std::string> *fields,
           std::vector<KVPair> &result);

  int Scan(const std::string &table, const std::string &key,
           int len, const std::vector<std::string> *fields,
           std::vector<std::vector<KVPair>> &result) ;

  int Update(const std::string &table, const std::string &key,
             std::vector<KVPair> &values);

  int Insert(const std::string &table, const std::string &key,
             std::vector<KVPair> &values) ;

  int Delete(const std::string &table, const std::string &key);
  void openStatistics();
  void printAccessFreq();
  virtual ~LevelDB();
  virtual void doSomeThing(const char *thing_str="adjust_filter");
  virtual void analysisTableKey();
  void Close();
private:
    leveldb::DB *db_;
    std::ofstream outW; 
    static bool hasRead;
    void printFilterCount();
};
}
#endif
