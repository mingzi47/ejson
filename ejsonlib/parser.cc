#pragma once
#include "parser.hh"
#include <stdexcept>
#include <optional>
#include <iostream>
#include <cerrno>
#include <cstdlib>
#include <cmath>

using namespace eee;

Parser::Parser() : tokens(nullptr), json_data(), pos(0) {
}

Parser::Parser(std::string data) : tokens(data), json_data(), pos(0) {
    Parser::parse();
}

Json Parser::getValue() {
    return json_data;
}

void Parser::clear() {
    pos = 0;
    json_data = nullptr;
}

void Parser::parse_whitespace() {
    while ((pos < tokens.size())
           && (tokens[pos] == '\t' || tokens[pos] == '\n' || tokens[pos] == '\r'
               || tokens[pos] == ' ')) {
        this->pos++;
    }
}

Json Parser::parse_null() {
    if (tokens.find("null", pos) != pos) {
        throw std::logic_error("value is not NULL ! ");
    }
    pos += 4;
    return Json();
}

Json Parser::parse_bool(bool flag) {
    if (flag) {
        if (tokens.find("true", pos) != pos) {
            throw std::logic_error("value is not TRUE ! ");
        }
        pos += 4;
        return Json(true);
    }
    if (tokens.find("false", pos) != pos) {
        throw std::logic_error("value is not FALSE ! ");
    }
    pos += 5;
    return Json(false);
}

Json Parser::parse_number() {
    int tmp = pos, m = tokens.size();
    bool flag = false;
    if (tokens[tmp] == '-') tmp++;
    if (tokens[tmp] == '0') tmp++;
    else {
        if (tokens[tmp] < '0' || tokens[tmp] > '9')
            throw std::logic_error("value is not number !");
        for (; tmp < m && (tokens[tmp] >= '0' && tokens[tmp] <= '9'); tmp++)
            ;
    }
    if (tokens[tmp] == '.') {
        tmp++;
        if (tokens[tmp] < '0' || tokens[tmp] > '9')
            throw std::logic_error("value is not number !");
        flag = true;
        for (; tmp < m && (tokens[tmp] >= '0' && tokens[tmp] <= '9'); tmp++)
            ;
    }
    if (tokens[tmp] == 'e' || tokens[tmp] == 'E') {
        tmp++;
        if (tokens[tmp] == '+' || tokens[tmp] == '-') tmp++;
        if (tokens[tmp] < '0' || tokens[tmp] > '9')
            throw std::logic_error("value is not number !");
        flag = true;
        for (; tmp < m && (tokens[tmp] >= '0' && tokens[tmp] <= '9'); tmp++)
            ;
    }
    errno = 0;
    double data = std::strtod(tokens.c_str() + pos, nullptr);
    if (errno == ERANGE && (data == HUGE_VAL || data == -HUGE_VAL))
        throw std::logic_error("value is not number !");
    pos = tmp;
    if (flag) return Json(data);
    return Json((int)data);
}

Json Parser::parse_string() {
    std::string data;
    pos++;
    size_t m = tokens.size();
    for (; pos < m; pos++) {
        switch (tokens[pos]) {
            case '\\': {
                if (pos + 1 < m) {
                    throw std::logic_error("String index failed !");
                }
                pos++;
                switch (tokens[pos]) {
                    case '\"':
                        data.push_back('\"');
                        break;
                    case '\\':
                        data.push_back('\\');
                        break;
                    case 't':
                        data.push_back('\t');
                        break;
                    case '/':
                        data.push_back('/');
                        break;
                    case 'b':
                        data.push_back('\b');
                        break;
                    case 'f':
                        data.push_back('\f');
                        break;
                    case 'n':
                        data.push_back('\n');
                        break;
                    case 'r':
                        data.push_back('\r');
                        break;
                    case 'u': {
                        for (int i = 0; i < 4; i++, pos++)
                            data.push_back(tokens[pos]);
                        break;
                    }
                    default:
                        throw std::logic_error(" string \\ failed !");
                }
                break;
            }
            case '\0': {
                throw std::logic_error(" string \\0 failed ! ");
            }
            case '\"': {
                pos++;
                return Json(data);
            }
            default: {
                if ((unsigned char)tokens[pos] < 0x20) {
                    throw std::logic_error(" string failed ! ");
                }
                data.push_back(tokens[pos]);
            }
        }
    }
    throw std::logic_error(" string end failed ! ");
}

Json Parser::parse_array() {
    std::vector<Json> data;
    pos++;
    size_t m = tokens.size();
    for (; pos < m;) {
        Parser::parse_whitespace();

        data.emplace_back(Parser::parse_value());

        Parser::parse_whitespace();

        if (tokens[pos] == ']') {
            pos++;
            return Json(data);
        } else if (tokens[pos] == ',') {
            pos++;
            continue;
        } else {
            break;
        }
    }
    throw std::logic_error("array parse failed !");
}

Json Parser::parse_object() {
    std::map<std::string, Json> data;
    pos++;
    size_t m = tokens.size();
    for (; pos < m;) {
        Parser::parse_whitespace();
        if (tokens[pos] != '"') { std::logic_error("key failed !"); }
        std::optional<std::string> tmp = Parser::parse_string().valueString();
        if (!tmp) { std::logic_error("key failed !"); }
        std::string key = tmp.value();
        Parser::parse_whitespace();

        if (tokens[pos] != ':') { std::logic_error(": failed (object) !"); }

        pos++;
        data[key] = Parser::parse_value();
        Parser::parse_whitespace();

        if (tokens[pos] == '}') {
            pos++;
            return Json(data);
        } else if (tokens[pos] == ',') {
            pos++;
            continue;
        } else {
            break;
        }
    }
    throw std::logic_error("object parse failed !");
}

Json Parser::parse_value() {
    Parser::parse_whitespace();
    switch (tokens[pos]) {
        case 'n':
            return parse_null();
        case 't':
            return parse_bool(true);
        case 'f':
            return parse_bool(false);
        case '"':
            return parse_string();
        case '[':
            return parse_array();
        case '{':
            return parse_object();
        default:
            return parse_number();
    }
    return Json();
}

Json Parser::parse() {
    Json data = Parser::parse_value();
    Parser::parse_whitespace();
    if (tokens.size() > pos) { std::logic_error("parse is failed ! "); }
    return json_data = std::move(data);
}

Json Parser::parse(std::string data) {
    Parser::clear();
    tokens = data;
    pos = 0;
    return Parser::parse();
}