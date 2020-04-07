#!/bin/bash
function __runGLSM(){

rm -r ../persitentIndexDir/B_TreeStore.txt
rm -r ../persitentIndexDir/hashTableStore.txt
#rm -r /home/zq/SSD-d/temp1/*
#rm -r /home/zq/SSD-d/temp/*
rm -r /home/zq/temp1/*

./ycsbc -db unikv -threads 1 -P $workload_name -dbfilename "/home/zq/temp1" -configpath "$configpath" -skipLoad false > unikv-load-100G-read-10M-threads=test.txt
./ycsbc -db unikv -threads 1 -P $workload_name1 -dbfilename "/home/zq/temp1" -configpath "$configpath" -skipLoad true > unikv-load-100G-scan-1M-threads=test.txt

#./ycsbc -db leveldb -threads 1 -P $workload_name -dbfilename "/home/zq/SSD-e/temp1" -configpath "$configpath" -skipLoad false > leveldb-load-100G-read-10M.txt
#./ycsbc -db leveldb -threads 1 -P $workload_name1 -dbfilename "/home/zq/SSD-e/temp1" -configpath "$configpath" -skipLoad true > leveldb-load-100G-scan-1M.txt

#./ycsbc -db hyperleveldb -threads 1 -P $workload_name -dbfilename "/home/zq/SSD-e/temp1" -configpath "$configpath" -skipLoad false > hyperleveldb-load-100G-read-10M.txt
#./ycsbc -db hyperleveldb -threads 1 -P $workload_name1 -dbfilename "/home/zq/SSD-e/temp1" -configpath "$configpath" -skipLoad true > hyperleveldb-load-100G-scan-1M.txt

#./ycsbc -db pebblesdb -threads 1 -P $workload_name -dbfilename "/home/zq/SSD-e/temp1" -configpath "$configpath" -skipLoad false > pebblesdb-load-100G-read-10M.txt
#./ycsbc -db pebblesdb -threads 1 -P $workload_name1 -dbfilename "/home/zq/SSD-e/temp1" -configpath "$configpath" -skipLoad true > pebblesdb-load-100G-scan-1M.txt

#./ycsbc -db rocksdb -threads 1 -P $workload_name -dbfilename "/home/zq/SSD-e/temp1" -configpath "$configpath" -skipLoad false > pebblesdb-load-100G-read-10M.txt
#./ycsbc -db rocksdb -threads 1 -P $workload_name1 -dbfilename "/home/zq/SSD-e/temp1" -configpath "$configpath" -skipLoad true > pebblesdb-load-100G-scan-1M.txt

}

workload_name=./workloads/workloadc-1.spec
workload_name1=./workloads/workloadc-2.spec
workload_name2=./workloads/workloadc-3.spec
workload_name3=./workloads/workloadc-4.spec
workload_name4=./workloads/workloadc-5.spec
workload_name5=./workloads/workloadc-6.spec
workload_name6=./workloads/workloadc.spec

configpath=./configDir/leveldb_config.ini
#section=basic
#key=hierarchicalBoomflag
#value=true
#./modifyConfig.py "$configpath" "$section" "$key" "$value"
#echo "123"|sudo -S echo 3 > /proc/sys/vm/drop_caches
__runGLSM