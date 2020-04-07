#ifndef YCSB_C_UNIKVDB_H
#define YCSB_C_UNIKVDB_H
#include "core/db.h"
#include <iostream>
#include <string>
#include "core/properties.h"
#include <unikv/db.h>
#include <unikv/env.h>
#include<unikv/filter_policy.h>
//#include<unikv/statistics.h>
#include <fstream>
using std::cout;
using std::endl;

namespace ycsbc {
class UniKVDB : public DB{
public :
    UniKVDB(const char *dbfilename,const char* configPath);
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
  virtual ~UniKVDB();
  virtual void doSomeThing(const char *thing_str);
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
