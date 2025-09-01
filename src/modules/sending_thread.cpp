#include <sending_thread.h>
#include <crow.h>
#include <Send_queue.h>
#include <chrono>
#include <thread>
#include <atomic>

std::atomic<bool> running;

void sending_thread() {
    using namespace std::chrono_literals;

    while (running) {
        if (Send_queue::is_empty()) {
            std::this_thread::sleep_for(20ms);
            continue;
        }

        auto item = Send_queue::pop_();

        crow::websocket::connection* conn = std::get<0>(item);
        if (!conn || !(Send_queue::is_conn_valid())){
            continue;
        }
        crow::json::wvalue header = std::get<1>(item);
        std::string img_binary = std::get<2>(item);
        
        conn->send_text(header.dump());
        conn->send_binary(img_binary);
    }

    std::cout << "Sending thread exiting gracefully." << std::endl;
}
