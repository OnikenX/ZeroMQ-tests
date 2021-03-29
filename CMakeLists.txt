cmake_minimum_required(VERSION 3.17.5)

set(CMAKE_CXX_STANDARD 20)
include("cmake/HunterGate.cmake")
HunterGate(
        URL "https://github.com/cpp-pm/hunter/archive/v0.23.296.tar.gz"
        SHA1 "232f5022ee1d45955a7e8d3e1720f31bac1bb534"
)

project(ZeroMQ_tests)


if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -Wall -Wextra" #[[-fsanitize=address]])
endif ()

include_directories(src)

find_package(fmt REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(cppzmq REQUIRED)


## holepunching
add_executable(udp_punch_client src/punchholeing/udp/client.cpp)
add_executable(udp_punch_server src/punchholeing/udp/server.cpp)

## zeromq
add_executable(ZeroMQ_tests src/thread-play.cpp src/libs/threading.cpp src/libs/messages.cpp src/libs/exemples_zmq.cpp src/libs/json_tests.cpp)
target_link_libraries(ZeroMQ_tests cppzmq fmt::fmt)

add_executable(puller src/pairs/pushpull/puller.cpp src/libs/messages.cpp)
target_link_libraries(puller cppzmq)

add_executable(pusher src/pairs/pushpull/pusher.cpp src/libs/messages.cpp)
target_link_libraries(pusher cppzmq)

add_executable(pub src/pairs/pubsub/pub.cpp)
target_link_libraries(pub cppzmq)

add_executable(sub src/pairs/pubsub/sub.cpp)
target_link_libraries(sub cppzmq)


add_executable(req src/pairs/reqrep/req.cpp)
target_link_libraries(req cppzmq)

add_executable(rep src/pairs/reqrep/rep.cpp)
target_link_libraries(rep cppzmq)

add_executable(dechat src/translations/dechat.cpp)
target_link_libraries(dechat)

#add_executable(udpping1 src/translations/udpping1.cpp)
#target_link_libraries(udpping1 ${packages} cppzmq)

add_executable(udptest src/udp/udptest.cpp)

## asio / DOES NOT WORK
add_executable(asio_test src/asio/asio_main.cpp)

## libp2p tests
hunter_add_package(libp2p)
find_package(libp2p REQUIRED)
add_executable(p2pchat src/libp2p/p2pchat.cpp)
