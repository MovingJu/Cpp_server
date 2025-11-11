#!/bin/sh

: "${platform:=linux/arm64}"
: "${docker_img_tag:=cpp_serer}"
: "${port:=8000}"

docker-build() {
	docker buildx build \
		--platform $platform \
		-t movingju/test:$docker_img_tag \
		.
}

docker-run() {
	docker run \
		-p $port:8000 \
		movingju/test:$docker_img_tag
}

docker-push(){
	docker push \
		movingju/test:$docker_img_tag
}

release(){
	cmake . -B build -DCMAKE_BUILD_TYPE=release
	cmake --build build
}

build(){
	cmake . -B build
	cmake --build build
}

run() {
	./build/e
}

: "${test_var:=testing}"
test(){
	echo "$test_var"
}