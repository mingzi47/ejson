#pragma once

#include "json.hh"
#include <cstddef>

namespace eee {

class Parser {
  public:
    Parser();
    Parser(std::string data);
    ~Parser() = default;

    Json parse();
    Json parse(std::string data);

    Json getValue();

    void clear();

  private:
    void parse_whitespace();
    Json parse_value();
    Json parse_null();
    Json parse_bool(bool flag);
    Json parse_number();
    Json parse_string();
    Json parse_array();
    Json parse_object();

  private:
    std::string tokens;
    Json json_data;
    std::size_t pos;
};

} // namespace eee