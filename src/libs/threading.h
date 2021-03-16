//
// Created by onikenx on 15/03/21.
//

#ifndef ZEROMQ_TESTS_THREADING_H
#define ZEROMQ_TESTS_THREADING_H

#include <atomic>
#include <zmq.hpp>

void printValuesThread(int thread_id, int repetitions = 10);
void playWithRandom();
void threadsAandB();

int truenumber(int &num, std::atomic<bool>& cancel) ;

void test_async();
void informationPass();
void testing_lambdas();

#endif //ZEROMQ_TESTS_THREADING_H
