#include "ShutdownHandler.h"

#include <signal.h>
#include <pthread.h>
#include <utility>

namespace networking
{

ShutdownHandler::ShutdownHandler(std::function<void()>&& shutdownCallback)
    : shutdownCb{std::move(shutdownCallback)}
{
    sigset_t signals;

    sigemptyset(&signals);
    sigaddset(&signals, SIGINT);

    pthread_sigmask(SIG_BLOCK, &signals, nullptr);

    auto waitForSignal = [this, signals]
    {
        int receivedSignal{0};
        int result = sigwait(&signals, &receivedSignal);
        shutdownCb();
    };

    signalThread.emplace(waitForSignal);
}

ShutdownHandler::~ShutdownHandler()
{
}


} // namespace networking
