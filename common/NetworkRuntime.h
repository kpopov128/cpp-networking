#pragma once

namespace networking
{

class NetworkRuntime
{
public:
    NetworkRuntime();
    ~NetworkRuntime();

    NetworkRuntime(const NetworkRuntime&) = delete;
    NetworkRuntime& operator=(const NetworkRuntime&) = delete;

    NetworkRuntime(NetworkRuntime&&) = delete;
    NetworkRuntime& operator=(NetworkRuntime&&) = delete;
};

}
