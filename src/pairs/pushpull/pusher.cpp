//
// Created by onikenx on 15/03/21.
//

#include <libs/messages.h>

int main(){
    auto ctx = zmq::context_t(1);
    pushpull::pusher(ctx);

}
