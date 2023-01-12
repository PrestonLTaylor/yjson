#pragma once
#include <map>
#include <memory>
#include "json_string.h"
#include "json_value.h"

namespace json
{
    class object : public value
    {
    public:
        const char* class_name() const override { return "object"; }

        std::string to_string() const override;

        void add_value(std::shared_ptr<string> name, std::shared_ptr<value> value) { m_names_to_values[move(name)] = move(value); }

    private:
        // TODO: "does not require that name strings be unique", so might have to use a vector instead of a map
        std::map<std::shared_ptr<string>, std::shared_ptr<value>> m_names_to_values;
    };
}