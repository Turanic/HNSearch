FROM ubuntu:18.10

WORKDIR /root

# install depedencies
RUN apt update && \
  apt install -y \
    git\
    cmake\
    g++\
    wget

# build the program
RUN git clone https://github.com/Turanic/HNStat &&\
  cd HNStat &&\
  ./bootstrap.sh &&\
  cp out/hnStat /root

ADD entrypoint.sh .

# launch it
ENTRYPOINT [ "./entrypoint.sh" ]
