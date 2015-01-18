#ifndef BOP_THREADPOOL_HPP
#define BOP_THREADPOOL_HPP
#include <thread>
#include <utility>
#include <algorithm>
#include <mutex>
#include <queue>
#include <functional>
#include "../bop-defaults/types.hpp"

/*
    Threadpool class, has a queue of wrapped functions that
    will be executed as the threads become available.
*/

namespace bop {
    namespace util {
        class ThreadPool {
            private:

                void thread_function(const uint_type thread_index) {
                    /*
                        Code that the threads run continously.
                    */
                    std::function<void()> current_function = nullptr;
                    while(this->active) {
                        this->thread_activity[thread_index] = false;
                        /*
                            Attempt to lock the task_queue mutex so that we may
                            take a task from it without data racing.
                        */
                        if (this->run_functions && this->task_queue_mutex.try_lock()) {
                            /*
                                If the mutex was successfully locked then we check
                                for a task.
                            */
                            if (this->task_queue.size() > 0) {
                                /*
                                    If there is an available task function, move it out of the
                                    queue and into the current_function variable, then pop the
                                    now empty front of the queue.
                                */
                                current_function = std::move(this->task_queue.front());
                                this->task_queue.pop();
                            }
                            /*
                                Unlock the mutex now all that can be done to the queue
                                this iteration has been done.
                            */
                            this->task_queue_mutex.unlock();

                            if (current_function != nullptr) {
                                /*
                                    If a task function was taken from the queue then run it
                                    and set the current_function variable to an empty function
                                    so that the task is only run once.
                                */
                                this->thread_activity_mutex[thread_index].lock();
                                this->thread_activity[thread_index] = true;
                                this->thread_activity_mutex[thread_index].unlock();
                                current_function();
                                this->thread_activity_mutex[thread_index].lock();
                                this->thread_activity[thread_index] = false;
                                this->thread_activity_mutex[thread_index].unlock();
                                current_function = nullptr;
                            }
                        }

                    }
                }

                /*
                    The task queue and it's related mutex
                */
                std::queue<std::function<void()> > task_queue;
                std::mutex task_queue_mutex;

                /*
                    Array of threads
                */
                std::vector<std::thread> threads;

                /*
                    std::vector of booleans corresponding to threads
                    in the member "threads", each index indicates the
                    thread's current actvity.
                */

                std::vector<bool> thread_activity;
                std::vector<std::mutex> thread_activity_mutex;

                /*
                    boolean flag to prevent execution of further
                    tasks until the flag is true again.
                */
                bool run_functions;

                /*
                    boolean flag that will break all threads after their next
                    completed task if false.
                */

                bool active;



            public:
                ThreadPool() = delete;

                ThreadPool(uint_type reserve_threads) : task_queue(), run_functions(true), active(true) {
                    this->thread_activity_mutex = std::vector<std::mutex>(reserve_threads);
                    for (uint_type iter = 0; iter < reserve_threads; iter++) {
                        /*
                            Push the thread activity flag first so if it
                            may not be accessed by the thread before it
                            exists.
                        */
                        this->thread_activity.push_back(false);
                        this->threads.push_back(std::thread([this,iter]() -> void {this->thread_function(iter);}));

                    }
                }

                template<class Func, class ...Args>
                void addTask(Func&& function, Args&&... arguments) {
                    /*
                        The function needs to be wrapped in an anonymous
                        function of type void() so that it may be appended
                        to the task queue.
                    */
                    this->task_queue_mutex.lock();
                    this->task_queue.push([&,function,arguments...]() -> void {
                        function(arguments...);
                    });
                    this->task_queue_mutex.unlock();
                }

                ~ThreadPool() {
                    /*
                        Set the activation variables to false, closing some
                        threads as soon as possible.
                    */
                    this->run_functions = false;
                    this->active = false;
                    /*
                        Lock the task queue mutex, preventing any more tasks from
                        being executed.
                    */
                    this->task_queue_mutex.lock();
                    /*
                        Finally wait for the rest of the threads by requesting them
                        to join this thread in order of construction.
                    */
                    for (uint_type iter = 0; iter < this->threads.size(); iter++) {
                        this->threads[iter].join();
                    }

                }


                uint_type numberOfTasks() const {
                    return this->task_queue.size();
                }

                uint_type numberOfThreads() const {
                    return this->threads.size();
                }

                bool hasRunning() {
                    /*
                        Returns true if there are any threads running a task,
                        false otherwise.

                        If this function were to indicate if I've overused lambdas
                        a lot, and should probably stop but won't, it would return
                        true at all instances.
                    */
                    return [this]() -> bool {
                        bool has_running = false;
                        for (uint_type iter = 0; iter < this->thread_activity.size() && !has_running; iter++) {
                            this->thread_activity_mutex[iter].lock();
                            has_running |= this->thread_activity[iter];
                            this->thread_activity_mutex[iter].unlock();
                        }
                        return has_running;
                    }();
                }

                bool isActive() {
                    return this->run_functions;
                }

                void toggleRunning() {
                    this->run_functions = !this->run_functions;
                }


        };
    }
}

#endif
