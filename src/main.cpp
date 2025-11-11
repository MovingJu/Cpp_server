#include <thread>
#include <atomic>
#include <crow.h>
#include <CORS.h>
#include <Sending_thread.h>
#include <Upload.h>
#include <Image_threads.h>

int main() {
    crow::App<CORS> app;
    
    // Initializing
    std::thread send_thread;
    extern std::atomic<bool> running;
    running = true;
    send_thread = std::thread(&Sending_thread::work);
    Image_threads::create(3);

    CROW_ROUTE(app, "/ws")
        .websocket(&app)
        .onopen([&](crow::websocket::connection &conn){
            Sending_thread::valid_conn.insert({&conn, true});
            std::cout << "WebSocket Connected!" << '\n';
        })

        .onclose([&](crow::websocket::connection &conn, const std::string &msg, unsigned short code){
            Sending_thread::valid_conn.erase(&conn);
            // Sending_thread::empty_();

            std::cout << code << ", WebSocket Closed : " << msg << '\n'; 
        })

        .onmessage([](crow::websocket::connection &conn, const std::string &data, bool is_binary){ 
            return Upload::upload_(conn, data, is_binary); 
        });

    CROW_ROUTE(app, "/test")([](){
        return "Testing! the server is on running!";
    });

    app.port(8000)
        //    .multithreaded()
        .run();

    // Closing.
    running = false;
    Image_threads::join();
    Sending_thread::empty_();
    send_thread.join();

    return 0;
}
