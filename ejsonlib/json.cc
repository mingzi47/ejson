#include <cstdint>
#include <iostream>
#include <memory>
#include <optional>
#include <ostream>
#include <stdexcept>
#include <string>
#include <variant>

#include "json.hh"
using namespace eee;

using std::string;
using array = std::vector<Json>;
using object = std::map<std::string, Json>;

using str_ptr = std::unique_ptr<string>;
using arr_ptr = std::unique_ptr<array>;
using obj_ptr = std::unique_ptr<object>;

Json::Json() : _type{Type::JSON_NULL}, _value{nullptr} {
}

Json::Json(Type type) : _type{type} {
    switch (_type) {
        case Type::JSON_NULL:
            _value = nullptr;
            break;
        case Type::JSON_BOOL:
            _value = false;
            break;
        case Type::JSON_INT:
            _value = 0;
            break;
        case Type::JSON_DOUBLE:
            _value = 0.0;
            break;
        case Type::JSON_STRING:
            _value = std::make_unique<string>();
            break;
        case Type::JSON_ARRAY:
            _value = std::make_unique<array>();
            break;
        case Type::JSON_OBJECT:
            _value = std::make_unique<object>();
            break;
    }
}

Json::Json(std::nullptr_t value) : _type{Type::JSON_NULL}, _value{value} {
}

Json::Json(const bool value) : _type{Type::JSON_BOOL}, _value{value} {
}

Json::Json(const int value) : _type{Type::JSON_INT}, _value{value} {
}

Json::Json(const double value) : _type{Type::JSON_DOUBLE}, _value{value} {
}

Json::Json(const char *value)
    : _type{Type::JSON_STRING}, _value{std::make_unique<std::string>(value)} {
}

Json::Json(const string &value)
    : _type{Type::JSON_STRING}, _value{std::make_unique<string>(value)} {
}

Json::Json(const std::vector<Json> &value)
    : _type{Type::JSON_ARRAY}, _value(std::make_unique<array>(value)) {
}

Json::Json(const std::map<std::string, Json> &value)
    : _type(Type::JSON_OBJECT), _value(std::make_unique<object>(value)) {
}

Json::Json(const Json &value) {
    copy(value);
}

Json::Json(Json &&other) noexcept {
    clear();
    other._value.swap(_value);
    _type = other._type;
}

Json::~Json() {
    _value = nullptr;
    _type = Type::JSON_NULL;
}

Json &Json::operator=(Json const &other) {
    this->copy(other);
    return *this;
}

Json &Json::operator=(Json &&other) noexcept {
    clear();
    other._value.swap(_value);
    _type = other._type;
    return *this;
}

void Json::copy(const Json &other) {
    clear();
    switch (other._type) {
        case Type::JSON_STRING:
            _value = std::make_unique<string>(*std::get<str_ptr>(other._value));
            break;
        case Type::JSON_ARRAY:
            _value = std::make_unique<array>(*std::get<arr_ptr>(other._value));
            break;
        case Type::JSON_OBJECT:
            _value = std::make_unique<object>(*std::get<obj_ptr>(other._value));
            break;
        default:
            break;
    }
    _type = other._type;
}

void Json::clear() {
    this->~Json();
    _value = nullptr;
    _type = Type::JSON_NULL;
}

void Json::swap(Json &other) {
    other._value.swap(_value);
    std::swap(other._type, _type);
}

void Json::operator=(std::nullptr_t value) {
    clear();
    copy(Json{value});
}

void Json::operator=(const bool value) {
    clear();
    copy(Json{value});
}

void Json::operator=(const int value) {
    clear();
    copy(Json{value});
}

void Json::operator=(const double value) {
    clear();
    copy(Json{value});
}

void Json::operator=(const char *value) {
    clear();
    copy(Json{value});
}

void Json::operator=(const string &value) {
    clear();
    copy(Json{value});
}

void Json::operator=(const array &value) {
    clear();
    copy(Json{value});
}

void Json::operator=(const object &value) {
    clear();
    copy(Json{value});
}

