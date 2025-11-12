#!/bin/sh

: "${platform:=linux/arm64}"
: "${docker_img_tag:=cpp_serer}"
: "${port:=8000}"
: "${docker_repo:=public}"

docker-build() {
	docker buildx build \
		--platform $platform \
		-t movingju/$docker_repo:$docker_img_tag \
		.
}

docker-run() {
	docker run \
		-p $port:8000 \
		movingju/$docker_repo:$docker_img_tag
}

docker-push(){
	docker push \
		movingju/$docker_repo:$docker_img_tag
}

release(){
	cmake . -B build -DCMAKE_BUILD_TYPE=release
	cmake --build build
}

sync() {
	mkdir build_opencv
	cd build_opencv
	cmake ../opencv \
		-DCMAKE_BUILD_TYPE=Release \
		-DBUILD_WITH_STATIC_CRT=ON \
		-DBUILD_opencv_python3=OFF \
		-DBUILD_opencv_python_bindings_generator=OFF \
		-DBUILD_opencv_python_$docker_repos=OFF \
		-DBUILD_SHARED_LIBS=OFF \       # 정적 라이브러리
		-DWITH_CUDA=OFF \               # GPU 제외
		-DWITH_OPENCL=OFF \             # OpenCL 제외
		-DWITH_IPP=OFF \                # IPP 제외
		-DWITH_TBB=ON \                 # 필요 시 TBB 활성화
		-DWITH_OPENMP=ON \              # CPU 병렬화
		-DWITH_DIRECTX=OFF \
		-DWITH_OPENGL=OFF \
		-DENABLE_NEON=ON \
		-DBUILD_$docker_repoS=OFF \
		-DBUILD_PERF_$docker_repoS=OFF \
		-DBUILD_OPENEXR=OFF \
		-DWITH_OPENEXR=OFF \
		-DBUILD_EXAMPLES=OFF
	make -j4
	make install DESTDIR=./install_opencv
	cd ..
}


build(){
	cmake . -B build
	cmake --build build
}

run() {
	./build/e
}
