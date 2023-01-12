#pragma once
#include <string>

namespace json
{
    enum class token_type
    {
        open_square_bracket,
        open_curly_bracket,
        closed_square_bracket,
        closed_curly_bracket,
        colon,
        comma,
        quotation_mark,
        reverse_solidus,
        true_literal,
        false_literal,
        null_literal,
        minus,
        number,
        dot,
        data,
    };

    struct token
    {
        token_type type;
        std::string data;
    };
}