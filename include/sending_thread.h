#pragma once

#include <condition_variable>
#include <mutex>
#include <tuple>
#include <unordered_map>
#include <string>
#include <crow.h>
#include <Queue.h>

typedef std::tuple<crow::websocket::connection*, std::string, const std::string> Send_tuple;
extern std::atomic<bool> running;

namespace Sending_thread {
    extern std::unordered_map<crow::websocket::connection*, bool> valid_conn;
    extern std::condition_variable conditional_var;
    extern std::mutex send_queue_mutex;
    extern Queue<Send_tuple> send_queue;

    void work();
    void push_(crow::websocket::connection* conn, const std::string& msg, const std::string& img_binary);
    Send_tuple pop_();
    void empty_();
    
    bool is_empty();
};