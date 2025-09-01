#include <iostream>
#include <Single_threaded_Queue.h>
#include <Queue.h>
#include <thread>
#include <Functional.h>

Functional<Queue, int> shared;
Functional<Single_threaded_Queue, int> unsafe;

// 숫자가 커진다면 공유 자원 접근으로 인한 에러가 분명해진다.
void push_to_shared(int num){
    for (int i=0; i<10000; i++){
        shared.push_(i * num);
    }
}

void push_to_unsafe(int num){
    for (int i=0; i<10000; i++){
        unsafe.push_(i * num);
    }
}

int main(){
    std::thread t1(&push_to_shared, 1), t2(&push_to_shared, 2);
    
    t1.join();
    t2.join();
    
    std::cout << "safely shared queue : " << '\n';
    for (int elem : shared){
        std::cout << elem << '\n';
    }
    std::cout << '\n';

    return 0;

    // 이 아래 실행은 권장되지 않습니다.
    std::thread t3(&push_to_unsafe, 1), t4(&push_to_unsafe, 2);

    t3.join();
    t4.join();

    std::cout << "Normal queue : " << '\n';
    for (int elem : unsafe){
        std::cout << elem << '\n';
    }
    std::cout << '\n';

    return 0;
}