#include <thread>

#include <crow.h>
#include <__init__.h>

#include <main.h>

std::atomic<bool> running = true;

int main() {    
    crow::App<CORS> app;

    std::thread send_thread(sending_thread);

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
        return Upload::upload_(conn, data, is_binary);
    });

    app.port(8000).multithreaded().run();

    running = false;
    send_thread.join();
    std::cout << "Sending thread joined!" << std::endl;

    return 0;
}
