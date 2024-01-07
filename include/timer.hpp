/*
 * timer.hpp
 *
 *  Created on: Jul 29, 2017
 *      Author: nullifiedcat
 */

#pragma once

#include <chrono>

class Timer
{
public:
    std::chrono::time_point<std::chrono::system_clock> last;

    constexpr Timer() noexcept : last(std::chrono::system_clock::now()) {}

    inline bool check(unsigned int ms) const noexcept
    {
        auto now = std::chrono::system_clock::now();
        auto elapsed_ms = std::chrono::duration_cast<std::chrono::milliseconds>(now - last).count();
        return elapsed_ms >= ms;
    }

    bool test_and_set(unsigned int ms) noexcept
    {
        if (check(ms))
        {
            update();
            return true;
        }
        return false;
    }

    inline void update() noexcept
    {
        last = std::chrono::system_clock::now();
    }
};
