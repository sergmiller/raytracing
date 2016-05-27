//
//  sceneprocessor.cpp
//  raytracing
//
//  Created by Сергей Миллер on 26.05.16.
//  Copyright © 2016 sergmiller. All rights reserved.
//

#ifndef thread_pool_hpp
#define thread_pool_hpp

#include <thread>
#include <mutex>
#include <queue>
#include <future>

using namespace std;

class thread_safe_queue {
private:
    mutex mtx;
    condition_variable not_empty_cv;
    queue<function<void()>> Q;
    atomic<bool> is_shutdown;
public:
    explicit thread_safe_queue() : is_shutdown(false) {}
    ~thread_safe_queue() {}
    void enqueue(function<void()> item);
    void pop(function<void()>& item);
    void shutdown();
    bool empty() { return Q.empty(); }
};


class ThreadPool {
private:
    const size_t num_workers;
    vector<thread> workers;
    atomic<bool> is_shutdown;
    thread_safe_queue TSQ;
    mutex mtx;
public:
    explicit ThreadPool(const size_t num_workers);
    void submit(function<void()> func);
    void shutdown();
};


#endif

