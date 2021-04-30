#pragma once

#include <cstdint>
#include <atomic>

template <typename T>
struct BitTable
{
    T x[sizeof(T) * 8];

    constexpr BitTable() : x()
    {
        for (T i = 0; i < sizeof(T) * 8; i++)
        {
            x[i] = ((T)1 << i);
        }
    }

    inline const T operator()(int i) const
    {
        return x[i];
    }
};

static constexpr BitTable<uint64_t> button_lookup_table;

struct ControllerState
{
    std::atomic_uint64_t m_button_state;
    std::uint64_t m_button_state_staging;

    ControllerState();

    void start();
    void end();
    void add_button(int i);
};
