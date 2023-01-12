#pragma once
#include <format>
#include <string>
#include "json_value.h"

namespace json
{
    class string : public value
    {
    public:
        explicit string(const std::string& string) : m_string(string) {}

        const char* class_name() const override { return "string"; }

        // TODO: Escaping of special characters in to_json_string
        std::string to_string() const override { return std::format("\"{}\"", m_string); };

    private:
        // TODO: Might need a string capable of supporting unicode
        std::string m_string;
    };
}