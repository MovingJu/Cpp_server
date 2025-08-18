#include "crow.h"
#include "../include/__init__.h"

int main() {
    crow::App<CORS> app;

    CROW_ROUTE(app, "/test").methods(crow::HTTPMethod::OPTIONS)
    ([](crow::request&, crow::response& res){
        res.code = 202;
        res.end();
    });

    CROW_ROUTE(app, "/test")
    ([](const crow::request&, crow::response& res){
        res.body = "Hellow";
        res.end();
        return;
    });

    CROW_ROUTE(app, "/upload").methods(crow::HTTPMethod::OPTIONS)
    ([&](crow::response& res) {
        return Upload::upload(res);
    });

    CROW_ROUTE(app, "/upload").methods(crow::HTTPMethod::POST)
    ([&](const crow::request& req, crow::response& res) {
        return Upload::upload(req, res);
    });

    CROW_ROUTE(app, "/ws")
    .websocket(&app)
    .onopen([](crow::websocket::connection& conn){
        std::cout << "WebSocket Connected!" << '\n';
    })
    .onclose([](crow::websocket::connection& conn, const std::string& msg, unsigned short code){
        std::cout << code << ", WebSocket Closed : " << msg << '\n';
    })
    .onmessage([]
        (crow::websocket::connection& conn, const std::string& data, bool is_binary){
        return Upload::upload(conn, data, is_binary);
    });

    app.port(8000).multithreaded().run();
}
