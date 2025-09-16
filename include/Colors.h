#pragma once

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

namespace Colors {
    cv::Mat blue(cv::Mat& img);
    cv::Mat gray(cv::Mat& img);
};