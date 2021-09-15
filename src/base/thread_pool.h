#ifndef _BASE_THREAD_POOL_H_
#define _BASE_THREAD_POOL_H_
#include <queue>
#include <mutex>
#include <vector>
#include <memory>
#include <atomic>
#include <thread>
#include <future>
#include <stdexcept>
#include <functional>
#include <condition_variable>
namespace ez
{
    namespace base
    {
        class thread_pool
        {
        public:
            using task_function_t = std::function<void()>;

        private:
            using task_list_t = std::queue<task_function_t>;
            using workers_t   = std::vector<std::thread>;

        public:
            thread_pool(int size)
                : _size(size)
                , _stop(false)
            {
                auto worker_proc = [this]
                {
                    while (true)
                    {
                        thread_pool::task_function_t task;

                        {
                            std::unique_lock<std::mutex> __lock(this->_task_mutex);
                            this->_task_condition.wait(
                                __lock,
                                [this]
                                {
                                    return this->_stop || !this->_task_list.empty();
                                });
                            if (this->_stop && this->_task_list.empty())
                            {
                                return;
                            }
                            task = std::move(this->_task_list.front());
                            this->_task_list.pop();
                        }

                        task();
                    }
                };

                for (int i = 0; i < size; i++)
                {
                    this->_workers.emplace_back(worker_proc);
                }
            }

            ~thread_pool()
            {
                for (auto& worker : this->_workers)
                {
                    if (worker.joinable())
                    {
                        worker.join();
                    }
                }
            }

            template <class F, class... Args>
            auto commit(F&& f, Args&&... args) -> std::future<typename std::result_of<F(Args...)>::type>
            {
                using return_type = typename std::result_of<F(Args...)>::type;

                auto task = std::make_shared<std::packaged_task<return_type()> >(
                    std::bind(std::forward<F>(f), std::forward<Args>(args)...));

                std::future<return_type> res = task->get_future();
                {
                    std::unique_lock<std::mutex> __lock(_task_mutex);

                    // don't allow committing after stopping the pool
                    if (_stop)
                    {
                        throw std::runtime_error("Don't allow committing task to a stopped thread pool.");
                    }

                    _task_list.emplace(
                        [task]()
                        {
                            (*task)();
                        });
                }
                _task_condition.notify_one();
                return res;
            }

            void stop()
            {
                _stop = true;
                _task_condition.notify_all();
            }

        private:
            int                     _size;
            std::atomic_bool        _stop;
            task_list_t             _task_list;
            workers_t               _workers;
            std::mutex              _task_mutex;
            std::condition_variable _task_condition;
        };
    } // namespace base
} // namespace ez
#endif // !_BASE_THREAD_POOL_H_