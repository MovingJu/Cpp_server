port?=8000

platform=linux/arm64,linux/amd64#,linux/arm/v7
docker_img_tag=cpp_server

.PHONY: build run
build:
	docker buildx build \
		--platform  $(platform) \
		-t movingju/test:$(docker_img_tag) \
		--push .

test:
	docker build \
	-t movingju/test:$(docker_img_tag)\
	.

run:
	docker run -p $(port):8000 movingju/test:$(docker_img_tag)
