#pragma once
#include <string>

namespace json
{
    class value
    {
    public:
        virtual const char* class_name() const { return "value"; }

        // TODO: ASSERT_NOT_REACHED or similar
        virtual std::string to_string() const { return ""; };
    };
}