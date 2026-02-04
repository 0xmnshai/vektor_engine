#pragma once

#include <chrono>
#include <thread>
#include <string>
#include <functional>

#include "core/core.hpp"
#include "utils/profiling.hpp"

namespace vektor::utils
{
    class VEKTOR_API Timer
    {
    public:
        using Clock = std::chrono::steady_clock;

        Timer(const std::string &name, std::function<void(ProfileResult)> callback)
            : m_Name(name), m_Callback(callback), m_Running(false)
        {
        }

        ~Timer()
        {
            if (m_Running)
                stop();
        }

        void start()
        {
            m_Running = true;
            m_StartTime = Clock::now();
        }

        void stop()
        {
            auto endTime = Clock::now();

            long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTime)
                                  .time_since_epoch()
                                  .count();

            long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTime)
                                .time_since_epoch()
                                .count();

            ProfileResult result;
            result.name = m_Name;
            result.start = start;
            result.end = end;
            result.threadID = std::hash<std::thread::id>{}(std::this_thread::get_id());

            m_Running = false;

            m_Callback(result);
        }

        bool isRunning() const { return m_Running; }

    private:
        std::string m_Name;
        std::function<void(ProfileResult)> m_Callback;

        Clock::time_point m_StartTime;
        bool m_Running;
    };
}
