#include <thread>
#include <atomic>
#include <crow.h>
#include <__init__.h>
#include <Send_queue.h>
#include <sending_thread.h>

int main()
{
    crow::App<CORS> app;
    std::thread send_thread;
    extern std::atomic<bool> running;
    running = false;

    CROW_ROUTE(app, "/ws")
        .websocket(&app)
        .onopen([&](crow::websocket::connection &conn)
                {
        running = true;
        send_thread = std::thread(&sending_thread);

        Send_queue::set_conn_valid(running);
        std::cout << "WebSocket Connected!" << '\n'; })
        .onclose([&](crow::websocket::connection &conn, const std::string &msg, unsigned short code)
                 {
        running = false;
        Send_queue::set_conn_valid(running);
        Send_queue::empty_();

        send_thread.join();
        std::cout << code << ", WebSocket Closed : " << msg << '\n'; })
        .onmessage([](crow::websocket::connection &conn, const std::string &data, bool is_binary)
                   { return Upload::upload_(conn, data, is_binary); });

    CROW_ROUTE(app, "/test")([](){
        return "Testing! the server is on running!";
    });

    app.port(8000)
        //    .multithreaded()
        .run();

    if (running)
    {
        running = false;
        Send_queue::set_conn_valid(false);
        Send_queue::empty_();
        send_thread.join();
    }

    return 0;
}
