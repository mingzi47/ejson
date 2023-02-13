#pragma once

#include "json.hh"
#include <cstddef>

namespace eee {

//! \brief The Parser part of a JSON Lib implementation.

class Parser {
  private:
    //! \brief The JSON data that needs to be parsed
    std::string _tokens;
    //! \brief Parsed JSON data
    Json _data;
    //! \brief A _tokens that indicates which subscript of the tokens is
    //! resolved
    std::size_t _pos;

    //! \brief Parse white space
    void parse_whitespace();
    //! \brief Parse the function that startsResolve
    Json parse_value();
    //! \brief Parse null
    Json parse_null();
    //! \brief Parse bool
    //! \param flag 'true' if value of JSON true.
    Json parse_bool(bool flag);
    //! \brief Parse number
    Json parse_number();
    //! \brief Parse String
    Json parse_string();
    //! \brief Parse array
    Json parse_array();
    //! \brief Parse object
    Json parse_object();

  public:
    Parser();
    //! \brief Construct a Parser from String. And parse it.
    //! \param data A string that stores JSON data.
    Parser(const std::string &data);
    ~Parser() = default;

    //! \brief parse _tokens
    //! \return Returns the parsed data structure
    Json parse();
    //! \brief parse tokens
    //! \param data Assign a value to the _tokens
    //! \return Returns the parsed data structure
    Json parse(const std::string &data);

    //! \brief Get the parsed data structure
    //! \return Returns the parsed data structure
    Json getValue();

    void clear();
};

} // namespace eee