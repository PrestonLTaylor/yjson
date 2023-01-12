#include <iostream>
#include "json_parser.h"

int main()
{
    // TODO: Convert https://github.com/nst/JSONTestSuite/tree/master/test_parsing to automatic tests
    const auto parsed_json = json::parser::parse(R"("\"\\\/")");
    std::cout << parsed_json->to_string() << '\n';

    return 0;
}