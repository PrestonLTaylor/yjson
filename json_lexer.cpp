#include "json_lexer.h"

namespace json
{
    std::vector<token> lexer::lex(const std::string& json_string)
    {
        lexer lexer(json_string);
        return lexer.lex_json();
    }

    std::vector<token> lexer::lex_json()
    {
        std::vector<token> tokens;
        while (!m_consumer.eof())
        {
            tokens.push_back(lex_next_token());
        }

        return tokens;
    }

    token lexer::lex_next_token()
    {
        for (const auto& lexable_token : lexable_tokens)
        {
            const auto token_size = lexable_token.data.size();
            const auto view = m_consumer.peek(token_size);
            if (view == lexable_token.data)
            {
                m_consumer.advance(token_size);
                return lexable_token;
            }
        }

        const auto consumed = std::string(m_consumer.consume(1));
        if (std::isdigit(static_cast<unsigned char>(consumed.at(0))))
        {
            return token{ token_type::number, consumed };
        }

        return token{ token_type::data, consumed };
    }
}