bool Json::equal(const Json &other) const {
    if (_type != other._type) { return false; }
    switch (_type) {
        case Type::JSON_STRING:
            return *(std::get<str_ptr>(_value))
                   == *(std::get<str_ptr>(other._value));
        case Type::JSON_ARRAY:
            return *(std::get<arr_ptr>(_value))
                   == *(std::get<arr_ptr>(other._value));
        case Type::JSON_OBJECT:
            return *(std::get<obj_ptr>(_value))
                   == *(std::get<obj_ptr>(other._value));
        default:
            return _value == other._value;
    }
    return false;
}

bool Json::operator==(const Json &other) const {
    return equal(other);
}

bool Json::operator!=(const Json &other) const {
    return !equal(other);
}

Type Json::type() const {
    return _type;
}

bool Json::isNull() const {
    return _type == Type::JSON_NULL;
}

bool Json::isBool() const {
    return _type == Type::JSON_BOOL;
}

bool Json::isInt() const {
    return _type == Type::JSON_INT;
}

bool Json::isDouble() const {
    return _type == Type::JSON_DOUBLE;
}

bool Json::isString() const {
    return _type == Type::JSON_STRING;
}

bool Json::isArray() const {
    return _type == Type::JSON_ARRAY;
}

bool Json::isObject() const {
    return _type == Type::JSON_OBJECT;
}

std::optional<bool> Json::valueBool() const {
    if (_type == Type::JSON_BOOL) { return std::get<bool>(_value); }
    return std::nullopt;
}

std::optional<int> Json::valueInt() const {
    if (_type == Type::JSON_INT) { return std::get<int>(_value); }
    return std::nullopt;
}

std::optional<double> Json::valueDouble() const {
    if (_type == Type::JSON_DOUBLE) { return std::get<double>(_value); }
    return std::nullopt;
}

std::optional<std::string> Json::valueString() const {
    if (_type == Type::JSON_STRING) { return *(std::get<str_ptr>(_value)); }
    return std::nullopt;
}

std::optional<array> Json::valueArray() const {
    if (_type == Type::JSON_ARRAY) { return *(std::get<arr_ptr>(_value)); }
    return std::nullopt;
}

std::optional<object> Json::valueObject() const {
    if (_type == Type::JSON_OBJECT) { return *(std::get<obj_ptr>(_value)); }
    return std::nullopt;
}

std::optional<std::size_t> Json::length() const {
    if (_type == Type::JSON_STRING) {
        return std::get<str_ptr>(_value)->length();
    }
    return std::nullopt;
}

std::optional<std::size_t> Json::size() const {
    switch (_type) {
        case Type::JSON_STRING:
            return std::get<str_ptr>(_value)->size();
        case Type::JSON_ARRAY:
            return std::get<arr_ptr>(_value)->size();
        case Type::JSON_OBJECT:
            return std::get<obj_ptr>(_value)->size();
    }
    return std::nullopt;
}

Json &Json::operator[](const std::size_t index) const {
    if (_type == Type::JSON_ARRAY) {
        return std::get<arr_ptr>(_value)->at(index);
    }
    throw std::logic_error("operator[](size_t index) => error");
}

Json &Json::operator[](const char *key) const {
    return (*this)[std::string(key)];
}

Json &Json::operator[](const std::string &key) const {
    if (_type == Type::JSON_OBJECT) {
        return std::get<obj_ptr>(_value)->at(key);
    }
    throw std::logic_error("operator[](const std::stirng& key) => error");
}

void Json::push_back(const Json &other) {
    if (_type != Type::JSON_ARRAY) {
        throw std::logic_error("push_back(const Json& otehr) => type error");
    }
    std::get<arr_ptr>(_value)->push_back(other);
}

void Json::insert(std::pair<const char *, Json> k_v) {
    if (_type != Type::JSON_STRING) {
        throw std::logic_error(
            "insert(std::pair<const char*, Json> k_v) => type error");
    }
    std::get<obj_ptr>(_value)->insert({std::string(k_v.first), k_v.second});
}

