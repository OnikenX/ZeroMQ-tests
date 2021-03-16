//
// Created by onikenx on 15/03/21.
//

#include <future>
#include <iostream>
#include "exemples_zmq.h"

static std::atomic<bool> cancel = false;
static std::mutex output_mtx;
using namespace std::literals;


int hello_world_push(zmq::context_t &ctx) {
    zmq::socket_t sock(ctx, zmq::socket_type::push);

    //only one can bind
    sock.bind("inproc://test");
    if(!sock.send(zmq::str_buffer("Hello, world"), zmq::send_flags::none))
        return 1;
    auto lock = std::lock_guard(output_mtx);
    std::cout << "Sent: Hello, world"<< std::endl;
    return 0;
}

int hello_world_pull(zmq::context_t &ctx) {

    zmq::socket_t sock(ctx, zmq::socket_type::pull);
    //the others connect, other wise there will be errors and throws
    sock.connect("inproc://test");
    auto msg = zmq::message_t("Hello, world"s);
    if (!sock.recv(msg, zmq::recv_flags::none))
        return 1;
    auto lock = std::lock_guard(output_mtx);
    std::cout <<"Received: "s <<  msg.to_string() << std::endl;
    return 0;
}


int test_hello_world() {
    auto ctx = zmq::context_t(1);
    auto sender = std::async(std::launch::async, hello_world_push, std::ref(ctx));
    auto receiver = std::async(std::launch::async, hello_world_pull, std::ref(ctx));
    return sender.get() + receiver.get();
}


int multipart_messages() {
    zmq::context_t ctx(1);
    zmq::socket_t sock1(ctx, zmq::socket_type::push);
    zmq::socket_t sock2(ctx, zmq::socket_type::pull);
    sock1.bind("tcp://127.0.0.1:*");
    const std::string last_endpoint =
            sock1.get(zmq::sockopt::last_endpoint);
    std::cout << "Connecting to "
              << last_endpoint <<
              std::endl;
    sock2.
            connect(last_endpoint);

    std::array<zmq::const_buffer, 2> send_msgs = {
            zmq::str_buffer("foo"),
            zmq::str_buffer("bar!")
    };
    if (!
            zmq::send_multipart(sock1, send_msgs
            ))
        return 1;

    std::vector<zmq::message_t> recv_msgs;
    const auto ret = zmq::recv_multipart(
            sock2, std::back_inserter(recv_msgs));
    if (!ret)
        return 1;
    std::cout << "Got " << *ret
              << " messages" <<
              std::endl;
    return 0;
}



//// ############ pubsub_multithread_inproc ################

void SubscriberThread2(zmq::context_t *ctx) {
    //  Prepare our context and subscriber
    zmq::socket_t subscriber(*ctx, zmq::socket_type::sub);
    subscriber.connect("inproc://#1");

    //  Thread3 opens ALL envelopes
    subscriber.set(zmq::sockopt::subscribe, "");

    while (1) {
        // Receive all parts of the message
        std::vector<zmq::message_t> recv_msgs;
        zmq::recv_result_t result =
                zmq::recv_multipart(subscriber, std::back_inserter(recv_msgs));
        assert(result && "recv failed");
        assert(*result == 2);

        std::cout << "Thread3: [" << recv_msgs[0].to_string() << "] "
                  << recv_msgs[1].to_string() << std::endl;
    }
}

void SubscriberThread1(zmq::context_t *ctx) {
    //  Prepare subscriber
    zmq::socket_t subscriber(*ctx, zmq::socket_type::sub);
    subscriber.connect("inproc://#1");

    //  Thread2 opens "A" and "B" envelopes
    subscriber.set(zmq::sockopt::subscribe, "A");
    subscriber.set(zmq::sockopt::subscribe, "B");

    while (!cancel) {
        // Receive all parts of the message
        std::vector<zmq::message_t> recv_msgs;
        zmq::recv_result_t result =
                zmq::recv_multipart(subscriber, std::back_inserter(recv_msgs));
        assert(result && "recv failed");
        assert(*result == 2);

        std::cout << "Thread2: [" << recv_msgs[0].to_string() << "] "
                  << recv_msgs[1].to_string() << std::endl;
    }
}

void PublisherThread(zmq::context_t *ctx) {
    //  Prepare pusher
    zmq::socket_t publisher(*ctx, zmq::socket_type::pub);
    publisher.bind("inproc://#1");

    // Give the subscribers a chance to connect, so they don't lose any messages
    std::this_thread::sleep_for(20ms);

    while (!cancel) {
        //  Write three messages, each with an envelope and content
        publisher.send(zmq::str_buffer("A"), zmq::send_flags::sndmore);
        publisher.send(zmq::str_buffer("Message in A envelope"));
        publisher.send(zmq::str_buffer("B"), zmq::send_flags::sndmore);
        publisher.send(zmq::str_buffer("Message in B envelope"));
        publisher.send(zmq::str_buffer("C"), zmq::send_flags::sndmore);
        publisher.send(zmq::str_buffer("Message in C envelope"));
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

int test_pubsub_multithread_inproc() {

    /*
     * No I/O threads are involved in passing messages using the inproc transport.
     * Therefore, if you are using a ØMQ context for in-process messaging only you
     * can initialise the context with zero I/O threads.
     *
     * Source: http://api.zeromq.org/4-3:zmq-inproc
     */
    zmq::context_t ctx(0);

    auto thread1 = std::async(std::launch::async, PublisherThread, &ctx);

    // Give the pusher a chance to bind, since inproc requires it
    std::this_thread::sleep_for(10ms);

    auto thread2 = std::async(std::launch::async, SubscriberThread1, &ctx);
    auto thread3 = std::async(std::launch::async, SubscriberThread2, &ctx);

    thread1.wait();
    thread2.wait();
    thread3.wait();
    return 0;
    /*
     * Output:
     *   An infinite loop of a mix of:
     *     Thread2: [A] Message in A envelope
     *     Thread2: [B] Message in B envelope
     *     Thread3: [A] Message in A envelope
     *     Thread3: [B] Message in B envelope
     *     Thread3: [C] Message in C envelope
     */
}





