#include <format>
#include "json_array.h"

namespace json
{
    std::string array::to_string() const
    {
        if (m_values.empty())
        {
            return "[]";
        }

        std::string string = std::format("[{}", m_values.front()->to_string());
        for (auto i = 1U; i < m_values.size(); ++i)
        {
            string += std::format(", {}", m_values.at(i)->to_string());
        }
        string += "]";

        return string;
    }
}