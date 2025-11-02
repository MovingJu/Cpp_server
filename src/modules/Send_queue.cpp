#include <Send_queue.h>

typedef std::tuple<crow::websocket::connection*, std::string, const std::string> Send_tuple;

std::mutex Send_queue::send_queue_mutex;
Queue<Send_tuple> Send_queue::send_queue;
std::condition_variable Send_queue::conditional_var;

void Send_queue::push_(crow::websocket::connection* conn, const std::string& msg, const std::string& img_binary){

    auto send_target = std::make_tuple(conn, msg, img_binary);
    {
        std::unique_lock<std::mutex> lock(send_queue_mutex);
        send_queue.push_(send_target);
    }
    conditional_var.notify_one();
}
Send_tuple Send_queue::pop_(){

    std::unique_lock<std::mutex> lock(send_queue_mutex);
    conditional_var.wait(lock, [](){ return send_queue.get_length() > 0; });

    auto result = send_queue.pop_();
    
    return result;
}
void Send_queue::empty_(){
    std::unique_lock<std::mutex> lock(send_queue_mutex);
    send_queue.empty_();
}

bool Send_queue::is_empty(){
    std::unique_lock<std::mutex> lock(send_queue_mutex);
    bool result = (send_queue.get_length() == 0);
    lock.unlock();
    return result;
}
