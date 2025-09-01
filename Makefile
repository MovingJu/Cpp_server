port?=8000

platform=linux/arm64,linux/amd64#,linux/arm/v7
docker_img_tag=cpp_server

.PHONY: build run test
docker-build:
	docker buildx build \
		--platform  $(platform) \
		-t movingju/test:$(docker_img_tag) \
		.

docker-test:
	docker build \
	-t movingju/test:$(docker_img_tag) \
	.

docker-run:
	docker run -p $(port):8000 movingju/test:$(docker_img_tag)

docker-push:
	docker push movingju/test:$(docker_img_tag)

build:
	cmake . -B build
	cmake --build build

run: 
	./build/src/e

test: 
	./build/test/t