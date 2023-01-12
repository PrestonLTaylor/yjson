#include <format>
#include "json_object.h"

namespace json
{
    std::string object::to_string() const
    {
        if (m_names_to_values.empty())
        {
            return "{}";
        }

        const auto& front = *m_names_to_values.begin();
        std::string string = std::format(R"({{"{}" : {})", front.first->to_string(), front.second->to_string());
        for (auto itr = next(begin(m_names_to_values)); itr != end(m_names_to_values); ++itr)
        {
            string += std::format(R"(, {} : {})", itr->first->to_string(), itr->second->to_string());
        }
        string += "}";

        return string;
    }
}