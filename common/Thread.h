#pragma once

#include <thread>
#include <utility>
#include <type_traits>

namespace networking
{

class Thread
{
public:
    template<typename Callable, typename... Args>
        requires (!std::is_same_v<std::remove_cvref_t<Callable>, Thread>)
    explicit Thread(Callable&& f, Args&&... Arg) :
        workerThread(std::forward<Callable>(f), std::forward<Args>(Arg)...)
    {
    }

    Thread(const Thread& other) = delete;
    Thread& operator=(const Thread& other) = delete;

    Thread(Thread&& other) noexcept = default;
    Thread& operator=(Thread&& other);

    ~Thread();

    std::thread::id GetId() const noexcept{ return workerThread.get_id(); }
private:
    std::thread workerThread;
};

} // namespace networking
