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


int main() {
    using namespace std::literals;
    //  Prepare our context and requester
    zmq::context_t context(1);
    zmq::socket_t requester(context, zmq::socket_type::req);
    std::string ip;

    std::getline(std::cin, ip);
    std::cout << "Got: " << ip << ";" << std::endl;
    try {
        requester.connect("tcp://" + ip + ":5556");
        std::cout << "Write Something to be reversed, and write \"fim\" or close the keyboard to exit." << std::endl;
        std::atomic<bool> stop = false;
        std::string input;
        zmq::message_t received, send;
        while (!stop) {
            std::cin >> input;

            if (input == "fim"s || std::cin.eof())
                break;
            send = zmq::message_t(input);
            requester.send(send, zmq::send_flags::none);
            requester.recv(received, zmq::recv_flags::none);
            std::cout << received.to_string() << std::endl;
        }
    } catch (...) {
        exit(1);
    }
    std::cout << "Bye!" << std::endl;
    return 0;
}