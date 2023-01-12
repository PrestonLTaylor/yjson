#pragma once
#include <array>
#include <string>
#include <vector>
#include "consumer.h"
#include "json_token.h"

namespace json
{
    class lexer
    {
    public:
        static std::vector<token> lex(const std::string& json_string);

    private:
        explicit lexer(const std::string& json_string) : m_consumer(json_string) {}

        static inline std::array<token, 13> lexable_tokens
        {
            token{ token_type::open_square_bracket, "[" },
            token{ token_type::open_curly_bracket, "{" },
            token{ token_type::closed_square_bracket, "]" },
            token{ token_type::closed_curly_bracket, "}" },
            token{ token_type::colon, ":" },
            token{ token_type::comma, "," },
            token{ token_type::quotation_mark, "\"" },
            token{ token_type::reverse_solidus, "\\" },
            token{ token_type::true_literal, "true" },
            token{ token_type::false_literal, "false" },
            token{ token_type::null_literal, "null" },
            token{ token_type::minus, "-" },
            token{ token_type::dot, "." }
        };

        std::vector<token> lex_json();
        token lex_next_token();

        consumer m_consumer;
    };
}