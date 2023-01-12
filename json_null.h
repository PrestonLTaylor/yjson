#pragma once
#include "json_value.h"

namespace json
{
    class null : public value
    {
    public:
        const char* class_name() const override { return "null"; };
        std::string to_string() const override { return "null"; };
    };
}