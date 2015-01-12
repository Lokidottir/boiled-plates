#ifndef BOP_THREADPOOL_HPP
#define BOP_THREADPOOL_HPP
#include <thread>
#include <utility>
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

                void thread_function() {
                    /*
                        Code that the threads run continously.
                    */
                    std::function<void()> current_function = nullptr;
                    while(this->run_functions) {
                        /*
                            Attempt to lock the task_queue mutex so that we may
                            take a task from it without data races.
                        */
                        if (this->task_queue_mutex.try_lock()) {
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
                                current_function();
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
                    boolean flag to prevent execution of further
                    tasks.
                */
                bool run_functions;

            public:
                ThreadPool() = delete;

                ThreadPool(uint_type reserve_threads) : task_queue(), run_functions(true) {
                    for (uint_type iter = 0; iter < reserve_threads; iter++) {

                        this->threads.push_back(std::thread([this]() -> void {this->thread_function();}));
                    }
                }

                template<class Func, class ...Args>
                void addTask(Func&& function, Args&&... arguments) {
                    /*
                        The function needs to be wrapped in an anonymous
                        function of type void() so that it may be appended
                        to the task queue.
                    */
                    while(!this->task_queue_mutex.try_lock());
                    this->task_queue.push([&,function,arguments...]() -> void {
                        function(arguments...);
                    });
                    this->task_queue_mutex.unlock();
                }

                ~ThreadPool() {
                    /*
                        Set the run_functions variable to false, closing some
                        threads as soon as possible.
                    */
                    this->run_functions = false;
                    /*
                        Lock the task queue mutex, preventing any more tasks from
                        being executed.
                    */
                    while(!this->task_queue_mutex.try_lock());
                    /*
                        Finally wait for the rest of the threads by requesting them
                        to join this thread in order of construction.
                    */
                    for (uint_type iter = 0; iter < this->threads.size(); iter++) {
                        this->threads[iter].join();
                    }

                }


                uint_type size() {
                    return this->task_queue.size();
                }

        };
    }
}

#endif
