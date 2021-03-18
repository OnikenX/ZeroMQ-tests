# ZeroMQ Playground

This is my repo for testing c++ concurrency, zmq and some others libs for networking and some related stuff.

## ZeroMQ
ZeroMQ is a somewhat network wrapper that treats some of the most mundane tasks for you, they are in general easy to use on their own and simple for projects.

### Documentation

ZeroMQ has a lot of binding for their c api, python, java, php, etc, the one I'm more interested in c++ one, more specifically the cppzmq bindings, there are others that have other proposes but I choose this one because it had more likes in github and looked like the one with the biggest support from the main community.

The documentation for cppzmq is somewhat laking, the c api has all the documentation, and there is an [amazing guide](https://zguide.zeromq.org/) that explains how to use it but with the translations(other binding other than the c api) don't have their own documentation.

### Socket types

In zmq we have 4 pair type sockets, you can match some but not others, they follow somewhat the server-client logic:

| Server          | Client           | Description                                                                                                      |
| --------------- | ---------------- | ---------------------------------------------------------------------------------------------------------------- |
| Replay (REP)    | Request (REQ)    | Request sends a message and request replays to it                                                                |
| Push (PUSH)     | Pull (PULL)      | The push socket sends a message to the pull sockets connected to it (It sends a different message for each one). |
| Publisher (PUB) | Subscriber (SUB) | The idea is the same as the push-pull but the messages are same instead of different messages for each.          |

These are the sockets that I looked more into. For a full list of sockets see the `zmq_socket` man page.

