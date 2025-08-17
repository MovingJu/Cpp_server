#include "crow.h"
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <vector>

struct CORS {
    struct context {};
    void before_handle(crow::request&, crow::response& res, context&) {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Content-Type");
    }
    void after_handle(crow::request&, crow::response&, context&){}
};


int main() {
    crow::App<CORS> app;

    CROW_ROUTE(app, "/test").methods(crow::HTTPMethod::OPTIONS)([](
        crow::request&, crow::response& res){
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Content-Type");
        res.code = 202;
        res.end();
    });

    CROW_ROUTE(app, "/test")([](
        const crow::request&, crow::response& res){
        res.body = "Hellow";
        res.end();
        return;
    });

    CROW_ROUTE(app, "/upload").methods(crow::HTTPMethod::OPTIONS)([](
        const crow::request&, crow::response& res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Content-Type");
        res.code = 200;
        res.end();
        return;
    });

    CROW_ROUTE(app, "/upload").methods(crow::HTTPMethod::POST)
    ([&](const crow::request& req, crow::response& res) {
        res.set_header("Content-Type", "text/plain");
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS");
        res.set_header("Access-Control-Allow-Headers", "Content-Type");
        
        const char* w = req.url_params.get("width");
        const char* h = req.url_params.get("height");
        const int width = std::stoi(w), height = std::stoi(h);
        std::vector<uchar> buffer(req.body.begin(), req.body.end());
        
        cv::Mat img(height, width, CV_8UC4, (void*)buffer.data());
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
    });

    app.port(8000).multithreaded().run();
}
