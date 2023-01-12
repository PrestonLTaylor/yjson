#pragma once
#include <memory>
#include <vector>
#include "json_value.h"

namespace json
{
    class array final : public value
    {
    public:
        const char* class_name() const override { return "array"; }

        std::string to_string() const override;

        void add_value(std::shared_ptr<value> new_value) { m_values.push_back(move(new_value)); }

    private:
        std::vector<std::shared_ptr<value>> m_values;
    };
}