#pragma once
#include <format>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>
#include "json_forward.h"
#include "json_lexer.h"
#include "json_value.h"

// TODO: [[nodiscard]] consistency around code base
// TODO: Maybe make json_exception or similar
// TODO: Make exceptions and noexcept consistent through all code base
namespace json
{
    class parser
    {
    public:
        static std::shared_ptr<value> parse(const std::string& json_string);

    private:
        parser(const std::string& json_string) : m_tokens(json::lexer::lex(json_string)), m_token_index(0) {}

        bool has_remaining_tokens() const { return m_token_index < m_tokens.size(); }

        const token& peek_token() const
        {
            if (!has_remaining_tokens())
            {
                throw std::runtime_error{ "unexpected end of file found" };
            }

            return m_tokens.at(m_token_index);
        }

        bool is_next_token_of_type(token_type type)
        {
            return has_remaining_tokens() && peek_token().type == type;
        }

        const token& peek_token_of_type(token_type type)
        {
            const auto& token = peek_token();
            if (token.type != type)
            {
                // TODO: Line and column for error
                throw std::runtime_error{ std::format("unexpected '{}' found", token.data) };
            }

            return token;
        }

        const token& consume_token()
        {
            const auto& token = peek_token();
            ++m_token_index;
            return token;
        }

        const token& consume_token_of_type(token_type type)
        {
            const auto& token = consume_token();
            if (token.type != type)
            {
                // TODO: Line and column for error
                throw std::runtime_error{ std::format("unexpected '{}' found", token.data) };
            }

            return token;
        }

        std::shared_ptr<value> parse_json();
        std::shared_ptr<value> parse_value();

        std::shared_ptr<array> parse_array();

        std::shared_ptr<object> parse_object();
        void parse_name_value_pair(const std::shared_ptr<object> &object);

        std::shared_ptr<string> parse_string();
        std::string parse_escape_sequence();

        std::shared_ptr<number> parse_number();
        std::string parse_number_as_string();
        std::string parse_sign();
        std::string parse_integer();
        std::string parse_fractional();
        std::string parse_exponent();
        std::string parse_exponent_sign();

        std::vector<token> m_tokens;
        size_t m_token_index;
    };
}