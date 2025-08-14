e?=e
version?=20
dir?=~/Documents/cpp
files?=

extern=extern
add= -std=c++$(version) -o $(e) -lpthread -I$(extern) -Wno-psabi
docker_img_name=berry_cpp

berry_setup:
	docker build -t $(docker_img_name) .

berry:
	docker run --rm \
	-v $(PWD):/src -w /src $(docker_img_name) \
	sh -c "g++ main.cpp $(files) $(add) -static && strip e"

zip:
	zip -r archive.zip static templates

move:
	scp -P $$berry_port ./$(e) galesky@$$berry_ip:$(dir)

build:
	g++ main.cpp $(files) $(add)

run:
	make build
	./$(e)
	