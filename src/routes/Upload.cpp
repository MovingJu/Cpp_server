#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <vector>
#include "crow.h"

#include "../../include/Upload.h"

void Upload::upload(crow::response& res){
    res.code = 200;
    res.end();
    return;
}

void Upload::upload(const crow::request& req, crow::response& res){
    const char* w = req.url_params.get("width");
    const char* h = req.url_params.get("height");
    const int width = std::stoi(w), height = std::stoi(h);
    std::vector<uchar> buffer(req.body.begin(), req.body.end());
    
    cv::Mat img(height, width, CV_8UC4, (void*)buffer.data());
    
    // // RGBA to BGR(CV default)
    // cv::cvtColor(img, img, cv::COLOR_RGBA2BGR);

    if (img.empty()){
        res.code = 400;
        res.body = "Invalid Image";
        res.end();
        return;
    }

    std::vector<uchar> buf;
    cv::imencode(".jpg", img, buf);
    res.body.assign(buf.begin(), buf.end());
    
    res.end();
    return;
}

void Upload::upload(crow::websocket::connection& conn, const std::string& data, bool is_binary){
    if (!is_binary) {return;}

    const int width = 160, height = 120;

     if (data.size() != width * height * 4) {
        std::cerr << "Invalid data size: " << data.size() << std::endl;
        return;
    }
    
    cv::Mat img(height, width, CV_8UC4, (void*)data.data());

    cv::Mat gray, out;
    cv::cvtColor(img, gray, cv::COLOR_RGBA2GRAY);
    cv::cvtColor(gray, out, cv::COLOR_GRAY2RGBA);

    conn.send_binary(std::string((char*)out.data, out.total() * out.elemSize()));

    return;
}