#include <Image_threads.h>

#include <opencv2/core.hpp>
#include <chrono>
#include <thread>
#include <sending_thread.h>
#include <Send_queue.h>
#include <crow.h>

#include <Queue.h>
#include <Colors.h>
#include <Torch.h>

unsigned short Image_threads::num_thread = 0;
std::thread* Image_threads::thread_pool;
Queue<cv::Mat> Image_threads::work_queue;

void Image_threads::work(crow::websocket::connection* conn){
    Torch hand_model{"./models/hand_model_c.pt"};
    Torch finger_model{"./models/hand_model_c.pt"};
    while (running) {
        if (work_queue.get_length() == 0) {
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
            continue;
        }
        
        try {
            
            cv::Mat item = work_queue.pop_();
            cv::Mat gray = Colors::gray(item);
            cv::Mat blue = Colors::blue(item);

            const int hand = hand_model.process(item);
            const int finger = finger_model.process(item);

            Send_queue::push_(conn, "image_gray", std::string((char*)gray.data, gray.total() * gray.elemSize()));
            Send_queue::push_(conn, "image_blue", std::string((char*)blue.data, blue.total() * blue.elemSize()));
            Send_queue::push_(conn, "string_hand", std::to_string(hand));
            Send_queue::push_(conn, "string_finger", std::to_string(finger));

        } 
        catch (const std::runtime_error& e) {
            continue;
        }

        
    }

    std::cout << "Process thread exiting gracefully." << std::endl;
}

void Image_threads::create(crow::websocket::connection* conn, const unsigned short num_thread = 3){
    thread_pool = new std::thread[num_thread];
    for (int i=0; i<num_thread; i++){
        thread_pool[i] = std::thread(&Image_threads::work, conn);
    }
}

void Image_threads::join(){
    for (int i=0; i<num_thread; i++){
        thread_pool[i].join();
    }
    delete[] thread_pool;
}

void Image_threads::push_(const cv::Mat& img){
    work_queue.push_(img);
}