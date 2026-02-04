#pragma once

#include <algorithm>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <string>
#include <thread>
#include <mutex>
#include <sstream>

#include "utils/profiling.hpp"
#include "logger/logger.hpp"

namespace vektor
{
    struct InstrumentationSession
    {
        std::string Name;
    };

    class Instrumentor
    {
    public:
        Instrumentor(const Instrumentor &) = delete;
        Instrumentor(Instrumentor &&) = delete;

        void beginSession(const std::string &name, const std::string &filepath = "results.json")
        {
            std::lock_guard lock(m_Mutex);
            if (m_CurrentSession)
            {
                VEKTOR_CORE_ERROR("Instrumentor::beginSession('{0}') when session '{1}' already open.", name, m_CurrentSession->Name);

                internalEndSession();
            }
            m_OutputStream.open(filepath);

            if (m_OutputStream.is_open())
            {
                m_CurrentSession = new InstrumentationSession({name});
                writeHeader();
            }
            else
            {
                VEKTOR_CORE_ERROR("Instrumentor could not open results file '{0}'.", filepath);
            }
        }

        void endSession()
        {
            std::lock_guard lock(m_Mutex);
            internalEndSession();
        }

        void writeProfile(const utils::ProfileResult &result)
        {
            std::stringstream json;

            auto tid = std::hash<std::thread::id>{}(result.threadID);

            long long duration = result.end - result.start;

            json << std::setprecision(3) << std::fixed;
            json << ",{";
            json << "\"cat\":\"function\",";
            json << "\"dur\":" << duration << ",";
            json << "\"name\":\"" << result.name << "\",";
            json << "\"ph\":\"X\",";
            json << "\"pid\":0,";
            json << "\"tid\":" << tid << ",";
            json << "\"ts\":" << result.start;
            json << "}";

            std::lock_guard<std::mutex> lock(m_Mutex);
            if (m_CurrentSession)
            {
                m_OutputStream << json.str();
                m_OutputStream.flush();
                std::cout << "[Profiler] Wrote: " << result.name << std::endl;
            }
        }

        static Instrumentor &Get()
        {
            static Instrumentor instance;
            return instance;
        }

    private:
        Instrumentor()
            : m_CurrentSession(nullptr)
        {
        }

        ~Instrumentor()
        {
            endSession();
        }

        void writeHeader()
        {
            m_OutputStream << "{\"otherData\": {},\"traceEvents\":[";
            m_OutputStream.flush();
        }

        void writeFooter()
        {
            m_OutputStream << "]}";
            m_OutputStream.flush();
        }

        void internalEndSession()
        {
            if (m_CurrentSession)
            {
                writeFooter();
                m_OutputStream.close();
                delete m_CurrentSession;
                m_CurrentSession = nullptr;
            }
        }

    private:
        std::mutex m_Mutex;
        InstrumentationSession *m_CurrentSession;
        std::ofstream m_OutputStream;
    };

    class InstrumentationTimer
    {
    public:
        InstrumentationTimer(const char *name)
            : m_Name(name), m_Stopped(false)
        {
            m_StartTimepoint = std::chrono::steady_clock::now();
        }

        ~InstrumentationTimer()
        {
            if (!m_Stopped)
                stop();
        }

        void stop()
        {
            std::chrono::time_point<std::chrono::steady_clock> endTimepoint = std::chrono::steady_clock::now();
            long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();
            long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

            long long elapsedTime = end - start;
            Instrumentor::Get().writeProfile({m_Name, start, end, std::this_thread::get_id()});
            m_Stopped = true;
        }

    private:
        const char *m_Name;
        std::chrono::time_point<std::chrono::steady_clock> m_StartTimepoint;
        bool m_Stopped;
    };

    namespace InstrumentorUtils
    {

        template <size_t N>
        struct ChangeResult
        {
            char Data[N];
        };

        template <size_t N, size_t K>
        constexpr auto CleanupOutputString(const char (&expr)[N], const char (&remove)[K])
        {
            ChangeResult<N> result = {};

            size_t srcIndex = 0;
            size_t dstIndex = 0;
            while (srcIndex < N)
            {
                size_t matchIndex = 0;
                while (matchIndex < K - 1 && srcIndex + matchIndex < N - 1 && expr[srcIndex + matchIndex] == remove[matchIndex])
                    matchIndex++;
                if (matchIndex == K - 1)
                    srcIndex += matchIndex;
                result.Data[dstIndex++] = expr[srcIndex] == '"' ? '\'' : expr[srcIndex];
                srcIndex++;
            }
            return result;
        }
    }
}

#ifndef VEKTOR_PROFILE
#define VEKTOR_PROFILE 1
#endif

#if VEKTOR_PROFILE

#if defined(__GNUC__) || (defined(__MWERKS__) && (__MWERKS__ >= 0x3000)) || (defined(__ICC) && (__ICC >= 600)) || defined(__ghs__)
#define VEKTOR_FUNC_SIG __PRETTY_FUNCTION__
#elif defined(__DMC__) && (__DMC__ >= 0x810)
#define VEKTOR_FUNC_SIG __PRETTY_FUNCTION__
#elif (defined(__FUNCSIG__) || (_MSC_VER))
#define VEKTOR_FUNC_SIG __FUNCSIG__
#elif (defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 600)) || (defined(__IBMCPP__) && (__IBMCPP__ >= 500))
#define VEKTOR_FUNC_SIG __FUNCTION__
#elif defined(__BORLANDC__) && (__BORLANDC__ >= 0x550)
#define VEKTOR_FUNC_SIG __FUNC__
#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901)
#define VEKTOR_FUNC_SIG __func__
#elif defined(__cplusplus) && (__cplusplus >= 201103)
#define VEKTOR_FUNC_SIG __func__
#else
#define VEKTOR_FUNC_SIG "VEKTOR_FUNC_SIG unknown!"
#endif

#define VEKTOR_PROFILE_BEGIN_SESSION(name, filepath) ::vektor::Instrumentor::Get().beginSession(name, filepath)
#define VEKTOR_PROFILE_END_SESSION() ::vektor::Instrumentor::Get().endSession()
#define VEKTOR_PROFILE_SCOPE_LINE2(name, line)                                                           \
    constexpr auto fixedName##line = ::vektor::InstrumentorUtils::CleanupOutputString(name, "__cdecl "); \
    ::vektor::InstrumentationTimer timer##line(fixedName##line.Data)
#define VEKTOR_PROFILE_SCOPE_LINE(name, line) VEKTOR_PROFILE_SCOPE_LINE2(name, line)
#define VEKTOR_PROFILE_SCOPE(name) VEKTOR_PROFILE_SCOPE_LINE(name, __LINE__)
#define VEKTOR_PROFILE_FUNCTION() VEKTOR_PROFILE_SCOPE(VEKTOR_FUNC_SIG)
#else
#define VEKTOR_PROFILE_BEGIN_SESSION(name, filepath)
#define VEKTOR_PROFILE_END_SESSION()
#define VEKTOR_PROFILE_SCOPE(name)
#define VEKTOR_PROFILE_FUNCTION()
#endif

#undef VEKTOR_PROFILE
#define VEKTOR_PROFILE 1