#pragma once

#include "core/core.hpp"
#include "logger/logger.hpp"
#include "application/application.hpp"
#include "utils/instrumentor.hpp"

extern vektor::Application *createApplication();

int main(int argc, char **argv)
{
    vektor::logger::Logger::Init();

    VEKTOR_CORE_TRACE("Starting Vektor Engine");
    VEKTOR_CORE_CRITICAL("Vektor Engine is running");
    VEKTOR_CORE_WARN("Vektor Engine is running");

    VEKTOR_PROFILE_FUNCTION();
    {
        VEKTOR_PROFILE_BEGIN_SESSION("Startup", "VektorProfile-Runtime.json");

        vektor::Application *application = vektor::createApplication();

        application->Run();
        delete application;
    }
    VEKTOR_CORE_TRACE("Vektor Engine has stopped");
    VEKTOR_PROFILE_END_SESSION();

    return 0;
}