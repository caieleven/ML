FROM ubuntu:20.04
RUN sed -i s@/archive.ubuntu.com/@/mirrors.aliyun.com/@g /etc/apt/sources.list \
    && sed -i s@/security.ubuntu.com/@/mirrors.aliyun.com/@g /etc/apt/sources.list \
    && apt update \
    && export DEBIAN_FRONTEND=noninteractive \
    && apt install -y build-essential cmake ssh net-tools vim wget git python3-pip
    # && pip config set global.index-url http://mirrors.aliyun.com/pypi/simple \
    # && pip config set install.trusted-host mirrors.aliyun.com
# download code
WORKDIR /code
RUN git clone https://github.com/caieleven/ML.git \
    && cd ML \
    && pip install -r requirement.txt -i http://mirrors.aliyun.com/pypi/simple \
    && rm -rf ps-lite .* \
    && git clone https://github.com/caieleven/deps.git \
    && cd LogisticRegression_v2 \
    && mkdir build \
    && cmake .. && make \
# 数据生成
# TODO
# 默认config存储位置以及数据大小
CMD ["./ML/LogisticRegression_v2/scripts/configure.config", "1"]
ENTRYPOINT [ "/bin/bash", "./ML/LogisticRegression_v2/build/LogisticRegression"]


