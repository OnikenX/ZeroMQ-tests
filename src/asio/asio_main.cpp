//
// Created by onikenx on 26/03/21.
//
#include <asio.hpp>
#include <thread>
#include <iostream>

using namespace std::literals;

//### waiters ###//
void wait(bool async) {
    asio::io_context io;
    asio::steady_timer t(io, 5s);

    if (async) {
        // com este tem se uma thread internamente
        std::async(std::launch::async, []() {
            std::this_thread::sleep_for(5s);
            std::cout << "Waited(sleep) in async with std::thread for 5 seconds." << std::endl;
        });
        t.async_wait([&](const auto &) { std::cout << "Waited in async with asio for 5 seconds." << std::endl; });
        io.run();

    } else {
        //wait for 5seconds in the asio way
        t.wait();
        std::cout << "Waited with asio for 5 seconds." << std::endl;

        //std::thread way
        std::this_thread::sleep_for(5s);
        std::cout << "Waited(sleep) with std::thread for 5 seconds." << std::endl;
    }
}


//##### dynamic timer #####//
void print_dt(const auto &, asio::steady_timer &t, int &count) {
    if (count < 5) {
        std::cout << count << std::endl;
        ++count;
        t.expires_at(t.expiry() + 1s);
        t.async_wait([&](const auto &e) { print_dt(e, t, count); });
    }
}

void dynamic_timer() {
    asio::io_context io;
    int count = 0;
    asio::steady_timer t(io, asio::chrono::seconds(1));
    t.async_wait([&](const auto &e) {
        print_dt(e, t, count);
    });
    io.run();
    std::cout << "Final count is " << count << std::endl;
}


class printer_async {
private:
    asio::steady_timer timer_;
    int count_;
public :
    printer_async(asio::io_context &io) : timer_(io, 1s), count_(0) {
        timer_.async_wait([&](const auto &) { print(); });
    }

    void print() {
        if (count_ < 5) {
            std::cout << count_ << std::endl;
            ++count_;

            timer_.expires_at(timer_.expiry() + asio::chrono::seconds(1));
            timer_.async_wait([&](const auto &) { print(); });
        }
    }

    ~printer_async() {
        std::cout << "Final count is " << count_ << std::endl;
    }
};

//class printer_threaded {
//private:
//    asio::strand<asio::io_context::executor_type> strand_;
//    asio::steady_timer timer1_;
//    asio::steady_timer timer2_;
//    int count_;
//
//public:
//    printer_threaded(asio::io_context &io)
//            : strand_(asio::make_strand(io)),
//              timer1_(io, 1s),
//              timer2_(io, 1s),
//              count_(0) {
//        timer1_.async_wait(asio::bind_executor(strand_, [&] { print1(); }));
//        timer2_.async_wait(asio::bind_executor(strand_, [&] { print2(); }));
//    }
//
//    ~printer_threaded() {
//        std::cout << "Final count is " << count_ << std::endl;
//    }
//
//    void print1() {
//        if (count_ < 10) {
//            std::cout << "Timer 1: " << count_ << std::endl;
//            ++count_;
//            timer1_.expires_at(timer1_.expiry() + 1s);
//            timer1_.async_wait(asio::bind_executor(strand_, [&] { print1(); }));
//        }
//    }
//
//    void print2() {
//        if (count_ < 10) {
//            std::cout << "Timer 2: " << count_ << std::endl;
//            ++count_;
//
//            timer2_.expires_at(timer2_.expiry() + 1s);
//
//            timer2_.async_wait(asio::bind_executor(strand_, [&] { print2(); }));
//        }
//    }
//};

int main() {
    asio::io_context io;
    printer_threaded p(io);
    asio::thread t([&] { io.run(); });
    io.run();
    t.join();

    return 0;
}