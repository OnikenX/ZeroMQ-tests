//
// Created by onikenx on 15/03/21.
//

#ifndef ZEROMQ_TESTS_EXEMPLES_ZMQ_H
#define ZEROMQ_TESTS_EXEMPLES_ZMQ_H

#include <zmq.hpp>
#include <zmq_addon.hpp>

namespace exemples_zmq{

};

int hello_world_push(zmq::context_t &ctx);

int hello_world_pull(zmq::context_t &ctx);

int test_hello_world();

int multipart_messages();

//// ############ pubsub_multithread_inproc ################

void PublisherThread(zmq::context_t *ctx);

void SubscriberThread1(zmq::context_t *ctx);

void SubscriberThread2(zmq::context_t *ctx);

int test_pubsub_multithread_inproc();

#endif //ZEROMQ_TESTS_EXEMPLES_ZMQ_H
