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

class Sending_thread {
    private:
        static std::condition_variable conditional_var;
        static std::mutex send_queue_mutex;
        static Queue<Send_tuple> send_queue;
    public:
        static void work();
        static void push_(crow::websocket::connection* conn, const std::string& msg, const std::string& img_binary);
        static Send_tuple pop_();
        static void empty_();
    public:
        static bool is_empty();
};