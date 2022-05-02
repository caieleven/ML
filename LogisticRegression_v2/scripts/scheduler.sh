###
 # @Author: Baorun Chen
 # @Date: 2022-05-02 00:19:06
 # @LastEditTime: 2022-05-02 00:36:36
 # @Description: 
### 

export DMLC_PS_ROOT_URI $(ifconfig -a|grep inet|grep -v 127.0.0.1|awk '{print $2}')
echo DMLC_PS_ROOT_URI