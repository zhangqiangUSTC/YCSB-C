#include"basic_config.hh"
#include<iostream>
template<typename T>
void Basic_ConfigMod<T>::setConfigPath (const char* path) {
    boost::property_tree::ini_parser::read_ini(path, _pt);
    assert (!_pt.empty());
}
template<typename T>
bool Basic_ConfigMod<T>::readBool (const char* key) {
    return _pt.get<bool>(key);
}
template<typename T>
int Basic_ConfigMod<T>::readInt (const char* key) {
    return _pt.get<int>(key);
}
template<typename T>
std::string Basic_ConfigMod<T>::readString (const char* key) {
    return _pt.get<std::string>(key);
}

template<typename T>
double Basic_ConfigMod<T>::readFloat(const char *key){
    return _pt.get<double>(key);
}

template<typename T>
size_t Basic_ConfigMod<T>::readSize_t(const char *key){
   return _pt.get<size_t>(key);
}


void LevelDB_ConfigMod::setConfigPath(const char*path){
    boost::property_tree::ini_parser::read_ini(path, _pt);
    assert (!_pt.empty());
    //_bloom_filename = readString("basic.bloomFilename");
    _bloom_bits = readInt("basic.bloomBits");
   // std::cout<<"_bloom_bits_: "<<_bloom_bits<<std::endl;////////////////////////////////////
    _max_file_size = readInt("basic.maxFilesize");
    _max_open_files = readInt("basic.maxOpenfiles");
    _bloom_type = readInt("basic.bloomType");
    _open_log = readBool("basic.openLog");
    _compression_flag = readBool("basic.compressionFlag");
    _directIO_flag = readBool("basic.directIOFlag");
    _seek_compaction_flag = readBool("basic.seekCompactionFlag");
    _statistics_open = readBool("basic.statisticsOpen");
    //_bloom_bits_array_filename = readString("basic.bitsArrayFilename");
    _blockCacheSize = readSize_t("basic.blockCacheSize");
    _MemTableSize=readSize_t("basic.memTableSize");
}
/*template<typename T>
boost::shared_ptr<T> Basic_ConfigMod<T>::instance= nullptr;*/
std::string LevelDB_ConfigMod::getBloom_filename(){
    assert(!_pt.empty());
    return _bloom_filename;
}

int LevelDB_ConfigMod::getBloom_bits(){
    assert(!_pt.empty());
    return _bloom_bits;
}

int LevelDB_ConfigMod::getMax_file_size(){
    assert(!_pt.empty());
    return _max_file_size;
}

int LevelDB_ConfigMod::getMax_open_files(){
    assert(!_pt.empty());
    return _max_open_files;
}

int LevelDB_ConfigMod::getBloomType(){
    assert(!_pt.empty());
    return _bloom_type;
}

bool LevelDB_ConfigMod::getOpen_log(){
     assert(!_pt.empty());
     return _open_log;
}

bool LevelDB_ConfigMod::getCompression_flag(){
    assert(!_pt.empty());
    return _compression_flag;
}

bool LevelDB_ConfigMod::getDirectIOFlag(){
    assert(!_pt.empty());
    return _directIO_flag;
}

bool LevelDB_ConfigMod::getSeekCompactionFlag()
{
   assert(!_pt.empty());
   return _seek_compaction_flag;
}

bool LevelDB_ConfigMod::getStatisticsOpen()
{
   assert(!_pt.empty());
   return _statistics_open;
}

std::string LevelDB_ConfigMod::getBitsArrayFilename(){
    assert(!_pt.empty());
    return _bloom_bits_array_filename;
}

int LevelDB_ConfigMod::getLRUsNum()
{
    assert(!_pt.empty());
    return _lrus_num;
}

double LevelDB_ConfigMod::getFiltersCapacityRatio()
{
    assert(!_pt.empty());
    return _filters_capacity_ratio;
}
size_t LevelDB_ConfigMod::getBlockCacheSize(){
    assert(!_pt.empty());
    return _blockCacheSize;
}

