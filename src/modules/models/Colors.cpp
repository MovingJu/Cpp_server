#include <Colors.h>

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

cv::Mat Colors::blue(cv::Mat& img){
    cv::Mat out;
    cv::cvtColor(img, out, cv::COLOR_BGR2RGBA);
    return out;
}

cv::Mat Colors::gray(cv::Mat& img){
    cv::Mat gray, out;
    cv::cvtColor(img, gray, cv::COLOR_RGBA2GRAY);
    cv::cvtColor(gray, out, cv::COLOR_GRAY2RGBA);
    return out;
}