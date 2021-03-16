//
// Created by onikenx on 15/03/21.
//

#ifndef ZEROMQ_TESTS_MESSAGES_H
#define ZEROMQ_TESTS_MESSAGES_H

#include <zmq.hpp>
#include <vector>
#include <mutex>

void sending(zmq::context_t &ctx, std::atomic<bool> &cancel);
int exemple_multipart_message();
void test_message();

namespace pushpull{
    static auto mtx_pubsub =  std::mutex();
    void pusher(zmq::context_t& ctx);
    void puller(zmq::context_t& ctx);
    void sinker(zmq::context_t& ctx);
    void test_init();
}

#endif //ZEROMQ_TESTS_MESSAGES_H
