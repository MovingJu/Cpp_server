#pragma once

#include <condition_variable>
#include <mutex>
#include <tuple>
#include <string>
#include <crow.h>
#include <Queue.h>

class Send_queue{
    private:
        static bool conn_valid;
        static std::condition_variable conditional_var;
        static std::mutex send_queue_mutex;
        static Queue<std::tuple<crow::websocket::connection*, crow::json::wvalue, const std::string>> send_queue;
    public:
        static void push_(crow::websocket::connection* conn, const std::string& msg, const std::string& img_binary);
        static std::tuple<crow::websocket::connection*, crow::json::wvalue, const std::string> pop_();
        static void empty_();
    public:
        static bool is_empty();
        static bool is_conn_valid();
        static void set_conn_valid(bool setter);
};