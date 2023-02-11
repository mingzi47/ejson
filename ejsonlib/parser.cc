#include "parser.hh"
#include <stdexcept>
#include <optional>
#include <iostream>
#include <cerrno>
#include <cstdlib>
#include <cmath>

using namespace eee;

Parser::Parser() : _tokens(nullptr), _data(), _pos(0) {
}

Parser::Parser(const std::string &data) : _tokens(data), _data(), _pos(0) {
    Parser::parse();
}

Json Parser::getValue() {
    return _data;
}

void Parser::clear() {
    _pos = 0;
    _data = nullptr;
}

void Parser::parse_whitespace() {
    while ((_pos < _tokens.size())
           && (_tokens[_pos] == '\t' || _tokens[_pos] == '\n'
               || _tokens[_pos] == '\r' || _tokens[_pos] == ' ')) {
        this->_pos++;
    }
}

Json Parser::parse_null() {
    if (_tokens.find("null", _pos) != _pos) {
        throw std::logic_error("value is not NULL ! ");
    }
    _pos += 4;
    return Json();
}

Json Parser::parse_bool(bool flag) {
    if (flag) {
        if (_tokens.find("true", _pos) != _pos) {
            throw std::logic_error("value is not TRUE ! ");
        }
        _pos += 4;
        return Json(true);
    }
    if (_tokens.find("false", _pos) != _pos) {
        throw std::logic_error("value is not FALSE ! ");
    }
    _pos += 5;
    return Json(false);
}

Json Parser::parse_number() {
    int tmp = _pos, m = _tokens.size();
    bool flag = false;
    if (_tokens[tmp] == '-') tmp++;
    if (_tokens[tmp] == '0') tmp++;
    else {
        if (_tokens[tmp] < '0' || _tokens[tmp] > '9')
            throw std::logic_error("value is not number !");
        for (; tmp < m && (_tokens[tmp] >= '0' && _tokens[tmp] <= '9'); tmp++)
            ;
    }
    if (_tokens[tmp] == '.') {
        tmp++;
        if (_tokens[tmp] < '0' || _tokens[tmp] > '9')
            throw std::logic_error("value is not number !");
        flag = true;
        for (; tmp < m && (_tokens[tmp] >= '0' && _tokens[tmp] <= '9'); tmp++)
            ;
    }
    if (_tokens[tmp] == 'e' || _tokens[tmp] == 'E') {
        tmp++;
        if (_tokens[tmp] == '+' || _tokens[tmp] == '-') tmp++;
        if (_tokens[tmp] < '0' || _tokens[tmp] > '9')
            throw std::logic_error("value is not number !");
        flag = true;
        for (; tmp < m && (_tokens[tmp] >= '0' && _tokens[tmp] <= '9'); tmp++)
            ;
    }
    errno = 0;
    double data = std::strtod(_tokens.c_str() + _pos, nullptr);
    if (errno == ERANGE && (data == HUGE_VAL || data == -HUGE_VAL))
        throw std::logic_error("value is not number !");
    _pos = tmp;
    if (flag) return Json(data);
    return Json((int)data);
}

Json Parser::parse_string() {
    std::string data;
    _pos++;
    size_t m = _tokens.size();
    for (; _pos < m; _pos++) {
        switch (_tokens[_pos]) {
            case '\\': {
                if (_pos + 1 < m) {
                    throw std::logic_error("String index failed !");
                }
                _pos++;
                switch (_tokens[_pos]) {
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
                        for (int i = 0; i < 4; i++, _pos++)
                            data.push_back(_tokens[_pos]);
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
                _pos++;
                return Json(data);
            }
            default: {
                if ((unsigned char)_tokens[_pos] < 0x20) {
                    throw std::logic_error(" string failed ! ");
                }
                data.push_back(_tokens[_pos]);
            }
        }
    }
    throw std::logic_error(" string end failed ! ");
}

Json Parser::parse_array() {
    std::vector<Json> data;
    _pos++;
    size_t m = _tokens.size();
    for (; _pos < m;) {
        Parser::parse_whitespace();

        data.emplace_back(Parser::parse_value());

        Parser::parse_whitespace();

        if (_tokens[_pos] == ']') {
            _pos++;
            return Json(data);
        } else if (_tokens[_pos] == ',') {
            _pos++;
            continue;
        } else {
            break;
        }
    }
    throw std::logic_error("array parse failed !");
}

Json Parser::parse_object() {
    std::map<std::string, Json> data;
    _pos++;
    size_t m = _tokens.size();
    for (; _pos < m;) {
        Parser::parse_whitespace();
        if (_tokens[_pos] != '"') { std::logic_error("key failed !"); }
        std::optional<std::string> tmp = Parser::parse_string().valueString();
        if (!tmp) { std::logic_error("key failed !"); }
        std::string key = tmp.value();
        Parser::parse_whitespace();

        if (_tokens[_pos] != ':') { std::logic_error(": failed (object) !"); }

        _pos++;
        data[key] = Parser::parse_value();
        Parser::parse_whitespace();

        if (_tokens[_pos] == '}') {
            _pos++;
            return Json(data);
        } else if (_tokens[_pos] == ',') {
            _pos++;
            continue;
        } else {
            break;
        }
    }
    throw std::logic_error("object parse failed !");
}

Json Parser::parse_value() {
    Parser::parse_whitespace();
    switch (_tokens[_pos]) {
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
    if (_tokens.size() > _pos) { std::logic_error("parse is failed ! "); }
    return _data = std::move(data);
}

Json Parser::parse(const std::string &data) {
    Parser::clear();
    _tokens = data;
    _pos = 0;
    return Parser::parse();
}