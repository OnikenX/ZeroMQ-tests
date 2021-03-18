//
// Created by onikenx on 16/03/21.
//

//
//  Weather update client in C++
//  Connects SUB socket to tcp://localhost:5556
//  Collects weather updates and finds avg temp in zipcode
//

#include <zmq.hpp>
#include <iostream>
#include <sstream>


#include "pubsub.h"

int main(int argc, char *argv[]) {
    zmq::context_t context(1);

    //  Socket to talk to server
    zmq::socket_t subscriber(context, zmq::socket_type::sub);
    subscriber.connect("tcp://localhost:5555");
    subscriber.set(zmq::sockopt::subscribe, default_sub);
    auto msg = zmq::message_t();
    int recebido = 0;
    while(recebido <=100) {
        std::cout << "entrou\n";
        subscriber.recv(msg);
        std::cout << "Recebi: " << recebido << std::endl;
    }
    std::cout << "Acabou!!"<< std::endl;
}