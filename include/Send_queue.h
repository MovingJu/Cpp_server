#pragma once

#include <condition_variable>
#include <mutex>
#include <tuple>
#include <unordered_map>
#include <string>
#include <crow.h>
#include <Queue.h>

typedef std::tuple<crow::websocket::connection*, std::string, const std::string> Send_tuple;

class Send_queue {
    private:
        static std::unordered_map<crow::websocket::connection*, bool> validity_table;
        static std::condition_variable conditional_var;
        static std::mutex send_queue_mutex;
        static Queue<Send_tuple> send_queue;
    public:
        static void push_(crow::websocket::connection* conn, const std::string& msg, const std::string& img_binary);
        static Send_tuple pop_();
        static void empty_();
    public:
        static bool is_empty();
};