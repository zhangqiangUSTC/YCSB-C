//
//  core_workload.h
//  YCSB-C
//
//  Created by Jinglei Ren on 12/9/14.
//  Copyright (c) 2014 Jinglei Ren <jinglei@ren.systems>.
//

#ifndef YCSB_C_CORE_WORKLOAD_H_
#define YCSB_C_CORE_WORKLOAD_H_

#include <vector>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include "db.h"
#include "properties.h"
#include "generator.h"
#include "discrete_generator.h"
#include "counter_generator.h"
#include "utils.h"
#include <boost/timer.hpp>
namespace ycsbc {

enum Operation {
  INSERT,
  READ,
  UPDATE,
  SCAN,
  READMODIFYWRITE
};

class CoreWorkload {
 public:
  /// 
  /// The name of the database table to run queries against.
  ///
  static const std::string TABLENAME_PROPERTY;
  static const std::string TABLENAME_DEFAULT;
  
  /// 
  /// The name of the property for the number of fields in a record.
  ///
  static const std::string FIELD_COUNT_PROPERTY;
  static const std::string FIELD_COUNT_DEFAULT;
  
  /// 
  /// The name of the property for the field length distribution.
  /// Options are "uniform", "zipfian" (favoring short records), and "constant".
  ///
  static const std::string FIELD_LENGTH_DISTRIBUTION_PROPERTY;
  static const std::string FIELD_LENGTH_DISTRIBUTION_DEFAULT;
  
  /// 
  /// The name of the property for the length of a field in bytes.
  ///
  static const std::string FIELD_LENGTH_PROPERTY;
  static const std::string FIELD_LENGTH_DEFAULT;
  
  /// 
  /// The name of the property for deciding whether to read one field (false)
  /// or all fields (true) of a record.
  ///
  static const std::string READ_ALL_FIELDS_PROPERTY;
  static const std::string READ_ALL_FIELDS_DEFAULT;

  /// 
  /// The name of the property for deciding whether to write one field (false)
  /// or all fields (true) of a record.
  ///
  static const std::string WRITE_ALL_FIELDS_PROPERTY;
  static const std::string WRITE_ALL_FIELDS_DEFAULT;
  
  /// 
  /// The name of the property for the proportion of read transactions.
  ///
  static const std::string READ_PROPORTION_PROPERTY;
  static const std::string READ_PROPORTION_DEFAULT;
  
  /// 
  /// The name of the property for the proportion of update transactions.
  ///
  static const std::string UPDATE_PROPORTION_PROPERTY;
  static const std::string UPDATE_PROPORTION_DEFAULT;
  
  /// 
  /// The name of the property for the proportion of insert transactions.
  ///
  static const std::string INSERT_PROPORTION_PROPERTY;
  static const std::string INSERT_PROPORTION_DEFAULT;
  
  /// 
  /// The name of the property for the proportion of scan transactions.
  ///
  static const std::string SCAN_PROPORTION_PROPERTY;
  static const std::string SCAN_PROPORTION_DEFAULT;
  
  ///
  /// The name of the property for the proportion of
  /// read-modify-write transactions.
  ///
  static const std::string READMODIFYWRITE_PROPORTION_PROPERTY;
  static const std::string READMODIFYWRITE_PROPORTION_DEFAULT;
  
  /// 
  /// The name of the property for the the distribution of request keys.
  /// Options are "uniform", "zipfian" and "latest".
  ///
  static const std::string REQUEST_DISTRIBUTION_PROPERTY;
  static const std::string REQUEST_DISTRIBUTION_DEFAULT;
  static const std::string ZIPFIAN_CONST;
  static const std::string ZIPFIAN_CONST_DEFAULT;
  /// 
  /// The name of the property for the max scan length (number of records).
  ///
  static const std::string MAX_SCAN_LENGTH_PROPERTY;
  static const std::string MAX_SCAN_LENGTH_DEFAULT;
  
  /// 
  /// The name of the property for the scan length distribution.
  /// Options are "uniform" and "zipfian" (favoring short scans).
  ///
  static const std::string SCAN_LENGTH_DISTRIBUTION_PROPERTY;
  static const std::string SCAN_LENGTH_DISTRIBUTION_DEFAULT;

  /// 
  /// The name of the property for the order to insert records.
  /// Options are "ordered" or "hashed".
  ///
  static const std::string INSERT_ORDER_PROPERTY;
  static const std::string INSERT_ORDER_DEFAULT;

  static const std::string INSERT_START_PROPERTY;
  static const std::string INSERT_START_DEFAULT;
  
  static const std::string RECORD_COUNT_PROPERTY;
  static const std::string OPERATION_COUNT_PROPERTY;
  
  static const std::string WITH_TIMESTAMP_PROPERTY;
  static const std::string WITH_TIMESTAMP_PROPERTY_DEFAULT;
  
  static const std::string WITH_OPERATION_PROPERTY;
  static const std:: string WITH_OPERATION_PROPERTY_DEFAULT;
  
