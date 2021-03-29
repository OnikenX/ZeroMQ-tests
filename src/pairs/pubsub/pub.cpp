//
// Created by onikenx on 16/03/21.
//

//
//  Weather update server in C++
//  Binds PUB socket to tcp://*:5556
//  Publishes random weather updates
//
#include <zmq.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <atomic>
#include <random>
#include <iostream>
#include <thread>
#include <fmt/format.h>
#include <future>


#include "pubsub.h"

int main() {
    using namespace std::literals;
    auto ctx = zmq::context_t(1);
    auto pub = zmq::socket_t(ctx, zmq::socket_type::pub);
    pub.bind("tcp://*:5555");
    int message = 1;
    std::atomic<bool> cancel = false;
    auto f1 = std::async([&]() {
        std::cout << "Press any key to stop." << std::endl;
        std::cin.get();
        cancel = true;
    });
    std::ostringstream oss;
    while (!cancel) {
        try {
            auto str = std::format("{} {}", default_sub, message);
            std::cout << "To send str: " << str << std::endl;
            pub.send(zmq::message_t(str));
            std::cout << "Sent: " << message << std::endl;
            std::this_thread::sleep_for(2s);
            message++;
            if (message >= 99999) break;
        } catch (std::exception &e) {
            std::cout << "Some error happened... " << e.what();
        }
    }
    std::cout << message-1 << " messages sent." << std::endl;
}
