#pragma once

#include "Thread.h"

#include <functional>
#include <optional>

namespace networking
{

class ShutdownHandler
{
public:
    explicit ShutdownHandler(std::function<void()>&& shutdownCallback);
    ~ShutdownHandler();

    ShutdownHandler(const ShutdownHandler&) = delete;
    ShutdownHandler& operator=(const ShutdownHandler&) = delete;

private:
    std::function<void()> shutdownCb;
    std::optional<Thread> signalThread;
};

} // namespace networking
