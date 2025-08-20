#include <iostream>
#include <Queue.h>
#include <Functional.h>

int main(){
    
    Functional<Queue, int> fun1, fun2;
    
    for (int i=0; i<9; i++){
        fun2 = fun1.push(i);
    }
    for (int elem : fun2) {
        std::cout << elem << std::endl;
    }
    Functional<Queue, int> fun3 = fun2.map([](int elem){
        return elem * 2;
    });
    for (int elem : fun3) {
        std::cout << elem << std::endl;
    }

    return 0;
}