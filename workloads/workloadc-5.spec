# Yahoo! Cloud System Benchmark
# Workload C: Read only
#   Application example: user profile cache, where profiles are constructed elsewhere (e.g., Hadoop)
#                        
#   Read/update ratio: 100/0
#   Default data size: 1 KB records (10 fields, 100 bytes each, plus key)
#   Request distribution: zipfian
#   100000000

recordcount=100000000
#400000000
#1562500
#3125000
#6250000
#52428800
#210000000
#50000000 

operationcount=1000000

workload=com.yahoo.ycsb.workloads.CoreWorkload

readallfields=true

readproportion=0
updateproportion=0

scanproportion=1
insertproportion=0

fieldlength=1000 

requestdistribution=zipfian 

maxscanlength=50
#scanlengthdistribution=uniform

#zipfianconst=1.2

