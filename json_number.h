#pragma once
#include <string>
#include "json_value.h"

namespace json
{
    class number : public value
    {
    public:
        explicit number(double number) : m_number(number) {}

        const char* class_name() const override { return "number"; }

        std::string to_string() const override;

    private:
        double m_number;
    };
};