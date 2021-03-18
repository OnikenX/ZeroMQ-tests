cmake_minimum_required(VERSION 3.17)
project(ZeroMQ_tests)

set(CMAKE_CXX_STANDARD 20)

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -Wall -Wextra" #[[-fsanitize=address]])
endif ()

include_directories(src)

find_package(fmt REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(cppzmq REQUIRED)
set(packages cppzmq fmt nlohmann_json)

add_executable(ZeroMQ_tests src/thread-play.cpp src/libs/threading.cpp src/libs/messages.cpp src/libs/exemples_zmq.cpp src/libs/json_tests.cpp)
add_executable(puller src/pairs/pushpull/puller.cpp src/libs/messages.cpp)
add_executable(pusher src/pairs/pushpull/pusher.cpp src/libs/messages.cpp)
add_executable(pub src/pairs/pubsub/pub.cpp)
add_executable(sub src/pairs/pubsub/sub.cpp)
add_executable(req src/pairs/reqrep/req.cpp)
add_executable(rep src/pairs/reqrep/rep.cpp)
add_executable(dechat src/translations/dechat.cpp)
target_link_libraries(ZeroMQ_tests ${packages})
target_link_libraries(puller ${packages})
target_link_libraries(pusher ${packages})
target_link_libraries(pub ${packages})
target_link_libraries(sub ${packages})
target_link_libraries(req ${packages})
target_link_libraries(rep ${packages})
target_link_libraries(dechat ${packages})