  static const std::string TIMESTAMP_TRACEFILENAME_PROPERTY;
  static const std::string TIMESTAMP_TRACEFILENAME_PROPERTY_DEFAULT;
  
  static const std::string WITH_LATENCY_PROPERTY;
  static const std::string WITH_LATENCY_PROPERTY_DEFAULT;
  
  static const std::string LATENCYFILENAME_PROPERTY ;
  static const std::string LATENCYFILENAME_PROPERTY_DEFAULT;
  static const std::string SKIPRATIO_INLOAD_PROPERTY;
  static const std::string SKIPRATIO_INLOAD_PROPERTY_DEFAULT; 
  static const std::string ADJUST_FILTER_PROPERTY;
  static const std::string ADJUST_FILTER_PROPERTY_DEFAULT;
  static const std::string HOTSPOT_DATA_FRACTION;
  static const std::string HOTSPOT_DATA_FRACTION_DEFAULT ;
  static const std::string HOTSPOT_OPN_FRACTION ;
  static const std::string HOTSPOT_OPN_FRACTION_DEFAULT ;
  static int initCount;
  ///
  /// Initialize the scenario.
  /// Called once, in the main client thread, before any operations are started.
  ///
  virtual void Init(const utils::Properties &p);
  
  virtual void BuildValues(std::vector<ycsbc::DB::KVPair> &values);
  virtual void BuildUpdate(std::vector<ycsbc::DB::KVPair> &update);
  
  virtual std::string NextTable() { return table_name_; }
  virtual std::string NextSequenceKey(); /// Used for loading data
  virtual std::string NextTransactionKey(); /// Used for transactions
  virtual std::string NextAbsentKey();

  virtual Operation NextOperation() { return op_chooser_.Next(); }
  virtual std::string NextFieldName();
  virtual size_t NextScanLength() { return scan_len_chooser_->Next(); }
  
  bool read_all_fields() const { return read_all_fields_; }
  bool write_all_fields() const { return write_all_fields_; }

  CoreWorkload() :
      field_count_(0), read_all_fields_(false), write_all_fields_(false),
      field_len_generator_(NULL), key_generator_(NULL), key_chooser_(NULL),
      field_chooser_(NULL), scan_len_chooser_(NULL), insert_key_sequence_(3),
      ordered_inserts_(true), record_count_(0) {
  }
  
  virtual ~CoreWorkload() {
    if (field_len_generator_) delete field_len_generator_;
    if (key_generator_) delete key_generator_;
    if (key_chooser_) delete key_chooser_;
    if (field_chooser_) delete field_chooser_;
    if (scan_len_chooser_) delete scan_len_chooser_;
    if(timestamp_trace_fp_) fclose(timestamp_trace_fp_);
  }
  
 protected:
  static Generator<uint64_t> *GetFieldLenGenerator(const utils::Properties &p);
  std::string BuildKeyName(uint64_t key_num);

  std::string table_name_;
  int field_count_;
  bool read_all_fields_;
  bool write_all_fields_;
  Generator<uint64_t> *field_len_generator_;
  Generator<uint64_t> *key_generator_;
  DiscreteGenerator<Operation> op_chooser_;
  Generator<uint64_t> *key_chooser_;
  Generator<uint64_t> *field_chooser_;
  Generator<uint64_t> *scan_len_chooser_;
  CounterGenerator insert_key_sequence_;
  bool ordered_inserts_;
  size_t record_count_;
public:
  bool with_timestamp_;
  bool with_operation_; //operation come from trace;
  bool with_latency_filename_;
  FILE *timestamp_trace_fp_;
  FILE *latency_fp_;
  int skipratio_inload;
  bool adjust_filter_;
};

inline std::string CoreWorkload::NextSequenceKey() {
  uint64_t key_num = key_generator_->Next();
  return BuildKeyName(key_num);
}

inline std::string CoreWorkload::NextTransactionKey() {
  uint64_t key_num;
  do {
    key_num = key_chooser_->Next();
  } while (key_num > insert_key_sequence_.Last());
  return BuildKeyName(key_num);
}

inline std::string CoreWorkload::NextAbsentKey(){
  uint64_t key_num;
  do {
    key_num = key_chooser_->Next();
  } while (key_num > insert_key_sequence_.Last());
  key_num += insert_key_sequence_.Last();
  return BuildKeyName(key_num);
}

inline std::string CoreWorkload::BuildKeyName(uint64_t key_num) {
  char key[100];
  if (!ordered_inserts_) {
    key_num = utils::Hash(key_num);
  }
//  key_num %= insert_key_sequence_.Last();/////////////////////////////
  snprintf(key, sizeof(key), "%020lu", key_num);
  return std::string("user").append(std::string(key,20));
}

inline std::string CoreWorkload::NextFieldName() {
  return std::string("field").append(std::to_string(field_chooser_->Next()));
}
  
} // ycsbc

#endif // YCSB_C_CORE_WORKLOAD_H_