size_t LevelDB_ConfigMod::getMemTableSize(){
    assert(!_pt.empty());
    return _MemTableSize;
}

int LevelDB_ConfigMod::getSizeRatio()
{
    assert(!_pt.empty());
    return _sizeRatio;
}


template<>
boost::shared_ptr<LevelDB_ConfigMod> Basic_ConfigMod<LevelDB_ConfigMod>::instance = nullptr;	


void PebblesDB_ConfigMod::setConfigPath(const char*path){
    boost::property_tree::ini_parser::read_ini(path, _pt);
    assert (!_pt.empty());
    //_bloom_filename = readString("basic.bloomFilename");
    _bloom_bits = readInt("basic.bloomBits");
   // std::cout<<"_bloom_bits_: "<<_bloom_bits<<std::endl;////////////////////////////////////
    _max_file_size = readInt("basic.maxFilesize");
    _max_open_files = readInt("basic.maxOpenfiles");
    _bloom_type = readInt("basic.bloomType");
    _open_log = readBool("basic.openLog");
    _compression_flag = readBool("basic.compressionFlag");
    _directIO_flag = readBool("basic.directIOFlag");
    _seek_compaction_flag = readBool("basic.seekCompactionFlag");
    _statistics_open = readBool("basic.statisticsOpen");
    //_bloom_bits_array_filename = readString("basic.bitsArrayFilename");
    //_lrus_num = readInt("basic.LRUNum");
    //_filters_capacity_ratio = readFloat("basic.FilterCapacityRatio");
    _blockCacheSize = readSize_t("basic.blockCacheSize");
    //_sizeRatio = readInt("basic.sizeRatio");
    _MemTableSize=readSize_t("basic.memTableSize");
}
/*template<typename T>
boost::shared_ptr<T> Basic_ConfigMod<T>::instance= nullptr;*/
std::string PebblesDB_ConfigMod::getBloom_filename(){
    assert(!_pt.empty());
    return _bloom_filename;
}

int PebblesDB_ConfigMod::getBloom_bits(){
    assert(!_pt.empty());
    return _bloom_bits;
}

int PebblesDB_ConfigMod::getMax_file_size(){
    assert(!_pt.empty());
    return _max_file_size;
}

int PebblesDB_ConfigMod::getMax_open_files(){
    assert(!_pt.empty());
    return _max_open_files;
}

int PebblesDB_ConfigMod::getBloomType(){
    assert(!_pt.empty());
    return _bloom_type;
}

bool PebblesDB_ConfigMod::getOpen_log(){
     assert(!_pt.empty());
     return _open_log;
}

bool PebblesDB_ConfigMod::getCompression_flag(){
    assert(!_pt.empty());
    return _compression_flag;
}

bool PebblesDB_ConfigMod::getDirectIOFlag(){
    assert(!_pt.empty());
    return _directIO_flag;
}

bool PebblesDB_ConfigMod::getSeekCompactionFlag()
{
   assert(!_pt.empty());
   return _seek_compaction_flag;
}

bool PebblesDB_ConfigMod::getStatisticsOpen()
{
   assert(!_pt.empty());
   return _statistics_open;
}

std::string PebblesDB_ConfigMod::getBitsArrayFilename(){
    assert(!_pt.empty());
    return _bloom_bits_array_filename;
}

int PebblesDB_ConfigMod::getLRUsNum()
{
    assert(!_pt.empty());
    return _lrus_num;
}

double PebblesDB_ConfigMod::getFiltersCapacityRatio()
{
    assert(!_pt.empty());
    return _filters_capacity_ratio;
}
size_t PebblesDB_ConfigMod::getBlockCacheSize(){
    assert(!_pt.empty());
    return _blockCacheSize;
}

size_t PebblesDB_ConfigMod::getMemTableSize(){
    assert(!_pt.empty());
    return _MemTableSize;
}

