FROM debian:bullseye-slim

RUN apt-get update && \
    apt-get install -y --no-install-recommends \
        libopencv-dev \
        g++ \
        build-essential \
        cmake

WORKDIR /app

COPY ./extern ./extern
COPY ./include ./include
COPY ./src ./src
COPY ./CMakeLists.txt .

RUN cmake . -B build

RUN cmake --build build

CMD ["./build/e"]