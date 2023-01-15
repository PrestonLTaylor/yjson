#include "json_parser.h"
#include "json_array.h"
#include "json_boolean.h"
#include "json_null.h"
#include "json_number.h"
#include "json_object.h"
#include "json_string.h"

namespace json
{
    std::shared_ptr<value> parser::parse(const std::string& json_string)
    {
        parser parser(json_string);
        return parser.parse_json();
    }

    std::shared_ptr<value> parser::parse_json()
    {
        auto parsed_json = parse_value();
        if (has_remaining_tokens())
        {
            // TODO: Better error message for this (e.g. next tokens was 'token_type')
            throw std::runtime_error{ "unexpected tokens after initial value" };
        }

        return parsed_json;
    }

    // TODO: Use iterative approach instead of recursive descent
    std::shared_ptr<value> parser::parse_value()
    {
        // TODO: Ignore insignificant whitespace
        const auto current_token = peek_token();
        switch (current_token.type)
        {
            using enum token_type;
            case token_type::open_square_bracket:
                return parse_array();

            case token_type::open_curly_bracket:
                return parse_object();

            case token_type::quotation_mark:
                return parse_string();

            case token_type::minus:
            case token_type::number:
                return parse_number();

            case token_type::true_literal:
                consume_token();
                return std::make_shared<boolean>(true);

            case token_type::false_literal:
                consume_token();
                return std::make_shared<boolean>(false);

            case token_type::null_literal:
                consume_token();
                return std::make_shared<null>();

            default:
                throw std::runtime_error{ std::format("unexpected '{}' while trying to parse a value", current_token.data) };
        }
    }

    std::shared_ptr<array> parser::parse_array()
    {
        auto parsed_array = std::make_shared<array>();
        consume_token_of_type(token_type::open_square_bracket);
        if (is_next_token_of_type(token_type::closed_square_bracket))
        {
            consume_token();
            return parsed_array;
        }

        parsed_array->add_value(parse_value());

        while (!is_next_token_of_type(token_type::closed_square_bracket))
        {
            consume_token_of_type(token_type::comma);
            parsed_array->add_value(parse_value());
        }

        consume_token_of_type(token_type::closed_square_bracket);
        return parsed_array;
    }

    std::shared_ptr<object> parser::parse_object()
    {
        auto parsed_object = std::make_shared<object>();
        consume_token_of_type(token_type::open_curly_bracket);
        if (is_next_token_of_type(token_type::closed_curly_bracket))
        {
            consume_token();
            return parsed_object;
        }

        parse_name_value_pair(parsed_object);

        while (!is_next_token_of_type(token_type::closed_curly_bracket))
        {
            consume_token_of_type(token_type::comma);

            parse_name_value_pair(parsed_object);
        }

        consume_token_of_type(token_type::closed_curly_bracket);
        return parsed_object;
    }

    void parser::parse_name_value_pair(const std::shared_ptr<object>& object)
    {
        auto parsed_name = parse_string();
        consume_token_of_type(token_type::colon);
        auto parsed_value = parse_value();

        object->add_value(parsed_name, parsed_value);
    }

    std::shared_ptr<string> parser::parse_string()
    {
        consume_token_of_type(token_type::quotation_mark);

        std::string parsed_string;
        while (!is_next_token_of_type(token_type::quotation_mark))
        {
            if (is_next_token_of_type(token_type::reverse_solidus))
            {
                parsed_string += parse_escape_sequence();
            }
            else
            {
                parsed_string += consume_token().data;
            }
        }

        consume_token_of_type(token_type::quotation_mark);

        return std::make_shared<string>(parsed_string);
    }

    std::string parser::parse_escape_sequence()
    {
        consume_token_of_type(token_type::reverse_solidus);
        auto escape_character = consume_token().data;
        switch (escape_character.at(0))
        {
            case '"':
            case '\\':
            case '/':
                return escape_character;

            case 'b':
                return "\b";

            case 'f':
                return "\f";

            case 'n':
                return "\n";

            case 'r':
                return "\r";

            case 't':
                return "\t";

            // TODO: hexadecimal escaping
            //case 'u':

            default:
                throw std::runtime_error{ std::format("unexpected '{}' while trying to parse an escape sequence", escape_character) };
        }
    }

    std::shared_ptr<number> parser::parse_number()
    {
        const auto number_as_string = parse_number_as_string();
        const auto parsed_number = std::stod(number_as_string);
        return std::make_shared<number>(parsed_number);
    }

    std::string parser::parse_number_as_string()
    {
        const auto sign = parse_sign();
        const auto integer = parse_integer();
        const auto fractional = parse_fractional();
        const auto exponent = parse_exponent();
        return sign + integer + fractional + exponent;
    }

    std::string parser::parse_sign()
    {
        if(!is_next_token_of_type(token_type::minus))
        {
            return "";
        }

        consume_token();
        return "-";
    }

    std::string parser::parse_integer()
    {
        if (peek_token_of_type(token_type::number).data == "0")
        {
            consume_token();
            return "0";
        }

        std::string integer_as_string;
        while (is_next_token_of_type(token_type::number))
        {
            integer_as_string += consume_token().data;
        }

        return integer_as_string;
    }

    std::string parser::parse_fractional()
    {
        if (!is_next_token_of_type(token_type::dot))
        {
            return "";
        }

        consume_token();

        // TODO: Maybe abstract to parse_number_sequence
        std::string fractional_as_string = ".";
        while (is_next_token_of_type(token_type::number))
        {
            fractional_as_string += consume_token().data;
        }

        return fractional_as_string;
    }

    std::string parser::parse_exponent()
    {
        if (const auto token = peek_token(); token.data != "e" && token.data != "E")
        {
            return "";
        }

        consume_token();

        std::string exponent_as_string = "E" + parse_exponent_sign();
        while (is_next_token_of_type(token_type::number))
        {
            exponent_as_string += consume_token().data;
        }

        return exponent_as_string;
    }

    std::string parser::parse_exponent_sign()
    {
        const auto& exponent_token = peek_token();
        if (exponent_token.data == "-" || exponent_token.data == "+")
        {
            consume_token();
            return exponent_token.data;
        }

        return "";
    }
}