int PebblesDB_ConfigMod::getSizeRatio()
{
    assert(!_pt.empty());
    return _sizeRatio;
}


template<>
boost::shared_ptr<PebblesDB_ConfigMod> Basic_ConfigMod<PebblesDB_ConfigMod>::instance = nullptr;	

void RocksDB_ConfigMod::setConfigPath(const char*path){
    boost::property_tree::ini_parser::read_ini(path, _pt);
    assert (!_pt.empty());
    //_bloom_filename = readString("basic.bloomFilename");
    _bloom_bits = readInt("basic.bloomBits");
   // std::cout<<"_bloom_bits_: "<<_bloom_bits<<std::endl;////////////////////////////////////
    _max_file_size = readInt("basic.maxFilesize");
    _max_open_files = readInt("basic.maxOpenfiles");
    _bloom_type = readInt("basic.bloomType");
    _open_log = readBool("basic.openLog");
    _compression_flag = readBool("basic.compressionFlag");
    _directIO_flag = readBool("basic.directIOFlag");
    _seek_compaction_flag = readBool("basic.seekCompactionFlag");
    _statistics_open = readBool("basic.statisticsOpen");
    //_bloom_bits_array_filename = readString("basic.bitsArrayFilename");
   // _lrus_num = readInt("basic.LRUNum");
   // _filters_capacity_ratio = readFloat("basic.FilterCapacityRatio");
    _blockCacheSize = readSize_t("basic.blockCacheSize");
    //_sizeRatio = readInt("basic.sizeRatio");
    _MemTableSize=readSize_t("basic.memTableSize");
}
/*template<typename T>
boost::shared_ptr<T> Basic_ConfigMod<T>::instance= nullptr;*/
std::string RocksDB_ConfigMod::getBloom_filename(){
    assert(!_pt.empty());
    return _bloom_filename;
}

int RocksDB_ConfigMod::getBloom_bits(){
    assert(!_pt.empty());
    return _bloom_bits;
}

int RocksDB_ConfigMod::getMax_file_size(){
    assert(!_pt.empty());
    return _max_file_size;
}

int RocksDB_ConfigMod::getMax_open_files(){
    assert(!_pt.empty());
    return _max_open_files;
}

int RocksDB_ConfigMod::getBloomType(){
    assert(!_pt.empty());
    return _bloom_type;
}

bool RocksDB_ConfigMod::getOpen_log(){
     assert(!_pt.empty());
     return _open_log;
}

bool RocksDB_ConfigMod::getCompression_flag(){
    assert(!_pt.empty());
    return _compression_flag;
}

bool RocksDB_ConfigMod::getDirectIOFlag(){
    assert(!_pt.empty());
    return _directIO_flag;
}

bool RocksDB_ConfigMod::getSeekCompactionFlag()
{
   assert(!_pt.empty());
   return _seek_compaction_flag;
}

bool RocksDB_ConfigMod::getStatisticsOpen()
{
   assert(!_pt.empty());
   return _statistics_open;
}

std::string RocksDB_ConfigMod::getBitsArrayFilename(){
    assert(!_pt.empty());
    return _bloom_bits_array_filename;
}

int RocksDB_ConfigMod::getLRUsNum()
{
    assert(!_pt.empty());
    return _lrus_num;
}

double RocksDB_ConfigMod::getFiltersCapacityRatio()
{
    assert(!_pt.empty());
    return _filters_capacity_ratio;
}
size_t RocksDB_ConfigMod::getBlockCacheSize(){
    assert(!_pt.empty());
    return _blockCacheSize;
}

size_t RocksDB_ConfigMod::getMemTableSize(){
    assert(!_pt.empty());
    return _MemTableSize;
}

int RocksDB_ConfigMod::getSizeRatio()
{
    assert(!_pt.empty());
    return _sizeRatio;
}


template<>
boost::shared_ptr<RocksDB_ConfigMod> Basic_ConfigMod<RocksDB_ConfigMod>::instance = nullptr;	
