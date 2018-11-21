FROM ubuntu:18.10

WORKDIR /root

# install depedencies
RUN apt update && \
  apt install -y \
    git\
    cmake\
    g++

# build the program
RUN git clone https://github.com/Turanic/HNStat &&\
  cd HNStat &&\
  ./bootstrap.sh &&\
  cp out/hnStat /root

ADD hn_logs.tsv.tar.bz2 /root/

# launch it
ENTRYPOINT [ "./hnStat", "top", "3", "hn_logs.tsv" ]
