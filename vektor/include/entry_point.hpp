#pragma once

#include "core/core.hpp"
#include "logger/logger.hpp"
#include "application/application.hpp"
#include "utils/instrumentor.hpp"

extern vektor::Application *createApplication();

int main(int argc, char **argv)
{
    vektor::logger::Logger::Init();

    vektor::Application *application;

    {
        VEKTOR_PROFILE_BEGIN_SESSION("Startup", "VektorProfile-Startup.json");
        application = vektor::createApplication();
        VEKTOR_PROFILE_END_SESSION();
    }

    {
        VEKTOR_PROFILE_BEGIN_SESSION("Runtime", "VektorProfile-Runtime.json");
        application->Run();
        VEKTOR_PROFILE_END_SESSION();
    }

    {
        VEKTOR_PROFILE_BEGIN_SESSION("Shutdown", "VektorProfile-Shutdown.json");
        delete application;
        VEKTOR_PROFILE_END_SESSION();
    }

    VEKTOR_CORE_TRACE("Vektor Engine has stopped");
    return 0;
}