//
// Created by onikenx on 16/03/21.
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

#if (defined (WIN32))
#include <zhelpers.hpp>
#endif

#define within(num) (int) ((float) num * random () / (RAND_MAX + 1.0))

int main() {
    //  Prepare our context and publisher
    zmq::context_t context(1);
    zmq::socket_t replayer(context, zmq::socket_type::rep);
    replayer.bind("tcp://*:5556");
    std::cout << "Server up and running." << std::endl;
    std::atomic<bool> stop = false;
    zmq::message_t received, sent;
    while (!stop) {
        auto size = replayer.recv(received);
        auto start = std::chrono::steady_clock::now();
        if (!size.has_value() && size.value() <= 0)
            continue;
        auto s = received.to_string();
        std::reverse(std::begin(s), std::end(s));
        sent = zmq::message_t(s);
        replayer.send(sent, zmq::send_flags::none);
        auto end = std::chrono::steady_clock::now();
        std::cout << "Elapsed time in nanoseconds : "
             << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count()
             << " ns" << std::endl;

        std::cout << "Elapsed time in microseconds : "
             << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count()
             << " µs" << std::endl;

        std::cout << "Elapsed time in milliseconds : "
             << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
             << " ms" << std::endl;

        std::cout << "Elapsed time in seconds : "
             << std::chrono::duration_cast<std::chrono::seconds>(end - start).count()
             << " sec" << std::endl;


    }
    return 0;
}