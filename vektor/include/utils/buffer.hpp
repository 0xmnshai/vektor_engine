#pragma once

#include <vector>
#include "core/core.hpp" 

namespace vektor::utils::buffer
{
    class VEKTOR_API Vertex
    {
    public:
        virtual ~Vertex() = default;

        virtual void bind() const = 0;
        virtual void unBind() const = 0;

        static Vertex *create(const std::vector<float> &vertices);
    };

    class VEKTOR_API Index
    {
    public:
        virtual ~Index() = default;

        virtual void bind() const = 0;
        virtual void unBind() const = 0;

        virtual uint32_t getCount() const = 0;

        static Index *create(const std::vector<uint32_t> &indices);
    };
}