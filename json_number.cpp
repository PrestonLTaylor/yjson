#include <format>
#include "json_number.h"

namespace json
{
    std::string number::to_string() const
    {
        return std::format("{:g}", m_number);
    }
}

