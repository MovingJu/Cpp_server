FROM debian:bullseye-slim

WORKDIR /app

RUN apt-get update && \
    apt-get install -y --no-install-recommends \
        libopencv-dev \
        g++ \
        build-essential \
        cmake

RUN wget https://download.pytorch.org/libtorch/cpu/libtorch-shared-with-deps-2.9.0%2Bcpu.zip
RUN unzip ./libtorch

COPY ./extern ./extern
COPY ./include ./include
COPY ./src ./src
COPY ./CMakeLists.txt .

RUN cmake . -B build

RUN cmake --build build

CMD ["./build/e"]