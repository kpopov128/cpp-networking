#include "Thread.h"

namespace networking
{

Thread::~Thread()
{
    if (workerThread.joinable())
        workerThread.join();
}

Thread& Thread::operator=(Thread&& other)
{
    if (this == &other)
        return *this;

    if (workerThread.joinable())
        workerThread.join();

    workerThread = std::move(other.workerThread);

    return *this;
}

} // namespace networking
