#!/bin/bash
function __runGLSM(){

#./ycsbc -db leveldb -threads 1 -P $workload_name -dbfilename "/home/zq/SSD-e/temp" -configpath "$configpath" -skipLoad false > partitionResult-leveldb-load-scan-2.txt
#sync; echo 3 > /proc/sys/vm/drop_caches
#./ycsbc -db leveldb -threads 1 -P $workload_name -dbfilename "/home/zq/SSD-e/temp1" -configpath "$configpath" -skipLoad false > partitionResult-leveldb-load-read-nb-a.txt
#sync; echo 3 > /proc/sys/vm/drop_caches
#sleep 0.2h
#ulimit -n 50000
#./ycsbc -db leveldb -threads 1 -P $workload_name -dbfilename "/home/zq/SSD-e/temp1" -configpath "$configpath" -skipLoad false > DiffKV-ycsb-a-14-30.txt
#./ycsbc -db hyperleveldb -threads 1 -P $workload_name -dbfilename "/home/zq/SSD-d/temp1" -configpath "$configpath" -skipLoad false > hyperleveldb-ycsb-a.txt
#./ycsbc -db pebblesdb -threads 1 -P $workload_name1 -dbfilename "/home/zq/SSD-d/temp2" -configpath "$configpath" -skipLoad false > pebblesdb-ycsb-b.txt
#ulimit -n 50000
#./ycsbc -db hyperleveldb -threads 1 -P $workload_name2 -dbfilename "/home/zq/SSD-d/temp1" -configpath "$configpath" -skipLoad false > hyperleveldb-ycsb-c.txt
#ulimit -n 50000
#./ycsbc -db hyperleveldb -threads 1 -P $workload_name3 -dbfilename "/home/zq/SSD-d/temp2" -configpath "$configpath" -skipLoad false > hyperleveldb-ycsb-d.txt
#ulimit -n 50000
#./ycsbc -db hyperleveldb -threads 1 -P $workload_name4 -dbfilename "/home/zq/SSD-d/temp3" -configpath "$configpath" -skipLoad false > hyperleveldb-ycsb-e.txt
#ulimit -n 50000
#./ycsbc -db hyperleveldb -threads 1 -P $workload_name5 -dbfilename "/home/zq/SSD-d/temp1" -configpath "$configpath" -skipLoad false > hyperleveldb-ycsb-f.txt
#ulimit -n 50000
#./ycsbc -db pebblesdb -threads 1 -P $workload_name5 -dbfilename "/home/zq/SSD-d/temp1" -configpath "$configpath" -skipLoad false > pebblesdb-ycsb-f.txt
#ulimit -n 50000
#rm -r ../persitentIndexDir/B_TreeStore.txt
#rm -r ../persitentIndexDir/hashTableStore.txt
#./ycsbc -db leveldb -threads 1 -P $workload_name -dbfilename "/home/zq/SSD-d/temp1" -configpath "$configpath" -skipLoad false > UniKV-ycsb-a.txt
#ulimit -n 50000
#rm -r ../persitentIndexDir/B_TreeStore.txt
#rm -r ../persitentIndexDir/hashTableStore.txt
#./ycsbc -db leveldb -threads 1 -P $workload_name1 -dbfilename "/home/zq/SSD-e/temp1" -configpath "$configpath" -skipLoad false > UniKV-ycsb-b-2.txt
#ulimit -n 50000

#./ycsbc -db leveldb -threads 1 -P $workload_name2 -dbfilename "/home/zq/SSD-e/temp1" -configpath "$configpath" -skipLoad false > UniKV-ycsb-c-2-50M.txt
#./ycsbc -db leveldb -threads 1 -P $workload_name2 -dbfilename "/home/zq/SSD-e/temp1" -configpath "$configpath" -skipLoad false > rocksdb-ycsb-c-50M.txt
#ulimit -n 50000
#./ycsbc -db leveldb -threads 1 -P $workload_name4 -dbfilename "/home/zq/SSD-d/temp1" -configpath "$configpath" -skipLoad false > UniKV-ycsb-e-50M--1.txt
#ulimit -n 50000
#ulimit -n 50000
#./ycsbc -db rocksdb -threads 1 -P $workload_name3 -dbfilename "/home/zq/SSD-e/temp2" -configpath "$configpath" -skipLoad false > rocksdb-ycsb-d-50M.txt

rm -r ../persitentIndexDir/B_TreeStore.txt
rm -r ../persitentIndexDir/hashTableStore.txt
ulimit -n 500000
./ycsbc -db leveldb -threads 1 -P $workload_name4 -dbfilename "/home/zq/SSD-e/temp1" -configpath "$configpath" -skipLoad false > UniKV-ycsb-e-50M-v21.txt

rm -r ../persitentIndexDir/B_TreeStore.txt
rm -r ../persitentIndexDir/hashTableStore.txt
ulimit -n 500000
./ycsbc -db leveldb -threads 1 -P $workload_name -dbfilename "/home/zq/SSD-d/temp1" -configpath "$configpath" -skipLoad false > UniKV-ycsb-a-50M-v21.txt

rm -r ../persitentIndexDir/B_TreeStore.txt
rm -r ../persitentIndexDir/hashTableStore.txt
ulimit -n 500000
./ycsbc -db leveldb -threads 1 -P $workload_name3 -dbfilename "/home/zq/SSD-d/temp2" -configpath "$configpath" -skipLoad false > UniKV-ycsb-d-50M-v21.txt

#rm -r ../persitentIndexDir/B_TreeStore.txt
#rm -r ../persitentIndexDir/hashTableStore.txt
#ulimit -n 500000
#./ycsbc -db leveldb -threads 1 -P $workload_name5 -dbfilename "/home/zq/SSD-e/temp2" -configpath "$configpath" -skipLoad false > UniKV-ycsb-f-50M-v20.txt

#./ycsbc -db rocksdb -threads 1 -P $workload_name1 -dbfilename "/home/zq/SSD-d/temp1" -configpath "$configpath" -skipLoad false > rocksdb-ycsb-b-50M.txt
#./ycsbc -db pebblesdb -threads 1 -P $workload_name5 -dbfilename "/home/zq/SSD-d/temp2" -configpath "$configpath" -skipLoad false > pebblesdb-ycsb-f-50M-v1.txt

#ulimit -n 500000
#./ycsbc -db rocksdb -threads 1 -P $workload_name5 -dbfilename "/home/zq/SSD-d/temp1" -configpath "$configpath" -skipLoad true > rocksdb-ycsb-f-50M--2.txt

#rm -r ../persitentIndexDir/B_TreeStore.txt
#rm -r ../persitentIndexDir/hashTableStore.txt
#./ycsbc -db leveldb -threads 1 -P $workload_name3 -dbfilename "/home/zq/SSD-e/temp1" -configpath "$configpath" -skipLoad false > UniKV-ycsb-d.txt
#ulimit -n 50000
#rm -r ../persitentIndexDir/B_TreeStore.txt
#rm -r ../persitentIndexDir/hashTableStore.txt
#./ycsbc -db leveldb -threads 1 -P $workload_name4 -dbfilename "/home/zq/SSD-e/temp2" -configpath "$configpath" -skipLoad false > UniKV-ycsb-e-1.txt
#ulimit -n 50000
#rm -r ../persitentIndexDir/B_TreeStore.txt
#rm -r ../persitentIndexDir/hashTableStore.txt
#./ycsbc -db leveldb -threads 1 -P $workload_name5 -dbfilename "/home/zq/SSD-e/temp3" -configpath "$configpath" -skipLoad false > UniKV-ycsb-f.txt

#./ycsbc -db hyperleveldb -threads 1 -P $workload_name3 -dbfilename "/home/zq/SSD-d/temp3" -configpath "$configpath" -skipLoad false > hyperleveldb-ycsb-d.txt
#./ycsbc -db pebblesdb -threads 1 -P $workload_name3 -dbfilename "/home/zq/SSD-d/temp1" -configpath "$configpath" -skipLoad false > pebblesdb-load-100G-mix-0.7:0.3-100M-zif=1.2.txt
#./ycsbc -db rocksdb -threads 1 -P $workload_name3 -dbfilename "/home/zq/SSD-d/temp1" -configpath "$configpath" -skipLoad false > rocksdb-load-100G-mix-0.3:0.7-100M-zif=1.2.txt
#./ycsbc -db hyperleveldb -threads 1 -P $workload_name3 -dbfilename "/home/zq/SSD-d/temp1" -configpath "$configpath" -skipLoad false > hyperleveldb-load-100G-mix-0.3:0.7-100M-zif=1.2.txt
#./ycsbc -db hyperleveldb -threads 1 -P $workload_name3 -dbfilename "/home/zq/SSD-d/temp1" -configpath "$configpath" -skipLoad false > hyperleveldb-load-100G-mix-0.5:0.5-100M-zif=1.2-v1.txt
#./ycsbc -db pebblesdb -threads 1 -P $workload_name -dbfilename "/home/zq/SSD-e/temp1" -configpath "$configpath" -skipLoad false > partitionResult-pebblesdb-load-100G-read10M-zif=1.2.txt
#./ycsbc -db pebblesdb -threads 1 -P $workload_name1 -dbfilename "/home/zq/SSD-e/temp1" -configpath "$configpath" -skipLoad true > pebblesdb-load-100G-scan1M-zif=1.2-6-2.txt
#./ycsbc -db pebblesdb -threads 1 -P $workload_name2 -dbfilename "/home/zq/SSD-e/temp1" -configpath "$configpath" -skipLoad true > partitionResult-pebblesdb-load-100G-update-100G-zif=1.2.txt
#./ycsbc -db pebblesdb -threads 1 -P $workload_name -dbfilename "/home/zq/SSD-e/temp1" -configpath "$configpath" -skipLoad true > partitionResult-pebblesdb-load-100G-read10M-zif=1.2-1.txt
#./ycsbc -db pebblesdb -threads 1 -P $workload_name1 -dbfilename "/home/zq/SSD-e/temp1" -configpath "$configpath" -skipLoad true > partitionResult-pebblesdb-load-100G-scan1M-zif=1.2-1.txt

#./ycsbc -db leveldb -threads 1 -P $workload_name -dbfilename "/home/zq/SSD-e/temp1" -configpath "$configpath" -skipLoad false > partitionResult-leveldb-load-update-test.txt
#./ycsbc -db leveldb -threads 1 -P $workload_name -dbfilename "/home/zq/SSD-e/temp1" -configpath "$configpath" -skipLoad false > partitionResult-leveldb-load-100G-update-100G-5:20-30-1.txt
#./ycsbc -db leveldb -threads 1 -P $workload_name -dbfilename "/home/zq/SSD-e/temp1" -configpath "$configpath" -skipLoad false > partitionResult-leveldb-load-100G-update-100G-5:20-40-zif0.9-1.txt
#./ycsbc -db pebblesdb -threads 1 -P $workload_name -dbfilename "/home/zq/SSD-e/temp1" -configpath "$configpath" -skipLoad false > partitionResult-pebblesdb-load-100G-update-100GB.txt
#./ycsbc -db pebblesdb -threads 1 -P $workload_name -dbfilename "/home/zq/SSD-e/temp1" -configpath "$configpath" -skipLoad false > partitionResult-pebblesdb-load-insert-zif.txt
#./ycsbc -db leveldb -threads 1 -P $workload_name1 -dbfilename "/home/zq/SSD-e/temp2" -configpath "$configpath" -skipLoad false > partitionResult-leveldb-load-scan-nb-c-uni.txt
#./ycsbc -db leveldb -threads 1 -P $workload_name2 -dbfilename "/home/zq/SSD-e/temp2" -configpath "$configpath" -skipLoad false > partitionResult-leveldb-load-mix-nb-c-uni-4.txt
#sync; echo 3 > /proc/sys/vm/drop_caches

#./ycsbc -db rocksdb -threads 1 -P $workload_name -dbfilename "/home/zq/SSD-e/temp-rocksdb-b" -configpath "$configpath" -skipLoad false > partitionResult-rocksdb-b.txt
#sync; 
#rm -r loadRecord.txt
#rm -r operateRecord.txt
#echo 3 > /proc/sys/vm/drop_caches
#workload_name=./workloads/glsmworkloadr_test.spec
#./ycsbc -db leveldb -threads 1 -P $workload_name -dbfilename "testLevelDB" -configpath "$configpath" -skipLoad true
}

workload_name=./workloads/workloada.spec
workload_name1=./workloads/workloadb.spec
workload_name2=./workloads/workloadc.spec
workload_name3=./workloads/workloadd.spec
workload_name4=./workloads/workload1.spec
workload_name5=./workloads/workloadf.spec

configpath=./configDir/leveldb_config.ini
#section=basic
#key=hierarchicalBoomflag
#value=true
#./modifyConfig.py "$configpath" "$section" "$key" "$value"
#echo "123"|sudo -S echo 3 > /proc/sys/vm/drop_caches
__runGLSM


