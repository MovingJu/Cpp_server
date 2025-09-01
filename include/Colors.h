#pragma once

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

class Colors {
    public:
        static cv::Mat blue(cv::Mat& img);
        static cv::Mat gray(cv::Mat& img);
};