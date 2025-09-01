#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <vector>
#include <crow.h>

#include <Upload.h>
#include <Send_queue.h>

void Upload::upload_(crow::websocket::connection& conn, const std::string& data, bool is_binary){
    if (!is_binary) {return;}
    const int width = 178, height = 218;
    if (data.size() != width * height * 4) {
        std::cerr << "Invalid data size: " << data.size() << std::endl;
        return;
    }
    
    cv::Mat img(height, width, CV_8UC4, (void*)data.data());

    cv::Mat gray, out;
    cv::cvtColor(img, gray, cv::COLOR_RGBA2GRAY);
    cv::cvtColor(gray, out, cv::COLOR_GRAY2RGBA);

    Send_queue::push_(&conn, "image_gray", std::string((char*)out.data, out.total() * out.elemSize()));    

    return;
}