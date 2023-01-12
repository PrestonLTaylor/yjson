#include "json_boolean.h"

namespace json
{
    std::string boolean::to_string() const
    {
        return m_boolean ? "true" : "false";
    }
}