###
 # @Author: Baorun Chen
 # @Date: 2022-03-08 13:57:20
 # @LastEditTime: 2022-04-17 22:49:02
 # @Description: 
### 
#!/bin/bash
# set -x
if [ $# -lt 3 ]; then
    echo "usage: $0 num_servers num_workers bin [args..]"
    exit -1;
fi

export DMLC_NUM_SERVER=$1
shift
export DMLC_NUM_WORKER=$1
shift
bin=$1
shift
arg="$@"

# start the scheduler
export DMLC_PS_ROOT_URI='127.0.0.1'
export DMLC_PS_ROOT_PORT=8019
export DMLC_ROLE='scheduler'
${bin} ${arg} &

export DMLC_ROLE='server'

i=0
while [ ${i} -lt ${DMLC_NUM_SERVER} ]; do
    ${bin} ${arg} &
    i=$((i + 1))
done


export DMLC_ROLE='worker'
i=0
while [ ${i} -lt ${DMLC_NUM_WORKER} ]; do
    ${bin} ${arg} &
    i=$((i + 1))
done


wait

