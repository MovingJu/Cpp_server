#include <Image_threads.h>

#include <opencv2/core.hpp>
#include <chrono>
#include <thread>
#include <atomic>
#include <mutex>
#include <Sending_thread.h>
#include <crow.h>
#include <tuple>

#include <Queue.h>
#include <Colors.h>
#include <Torch.h>

extern std::atomic<bool> running;
unsigned short Image_threads::num_thread = 0;
std::thread* Image_threads::thread_pool;
Queue<std::tuple<crow::websocket::connection*, cv::Mat>> Image_threads::work_queue;
std::mutex Image_threads::exclude;

void Image_threads::work(){
    Torch hand_model{"./models/hand_model_c.pt"};
    Torch finger_model{"./models/hand_model_c.pt"};
    while (running) {
        std::unique_lock<std::mutex> lock(exclude);
        if (work_queue.get_length() == 0) {
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
            continue;
        }
        auto item = work_queue.pop_();
        lock.unlock();

        crow::websocket::connection* conn = std::get<0>(item);
        cv::Mat img = std::get<1>(item);

        cv::Mat gray = Colors::gray(img).clone();
        cv::Mat blue = Colors::blue(img).clone();

        const int hand = hand_model.process(img);
        const int finger = finger_model.process(img);

        Sending_thread::push_(conn, "image_gray", std::string((char*)gray.data, gray.total() * gray.elemSize()));
        Sending_thread::push_(conn, "image_blue", std::string((char*)blue.data, blue.total() * blue.elemSize()));
        Sending_thread::push_(conn, "string_hand", std::to_string(hand));
        Sending_thread::push_(conn, "string_finger", std::to_string(finger));

    }

    std::cout << "Process thread exiting gracefully." << std::endl;
}

void Image_threads::create(const unsigned short _num_thread = 3){
    Image_threads::num_thread = _num_thread;

    thread_pool = new std::thread[_num_thread];
    for (int i=0; i<_num_thread; i++){
        thread_pool[i] = std::thread(&Image_threads::work);
    }
}

void Image_threads::join(){
    running = false;
    for (int i=0; i<Image_threads::num_thread; i++){
        thread_pool[i].join();
    }
    delete[] thread_pool;
}

void Image_threads::push_(crow::websocket::connection* conn, const cv::Mat& img){
    auto temp = std::make_tuple(conn, img);
    work_queue.push_(temp);
}