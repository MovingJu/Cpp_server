e?=e
version?=20
port?=8000
files?=

extern=./extern
add= -std=c++$(version) -o $(e) -lpthread -I$(extern) `pkg-config --cflags --libs opencv4`
docker_img_name=berry_cpp

d_build:
	docker buildx build \
		--platform linux/arm64,linux/amd64,linux/arm/v7 \
		-t movingju/test:cpp_server \
		--push .

d_run:
	docker run -p $(port):8000 movingju/test:cpp_server

build:
	g++ main.cpp $(files) $(add) && strip e

run:
	make build
	./$(e)
	