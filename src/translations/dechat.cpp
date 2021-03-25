//  Decentralized chat example

#include <zmq.hpp>
#include <future>
#include <iostream>
#define PORT 9000

static void listener_task(char *args, zmq::context_t &ctx) {
    auto listener = zmq::socket_t(ctx, zmq::socket_type::sub);
    char conn_str[100];
    for (int address = 1; address < 255; address++) {
        sprintf(conn_str, "tcp://%s%d:%d", args, address, PORT);
        std::cout << "conn_str: " << conn_str << std::endl;
        listener.connect(conn_str);
    }

    listener.set(zmq::sockopt::subscribe, "");
    while (true) {
        auto msg = zmq::message_t();
        try {
            zmq::recv_result_t size_rcv = listener.recv(msg);
        } catch (std::exception &e) {
            std::cerr << e.what() << std::endl;
            break;
        }
        if (!msg.empty())
            std::cout << msg.to_string() << std::endl;
        else
            std::cerr << "Msg empty" << std::endl;
    }
}

int main(int argc, char *argv[]) {
    char *dot = argc == 4 ? strrchr(argv[1], '.') : NULL;
    if (dot)
        dot[1] = 0;        //  Cut string after dot
    else {
        puts("Usage: dechat ipaddress interface username");
        puts("Example: dechat 192.168.55.123 eth0 joe");
        exit(0);
    }
    auto ctx = zmq::context_t(1);
    auto broadcaster = zmq::socket_t(ctx, zmq::socket_type::pub);
    std::stringstream ss;
    ss << "tcp://" << argv[2] << ":9000";
    broadcaster.bind(ss.str());
    std::cout << "binded to " << ss.str() << std::endl;
    auto f = std::async(std::launch::async, listener_task, argv[1], std::ref(ctx));
    try {
        while (true) {
            std::string message;
            std::getline(std::cin, message);
            ss = std::stringstream();
            ss << argv[3] << ": " << message;
            broadcaster.send(zmq::message_t(ss.str()), zmq::send_flags::none);
        }
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
    }
}

