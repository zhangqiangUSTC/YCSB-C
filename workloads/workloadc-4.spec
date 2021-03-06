# Yahoo! Cloud System Benchmark
# Workload C: Read only
#   Application example: user profile cache, where profiles are constructed elsewhere (e.g., Hadoop)
#                        
#   Read/update ratio: 100/0
#   Default data size: 1 KB records (10 fields, 100 bytes each, plus key)
#   Request distribution: zipfian
#   100000000

recordcount=400000000
#400000000
#1562500
#3125000
#6250000
#52428800
#210000000
#50000000 

operationcount=40000000

workload=com.yahoo.ycsb.workloads.CoreWorkload

readallfields=true

readproportion=1
updateproportion=0

scanproportion=0
insertproportion=0

fieldlength=232 #232  #65512 #32744  #16360  #232 1000 4072

requestdistribution=zipfian 

maxscanlength=50
#scanlengthdistribution=uniform

#zipfianconst=1.2