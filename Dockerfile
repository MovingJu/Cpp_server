FROM debian:bullseye-slim

WORKDIR /app

RUN apt-get update && \
    apt-get install -y --no-install-recommends \
        libopencv-dev \
        g++ \
        build-essential \
        cmake
RUN apt-get install -y --no-install-recommends \
        wget \
        unzip
RUN wget --no-check-certificate \
    https://download.pytorch.org/libtorch/cpu/libtorch-shared-with-deps-2.9.0%2Bcpu.zip
        
RUN unzip libtorch-shared-with-deps-2.9.0+cpu.zip

COPY ./extern ./extern
COPY ./models ./models
COPY ./include ./include
COPY ./src ./src
COPY ./CMakeLists.txt .

RUN cmake . -B build

RUN cmake --build build

CMD ["./build/e"]