void Json::insert(std::pair<const std::string &, Json> k_v) {
    if (_type != Type::JSON_OBJECT) {
        throw std::logic_error(
            "insert(std::pair<const char*, Json> k_v) => type error");
    }
    std::get<obj_ptr>(_value)->insert(k_v);
}

void Json::erase(const std::size_t index) {
    if (_type != Type::JSON_ARRAY) {
        throw std::logic_error("erase(const std::size_t index) => type error");
    }
    auto sz = std::get<arr_ptr>(_value)->size();
    if (sz <= index) {
        throw std::logic_error("erase(std::size_t index) => index error");
    }
    std::get<arr_ptr>(_value)->erase(
        std::get<arr_ptr>(_value)->begin() + index);
}

void Json::erase(const char *key) {
    erase(std::string{key});
}

void Json::erase(const std::string &key) {
    if (_type != Type::JSON_OBJECT) {
        throw std::logic_error("erase(const std::string key) => type error");
    }
    std::get<obj_ptr>(_value)->erase(key);
}

bool Json::empty() const {
    switch (_type) {
        case Type::JSON_ARRAY:
            return std::get<arr_ptr>(_value)->empty();
        case Type::JSON_OBJECT:
            return std::get<obj_ptr>(_value)->empty();
        default:
            throw std::logic_error("type is not array, object!");
    }
}

std::map<std::string, Json>::iterator Json::find(const char *key) const {
    return find(std::string(key));
}

std::map<std::string, Json>::iterator Json::find(const std::string &key) const {
    if (_type == Type::JSON_OBJECT) {
        throw std::logic_error("type is not array, object!");
    }
    return std::get<obj_ptr>(_value)->find(key);
}

std::string Json::stringify() const {
    return Json::pstringify(0, false);
}

std::string Json::fmtStringify() const {
    return Json::pstringify(0, true);
}

std::string Json::pstringify(std::size_t spaceNum, bool isFmt) const {
    string ret{};
    switch (_type) {
        case Type::JSON_NULL:
            ret += "null";
            break;
        case Type::JSON_BOOL:
            ret += (std::get<bool>(_value) ? "true" : "false");
            break;
        case Type::JSON_INT:
            ret += std::to_string(std::get<int>(_value));
            break;
        case Type::JSON_DOUBLE:
            ret += std::to_string(std::get<double>(_value));
            break;
        case Type::JSON_STRING:
            if (isFmt) { ret += "\""; }
            ret += std::string_view{*std::get<str_ptr>(_value)};
            if (isFmt) { ret += "\""; }
            break;
        case Type::JSON_ARRAY: {
            ret += "[";
            if (isFmt) { ret += "\n"; }
            size_t i = 0;
            for (const auto &value : *std::get<arr_ptr>(_value)) {
                if (i != 0) {
                    ret += ',';
                    if (isFmt) { ret += '\n'; }
                }
                i++;
                if (isFmt) { ret += std::string(spaceNum + 2, ' '); }
                ret += value.pstringify(spaceNum + 2, isFmt);
            }
            if (isFmt) { ret += "\n"; }
            if (isFmt) { ret += std::string(spaceNum, ' '); }
            ret += "]";
            break;
        }
        case Type::JSON_OBJECT: {
            if (isFmt) { ret += "{\n"; }
            size_t i = 0;
            for (const auto [k, v] : *std::get<obj_ptr>(_value)) {
                if (i != 0) {
                    ret += ',';
                    if (isFmt) { ret += '\n'; }
                }
                if (isFmt) { ret += std::string(spaceNum + 2, ' '); }
                ret += "\"";
                ret += k;
                ret += "\" : ";
                ret += v.pstringify(spaceNum + 7 + k.size(), isFmt);
                i++;
            }
            if (isFmt) { ret += '\n'; }
            if (isFmt) { ret += std::string(spaceNum, ' '); }
            ret += "}";
            break;
        }
    }
    return ret;
}