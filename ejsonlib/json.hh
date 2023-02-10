#pragma once
#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>
#include <map>
#include <variant>
#include <optional>
#include <memory>

namespace eee {

enum class Type {
    JSON_NULL,
    JSON_BOOL,
    JSON_INT,
    JSON_DOUBLE,
    JSON_STRING,
    JSON_ARRAY,
    JSON_OBJECT
};

class Json {
  private:
    std::variant<
        std::nullptr_t,
        bool,
        int,
        double,
        std::unique_ptr<std::string>,
        std::unique_ptr<std::vector<Json>>,
        std::unique_ptr<std::map<std::string, Json>>>
        _value;
    Type _type;

    std::string pstringify(std::size_t spaceNum = 0, bool isFmt = false) const;
    void copy(const Json &json);

  public:
    explicit Json();
    explicit Json(Type type);
    explicit Json(std::nullptr_t value);
    explicit Json(const bool value);
    explicit Json(const int value);
    explicit Json(const double value);
    explicit Json(const char *value);
    explicit Json(const std::string &value);
    explicit Json(const std::vector<Json> &value);
    explicit Json(const std::map<std::string, Json> &value);

    // 深拷贝
    Json(const Json &other);                // 拷贝构造
    Json(Json &&other) noexcept;            // 移动构造
    Json &operator=(const Json &other);     // 拷贝赋值
    Json &operator=(Json &&other) noexcept; // 移动赋值
    ~Json();

    void swap(Json &other);
    void clear();

    void operator=(std::nullptr_t value);
    void operator=(const bool value);
    void operator=(const int value);
    void operator=(const double value);
    void operator=(const char *value);
    void operator=(const std::string &value);
    void operator=(const std::vector<Json> &value);
    void operator=(const std::map<std::string, Json> &value);

    bool equal(const Json &other) const;
    bool operator==(const Json &other) const;
    bool operator!=(const Json &other) const;

    Type type() const;
    bool isNull() const;
    bool isBool() const;
    bool isInt() const;
    bool isDouble() const;
    bool isString() const;
    bool isArray() const;
    bool isObject() const;

    std::optional<bool> valueBool() const;
    std::optional<int> valueInt() const;
    std::optional<double> valueDouble() const;
    std::optional<std::string> valueString() const;
    std::optional<std::vector<Json>> valueArray() const;
    std::optional<std::map<std::string, Json>> valueObject() const;

    std::optional<std::size_t> length() const; // string
    std::optional<std::size_t> size() const;   // string, array, object

    Json &operator[](const std::size_t index) const; // array
    Json &operator[](const char *key) const;         // object
    Json &operator[](const std::string &key) const;  // object

    void push_back(const Json &other);                     // array
    void insert(std::pair<const char *, Json> k_v);        // object
    void insert(std::pair<const std::string &, Json> k_v); // object

    void erase(const std::size_t index); // array
    void erase(const char *key);         // object
    void erase(const std::string &key);  // object

    bool empty() const; // object, array
    std::map<std::string, Json>::iterator find(const char *key) const; // object
    std::map<std::string, Json>::iterator
    find(const std::string &key) const; // object

    std::string stringify() const;
    std::string fmtStringify() const;
    friend std::ostream &operator<<(std::ostream &a, Json &b) {
        return a << b.stringify();
    }
};

} // namespace eee