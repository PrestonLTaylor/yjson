#pragma once
#include "json_value.h"

namespace json
{
    class boolean : public value
    {
    public:
        explicit boolean(bool boolean) : m_boolean(boolean) {}

        const char* class_name() const override { return "boolean"; }

        std::string to_string() const override;

    private:
        bool m_boolean;
    };
}