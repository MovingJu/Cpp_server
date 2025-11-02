#pragma once

#include <opencv2/core.hpp>
#include <mutex>
#include <thread>
#include <crow.h>
#include "Queue.h"

extern std::atomic<bool> running;

/**
 * @brief Make queue to define process thread's work.
 * 
 * @example Image_threads.create(conn, 3);
 * @example Image_threads.push_(img);
 * @example Image_threads.join()
 */
class Image_threads {
    private:
        static unsigned short num_thread;
        static std::thread* thread_pool;
        static Queue<std::tuple<crow::websocket::connection*, cv::Mat>> work_queue;
        static std::mutex exclude;

    public:
        // defines what image processing thread does.
        static void work();

    public:
        static void create(const unsigned short num_threads);
        static void join();
        static void push_(crow::websocket::connection* conn, const cv::Mat& img);
};