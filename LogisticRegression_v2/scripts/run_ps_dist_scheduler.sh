root_path=`pwd`
export DMLC_NUM_WORKER=2
export DMLC_NUM_SERVER=1
export DMLC_PS_ROOT_URI='192.168.153.128'
export DMLC_PS_ROOT_PORT=8001
export DMLC_ROLE='scheduler'
sh ./scripts/start_scheduler.sh  $root_path/build/LR $root_path/newconfig.txt
