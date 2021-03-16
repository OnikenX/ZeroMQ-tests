//
// Created by onikenx on 15/03/21.
//

#include <mutex>
#include <condition_variable>
#include <fmt/printf.h>
#include <fmt/color.h>
#include <iostream>
#include <thread>
#include <future>

#include "threading.h"

using namespace std::literals;

//fmt::terminal_color random_fmt_color() {
//    static auto colours = magic_enum::enum_values<fmt::terminal_color>();
//    static auto distribution = std::uniform_int_distribution<int>(0, colours.size());
//    static auto rdm = std::random_device();
//    return colours[distribution(rdm)];
//}

void random_color_print(const std::string &string) {
    fmt::print(fg(fmt::terminal_color::bright_blue), "{}", string);
}

void thread1(std::shared_ptr<zmq::context_t> context) {
    //auto get = zmq::socket_t();
    fmt::print("Thread");
}


void printValuesThread(int thread_id, int repetitions) {
    for (int i = 0; i < repetitions; i++) {
        std::cout << "Thread " << thread_id << " - " << i << std::endl;
        std::this_thread::sleep_for(300ms);
    }
}

void playWithRandom() {

}


void threadsAandB() {
    auto mtx = std::mutex();
    auto cv = std::condition_variable();

    auto stopworking = std::atomic<bool>(false);
    fmt::print("stopworking: {}\n", stopworking);
    auto thrB = std::thread([&]() {
        auto lk = std::unique_lock(mtx);
        while (!stopworking) {
            fmt::print("Working...\n");
            std::this_thread::sleep_for(300ms);
        }
        lk.unlock();
        fmt::print("Unlooked! getting out!\n");
        std::cout << "Hello from thread B" << std::endl;
    });
    std::cin.get();
    stopworking = true;
    thrB.join();
    fmt::print("out in thread A\n");
}

int thread1(std::atomic<bool> &keepsending, zmq::context_t &context) {
    auto publisher = zmq::socket_t(context, zmq::socket_type::pub);
    publisher.bind("inproc://test"s);
    auto buffer = "Hello World"s;
    auto counter_loops = 0;
    while (keepsending) {
        publisher.send(zmq::buffer(buffer), zmq::send_flags::none);
        std::cout << "Sent: "s << buffer << std::endl;
        std::this_thread::sleep_for(1s);
        counter_loops++;
    }
    return counter_loops;
}

int truenumber(int &num, std::atomic<bool> &cancel) {
    for (int i = 0; i < 7 && !cancel; i++) {
        std::cout << i + 1 << " byou keika" << std::endl;
        std::this_thread::sleep_for(1s);
    }
    std::cout << "WRYYYYYY!!!!!" << std::endl;
    return num += 1;
}

void test_async() {
    auto num = 42;
    std::cout << num << std::endl;
    auto cancel = std::atomic<bool>(false);
    auto future = std::async(std::launch::async, &truenumber, std::ref(num), std::ref(cancel));
    std::cin.get();
    cancel = true;
    future.wait_for(1s);
    std::cout << future.get() << std::endl;
}

void informationPass() {
    auto context = zmq::context_t(1);
    auto keepsending = std::atomic<bool>(true);
    auto future1 = std::async(std::launch::async, [&](){
        return thread1(keepsending, context);
    });
    auto th2 = std::thread([&]() {
        auto subscriber = zmq::socket_t(context, zmq::socket_type::sub);
        subscriber.connect("inproc://test"s);
        auto msgrcv = zmq::message_t();
        while (keepsending) {
            auto value = subscriber.recv(msgrcv, zmq::recv_flags::none);
            if (value.has_value())
                std::cout << "Received: " << msgrcv.str() << std::endl;

        }
    });
    std::cin.get();
    keepsending = false;
    th2.join();
    std::cout << "Ended!" << std::endl;
}

void testing_lambdas(){


    auto lambda = [&](){
        return 2;
    };

    auto future = std::async(std::launch::async, lambda);
    fmt::print("{}\n", future.get());
}