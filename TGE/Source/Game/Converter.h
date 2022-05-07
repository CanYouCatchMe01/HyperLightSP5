#pragma once
#include "json.hpp"
#include "tga2d/math/Vector3.h"
#include "MeshComponent.h"
#include "tga2d/math/color.h"

namespace nlohmann
{
    template <class T>
    struct adl_serializer<Tga2D::Vector3<T>>
    {
        static Tga2D::Vector3<T> from_json(const json& j)
        {
            return 
            {
                j["x"].get<T>(),
                j["y"].get<T>(),
                j["z"].get<T>()
            };
        }
    };

    template <>
    struct adl_serializer<Tga2D::Color>
    {
        static Tga2D::Color from_json(const json& j)
        {
            return
            {
                j["r"].get<float>(),
                j["g"].get<float>(),
                j["b"].get<float>(),
				j["a"].get<float>()
            };
        }
    };
}