#include <Send_queue.h>

std::mutex Send_queue::send_queue_mutex;
Queue<std::tuple<crow::websocket::connection*, crow::json::wvalue, const std::string>> Send_queue::send_queue;
std::condition_variable Send_queue::conditional_var;

void Send_queue::push_(crow::websocket::connection* conn, const std::string& msg, const std::string& img_binary){
    crow::json::wvalue header;
    header["image_type"] = msg;

    auto send_target = std::make_tuple(conn, header, img_binary);
    {
        std::lock_guard<std::mutex> lock(send_queue_mutex);
        send_queue.push_(send_target);
    }
    conditional_var.notify_one();
}
std::tuple<crow::websocket::connection*, crow::json::wvalue, const std::string> Send_queue::pop_(){
    std::cout << "Sending image" << '\n';

    std::unique_lock<std::mutex> lock(send_queue_mutex);
    conditional_var.wait(lock, [](){ return send_queue.get_length() > 0; });

    auto result = send_queue.pop_();
    
    return result;
}

bool Send_queue::is_empty(){
    std::lock_guard<std::mutex> lock(send_queue_mutex);
    bool result = (send_queue.get_length() == 0);
    
    return result;
}