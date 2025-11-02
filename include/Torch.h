#pragma once

#include <torch/script.h>
#include <opencv2/opencv.hpp>
#include <mutex>

class Torch {
private:
    torch::jit::script::Module model;
public:
    Torch(const std::string& path) {
        try {
            model = torch::jit::load(path);
            model.eval();
        } catch (const c10::Error& e) {
            std::cerr << "Error loading model" << '\n';
            throw;
        }
    }

    torch::Tensor preprocess(const cv::Mat& img);
    int process(const cv::Mat& img);
};
