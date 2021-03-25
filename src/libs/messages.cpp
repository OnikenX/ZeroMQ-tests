//
// Created by onikenx on 15/03/21.
//

#include "messages.h"
#include <zmq.hpp>
#include <fmt/color.h>
#include <condition_variable>
#include <future>
#include <iostream>

#define TCP_SOCKET 5555

using namespace std::literals;

void sending(zmq::context_t &ctx, std::atomic<bool> &cancel) {
    bool can_add_stuff = false;
    auto requester = zmq::socket_t(ctx, zmq::socket_type::req);
    requester.bind(fmt::format("{}:{}", "tcp://*", TCP_SOCKET));
    auto msg = zmq::message_t();
    while (!cancel) {
        requester.send(zmq::buffer("ORA!"), zmq::send_flags::none);
        requester.recv(msg, zmq::recv_flags::none);
        fmt::print("Received: {}\n", msg.str());
    }
}


void receiving(zmq::context_t &ctx, std::atomic<bool> &cancel) {
    bool can_add_stuff = false;
    auto replayer = zmq::socket_t(ctx, zmq::socket_type::rep);
    replayer.bind(fmt::format("{}{}:{}", "tcp://", "localhost", TCP_SOCKET));
    auto msg = zmq::message_t();
    while (!cancel) {
        replayer.recv(msg, zmq::recv_flags::none);
        fmt::print("Received: {}\n", msg.str());
        replayer.send(zmq::buffer("MUDA!"), zmq::send_flags::none);
    }
}


int exemple_multipart_message() {
    zmq::context_t ctx;
    zmq::socket_t sock1(ctx, zmq::socket_type::push);
    zmq::socket_t sock2(ctx, zmq::socket_type::pull);
    sock1.bind("tcp://127.0.0.1:*");
    const std::string last_endpoint = sock1.get(zmq::sockopt::last_endpoint);
    std::cout << "Connecting to "
              << last_endpoint << std::endl;
    sock2.connect(last_endpoint);

    auto buffer1 = zmq::buffer("foo bar!");
    if (!sock1.send(buffer1, zmq::send_flags::none))
        return 1;

    auto buffer2 = zmq::message_t();
    const auto ret = sock2.recv(buffer2, zmq::recv_flags::none);

    if (!ret)
        return 1;
    std::cout << "Got " << buffer2.str() << " messages" << std::endl;
    return 0;
}


void test_message() {
    auto ctx = zmq::context_t(1);
    auto cancel = std::atomic<bool>(false);
    auto send = std::async(std::launch::async, sending, std::ref(ctx), std::ref(cancel));
    auto recv = std::async(std::launch::async, receiving, std::ref(ctx), std::ref(cancel));
    send.wait();

}

#define MAX_MESSAGES 99999
void pushpull::pusher(zmq::context_t &ctx) {
    auto publisher = zmq::socket_t(ctx, zmq::socket_type::push);
    publisher.bind("tcp://*:9001");
    int create = 1;
    for (; create < MAX_MESSAGES; create++){
        publisher.send(zmq::message_t(&create, sizeof(create)), zmq::send_flags::none);
        std::this_thread::sleep_for(1ms);
    }

}

void pushpull::puller(zmq::context_t &ctx) {
    //get data
    auto subber = zmq::socket_t(ctx, zmq::socket_type::pull);
    subber.connect("tcp://localhost:9001");
    int created = 0;
    auto msg = zmq::message_t();
    while (created < MAX_MESSAGES) {
        subber.recv(msg);
        created = *(int *) msg.data();
        std::this_thread::sleep_for(1s);
        std::cout << "Created: " << created << std::endl;
    }


    //send data
//    auto push_to_sink = zmq::socket_t(ctx, zmq::socket_type::push);
//    push_to_sink.bind("");
}

void pushpull::sinker(zmq::context_t &ctx) {
    //get last data

}

void pushpull::test_init() {
    auto ctx = zmq::context_t(1);
    auto pub = std::async(pusher, std::ref(ctx));
    std::vector<std::future<void>> workers;
    workers.reserve(3);
    std::cout << "Size: " << workers.size() << std::endl;
    for (int i = 0; i < 3; i++)
        workers.push_back(std::move(
                std::async(std::launch::async, puller, std::ref(ctx))
        ));

    pub.wait();
    for (auto &w : workers)
        w.wait();

}







