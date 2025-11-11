#include <Sending_thread.h>

std::atomic<bool> running;
typedef std::tuple<crow::websocket::connection*, std::string, const std::string> Send_tuple;

std::unordered_map<crow::websocket::connection*, bool> Sending_thread::valid_conn;
std::condition_variable Sending_thread::conditional_var;
std::mutex Sending_thread::send_queue_mutex;
Queue<Send_tuple> Sending_thread::send_queue;

void Sending_thread::work(){
    while (running) {
        if (Sending_thread::is_empty()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
            continue;
        }

        auto item = Sending_thread::pop_();

        crow::websocket::connection* conn = std::get<0>(item);
        if ((Sending_thread::valid_conn.find(conn) == Sending_thread::valid_conn.end()) 
            || !Sending_thread::valid_conn[conn]){
            continue;
        }
        if (!conn){
            continue;
        }
        std::string header = std::get<1>(item);
        std::string img_binary = std::get<2>(item);
        
        conn->send_text(header);
        conn->send_binary(img_binary);
    }

    std::cout << "Sending thread exiting gracefully." << std::endl;
}

void Sending_thread::push_(crow::websocket::connection* conn, const std::string& msg, const std::string& img_binary){

    auto send_target = std::make_tuple(conn, msg, img_binary);
    {
        std::unique_lock<std::mutex> lock(Sending_thread::send_queue_mutex);
        Sending_thread::send_queue.push_(send_target);
    }
    Sending_thread::conditional_var.notify_one();
}
Send_tuple Sending_thread::pop_(){

    std::unique_lock<std::mutex> lock(Sending_thread::send_queue_mutex);
    Sending_thread::conditional_var.wait(lock, [](){ return Sending_thread::send_queue.get_length() > 0; });

    auto result = Sending_thread::send_queue.pop_();
    
    return result;
}
void Sending_thread::empty_(){
    std::unique_lock<std::mutex> lock(Sending_thread::send_queue_mutex);
    Sending_thread::send_queue.empty_();
}

bool Sending_thread::is_empty(){
    std::unique_lock<std::mutex> lock(Sending_thread::send_queue_mutex);
    bool result = (Sending_thread::send_queue.get_length() == 0);
    lock.unlock();
    return result;
}
