#pragma once

#include <opencv2/core.hpp>
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
        static Queue<cv::Mat> work_queue;

    public:
        // defines what image processing thread does.
        static void work(crow::websocket::connection* conn);

    public:
        static void create(crow::websocket::connection* conn, const unsigned short);
        static void join();
        static void push_(const cv::Mat& img);
};