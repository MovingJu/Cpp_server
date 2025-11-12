# Cpp_server

This is repository that have code for [on-vacation-project](https://github.com/MovingJu/On-vacation-project)'s backend.

# Where can I test or download binary?

Come visit my website : [On vacation project](https://onvacation.movingju.com)

# Dependency

- OpenCV : This is ONLY library my project depends on.

I couldn't find out how to become independent from OpenCV, while doing inference with .onnx and tranforming images.

Luckily, I found way to statically link OpenCV.

# How to download and compile

First, pull this repository and update submodules.
```bash
git clone https://github.com/MovingJu/Cpp_server
cd Cpp_server
git submodule update --init --recursive
```

Second, compile this project.
```bash
source ./project.sh
sync
build
```

> Note: This repository doesn't include yolov5s.onnx model, because of its size.
> If you want to fully test this repo, please download yolov5s.onnx model and move it to models/ folder.