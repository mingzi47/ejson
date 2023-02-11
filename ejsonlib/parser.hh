#pragma once

#include "json.hh"
#include <cstddef>

namespace eee {

class Parser {
  private:
    std::string _tokens;
    Json _data;
    std::size_t _pos;

    void parse_whitespace();
    Json parse_value();
    Json parse_null();
    Json parse_bool(bool flag);
    Json parse_number();
    Json parse_string();
    Json parse_array();
    Json parse_object();

  public:
    Parser();
    Parser(const std::string &data);
    ~Parser() = default;

    Json parse();
    Json parse(const std::string &data);

    Json getValue();

    void clear();
};

} // namespace eee