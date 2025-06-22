#pragma once
#include <future>
#include <mutex>
#include <thread>
#include <type_traits>
#include "EFCoreModuleAPI.h"
#include "CoreMacros.h"


namespace EventfulEngine{
    class EFCORE_API Thread{
    public:
        Thread() = default;

        NOCOPY(Thread)

        template <class Callable, class... Args>
            requires (!std::same_as<Thread, std::remove_cvref_t<Callable>> &&
                !std::same_as<Thread, std::remove_cvref_t<Args...>> &&
                std::is_invocable_v<Callable, Args...>)
        explicit Thread(Callable&& f, Args&&... args)
            : _thread(std::forward<Callable>(f), std::forward<Args>(args)...){
        }


        Thread(Thread&&) noexcept = default;

        Thread& operator=(Thread&&) noexcept = default;

        ~Thread();

        /** Join the underlying thread if joinable. */
        void Join();

        /** Check if the thread is joinable. */
        [[nodiscard]] bool Joinable() const noexcept;

    private:
        std::jthread _thread;
    };

    /**
     * @brief Launch an asynchronous task returning a std::future.
     */
    template <class Callable, class... Args>
    auto Async(Callable&& f, Args&&... args)
        -> std::future<std::invoke_result_t<Callable, Args...>>{
        return std::async(std::launch::async,
                          std::forward<Callable>(f),
                          std::forward<Args>(args)...);
    }

    using Mutex = std::mutex;
    using RecursiveMutex = std::recursive_mutex;

    template <class MutexT>
    using UniqueLock = std::unique_lock<MutexT>;

    template <class MutexT>
    using ScopeLock = std::lock_guard<MutexT>;
}
