//
// Created by onikenx on 11/03/21.
//

#include <libs/threading.h>
#include <future>
#include <iostream>
#include "libs/messages.h"
#include "libs/exemples_zmq.h"

int main() {
//    auto ctx = zmq::context_t(1);
//    auto pub = zmq::socket_t(ctx, zmq::socket_type::sub);
    auto cancel = std::atomic<bool>(false);
    auto exiter = std::async([&](){
        std::cout << "Press key to cancel." << std::endl; std::cin.get();
        cancel = true;
    });
    auto ss = std::stringstream();
    while(!cancel){
        ss = std::stringstream();
        ss << "hello";
        std::cout << ss.str() << std::endl;
        using namespace std::literals;
        std::this_thread::sleep_for(1s);
    }
}