# 为了方便，代码没有从头开始构建，只从本地复制
# FROM ubuntu:20.04
# RUN RUN sed -i s@/archive.ubuntu.com/@/mirrors.aliyun.com/@g /etc/apt/sources.list \
#     && sed -i s@/security.ubuntu.com/@/mirrors.aliyun.com/@g /etc/apt/sources.list \
#     && apt update \
#     && export DEBIAN_FRONTEND=noninteractive \
#     && apt install -y build-essential cmake ssh net-tools vim wget 

FROM nodes:v2
COPY ./LogisticRegression_v2/build/LogisticRegression ./LogisticRegression_v2/scripts/local.sh \
    ./LogisticRegression_v2/ScriptsAndDate/config.txt ./DataSet/newhd.txt \
    ~/code/
CMD ["sh", "-c", "service ssh start; bash"]
