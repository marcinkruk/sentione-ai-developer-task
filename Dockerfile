FROM oraclelinux:8 as polem-ol8

RUN dnf upgrade && \
    dnf config-manager --set-enabled ol8_codeready_builder && \
    dnf install -y oracle-epel-release-el8

RUN dnf upgrade && \
    dnf install -y \
        wget \
        tar \
        bzip2 \
        make \
        binutils \
        glibc-devel \
        git \
        gcc \
        gcc-c++ \
        cmake \
        boost-devel \
        libxml2-devel \
        glib2-devel \
        perl-XML-Parser \
        perl-open \
        libxml++ \
        antlr-C++ \
        antlr-tool \
        java-1.8.0-openjdk-devel \
        python3-devel

RUN mkdir /build
WORKDIR /build

RUN wget http://mirror.centos.org/centos/8-stream/PowerTools/x86_64/os/Packages/libsigc++20-devel-2.10.0-6.el8.x86_64.rpm && \
    rpm -i libsigc++20-devel-2.10.0-6.el8.x86_64.rpm
RUN wget http://mirror.centos.org/centos/8-stream/PowerTools/x86_64/os/Packages/glibmm24-devel-2.56.0-2.el8.x86_64.rpm && \
    rpm -i glibmm24-devel-2.56.0-2.el8.x86_64.rpm
RUN dnf install -y libxml++-devel

# loki
RUN wget https://ftp.lysator.liu.se/pub/opensuse/distribution/leap/15.3/repo/oss/x86_64/libloki0_1_7-0.1.7-bp153.1.15.x86_64.rpm && \
    rpm -i libloki0_1_7-0.1.7-bp153.1.15.x86_64.rpm
RUN wget https://ftp.lysator.liu.se/pub/opensuse/distribution/leap/15.3/repo/oss/x86_64/libloki-devel-0.1.7-bp153.1.15.x86_64.rpm && \
    rpm -i libloki-devel-0.1.7-bp153.1.15.x86_64.rpm

# corpus2
RUN git clone http://nlp.pwr.wroc.pl/corpus2.git && \
    mkdir /build/corpus2/build
WORKDIR /build/corpus2/build
RUN cmake .. && make && make install && ldconfig

# wccl
WORKDIR /build
RUN git clone http://nlp.pwr.edu.pl/wccl.git && \
    mkdir /build/wccl/bin
WORKDIR /build/wccl
RUN sh -c 'grep -rl antlr-pic | xargs -e sed -i s/antlr-pic/antlr/g'
RUN sh -c 'grep -rl runantlr | xargs -e sed -i s/runantlr/antlr/g'
WORKDIR /build/wccl/bin
RUN cmake .. && make && make install && ldconfig

# morfeusz2
WORKDIR /build
RUN pip3 install pyparsing
RUN wget http://mirror.centos.org/centos/7/os/x86_64/Packages/swig-2.0.10-5.el7.x86_64.rpm && \
    rpm -i swig-2.0.10-5.el7.x86_64.rpm
RUN wget http://download.sgjp.pl/morfeusz/20220213/morfeusz-src-20220213.tar.gz && \
    tar xf morfeusz-src-20220213.tar.gz && \
    mkdir /build/Morfeusz/bin
WORKDIR /build/Morfeusz
RUN sed -i 's/add_subdirectory (gui)//g' CMakeLists.txt
RUN sed -i 's/add_subdirectory (wrappers)//g' morfeusz/CMakeLists.txt
WORKDIR /build/Morfeusz/bin
RUN cmake .. -D SKIP_TESTING=1 && make && make install

# polem
WORKDIR /build
RUN git clone https://github.com/CLARIN-PL/Polem.git && \
    mkdir /build/Polem/bin
WORKDIR /build/Polem/bin
RUN cmake .. && make && make install

# 
FROM polem-ol8 as sentione-ol8 

RUN dnf update -y

# nlohmann/JSON
WORKDIR /build
RUN git clone https://github.com/nlohmann/json.git
RUN cp -r /build/json/include/nlohmann/ /usr/include/

# flask
RUN pip3 install flask

# my code
RUN mkdir /build/sentione
WORKDIR /build/sentione
COPY polem_enrich/ polem_enrich/
COPY rest-api/ rest-api/

WORKDIR /build/sentione/polem_enrich
RUN make && make move_files

WORKDIR /build/sentione/rest-api
CMD ["python3", "app.py"]

