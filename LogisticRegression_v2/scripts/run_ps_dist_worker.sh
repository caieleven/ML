root_path=`pwd`
model_name=0
epochs=100
export DMLC_NUM_WORKER=2
export DMLC_NUM_SERVER=1
export DMLC_PS_ROOT_URI='192.168.153.128'
export DMLC_PS_ROOT_PORT=8001
export DMLC_ROLE='worker'
sh ./scripts/start_worker.sh $root_path/build/LR $root_path/newconfig.txt
