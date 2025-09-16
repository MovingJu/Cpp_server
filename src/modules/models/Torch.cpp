#include <Torch.h>

#include <mutex>
#include <torch/script.h>
#include <opencv2/opencv.hpp>
#include <iostream>

torch::Tensor Torch::preprocess(const cv::Mat& img) {
    std::unique_lock<std::mutex> lock(exclude);

    // RGBA -> RGB
    cv::Mat rgb;
    cv::cvtColor(img, rgb, cv::COLOR_RGBA2RGB);

    // 리사이즈 및 float 변환
    cv::Mat resized;
    cv::resize(rgb, resized, cv::Size(128, 128));
    resized.convertTo(resized, CV_32F, 1.0 / 255.0);

    // HWC -> CHW
    std::vector<int64_t> dims = {1, 3, resized.rows, resized.cols};
    torch::Tensor tensor_image = torch::from_blob(resized.data, dims, torch::kFloat);

    return tensor_image.clone();
}

int Torch::process(const cv::Mat& img) {
    std::unique_lock<std::mutex> lock(exclude);
    torch::Tensor input = preprocess(img);
    model.eval();

    torch::Tensor output = model.forward({input}).toTensor();
    auto pred = output.argmax(1);
    return pred.item<int>();
}
