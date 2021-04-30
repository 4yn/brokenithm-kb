#pragma once

#include <memory>

#include "ControllerState.hpp"

struct BrokenithmServer
{
    struct Impl;
    std::unique_ptr<Impl> m_impl;

    BrokenithmServer(int port);
    ~BrokenithmServer();

    void start_server();
    void stop_server();

    uint64_t get_controller_state();
};
