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

//! \brief The type of JSON data.
enum class Type {
    JSON_NULL,
    JSON_BOOL,
    JSON_INT,
    JSON_DOUBLE,
    JSON_STRING,
    JSON_ARRAY,
    JSON_OBJECT
};

//! \brief The container part of a JSON Lib implementation.
class Json {
  private:
    //! Data structure for save the JSON data.
    std::variant<
        std::nullptr_t,
        bool,
        int,
        double,
        std::unique_ptr<std::string>,
        std::unique_ptr<std::vector<Json>>,
        std::unique_ptr<std::map<std::string, Json>>>
        _value;
    //! Type of JSON.
    Type _type;

    //! \brief Convert JSON data structures to strings. This function will be
    //! called by stringify() and fmtStringify(). \param isFmt 'true' if
    //! formatting is required.
    std::string pstringify(std::size_t spaceNum = 0, bool isFmt = false) const;

    //! \brief Deep copy a Json
    void copy(const Json &json);

  public:
    explicit Json();
    //! \brief Construct a Json from Type.
    explicit Json(Type type);
    //! \brief Construct a Json from nullptr_t.
    explicit Json(std::nullptr_t value);
    //! \brief Construct a Json from bool.
    explicit Json(const bool value);
    //! \brief Construct a Json from int.
    explicit Json(const int value);
    //! \brief Construct a Json from double.
    explicit Json(const double value);
    //! \brief Construct a Json from const char*.
    explicit Json(const char *value);
    //! \brief Construct a Json from string.
    explicit Json(const std::string &value);
    //! \brief Construct a Json from array.
    explicit Json(const std::vector<Json> &value);
    //! \brief Construct a Json from object.
    explicit Json(const std::map<std::string, Json> &value);

    Json(const Json &other);
    Json(Json &&other) noexcept;
    Json &operator=(const Json &other);
    Json &operator=(Json &&other) noexcept;
    ~Json();

    void swap(Json &other);
    void clear();

    Json &operator=(std::nullptr_t value);
    Json &operator=(const bool value);
    Json &operator=(const int value);
    Json &operator=(const double value);
    Json &operator=(const char *value);
    Json &operator=(const std::string &value);
    Json &operator=(const std::vector<Json> &value);
    Json &operator=(const std::map<std::string, Json> &value);

    //! \brief Compare two Jsons
    //! \return 'true' if both JSON have equal _value and _type.
    bool equal(const Json &other) const;
    //! \brief Compare two Jsons
    //! \return 'true' if both JSON have equal _value and _type.
    bool operator==(const Json &other) const;
    //! \brief Compare two Jsons
    //! \return 'false' if both JSON have equal _value and _type.
    bool operator!=(const Json &other) const;

    //! \brief  Get the Type of Json.
    //! \return Type
    Type type() const;
    //! \brief Determine if the type is Type::JSON_NULL
    //! \return 'true' if the type is Type::JSON_NULL
    bool isNull() const;
    //! \brief Determine if the type is Type::JSON_BOOL
    //! \return 'true' if the type is Type::JSON_BOOL
    bool isBool() const;
    //! \brief Determine if the type is Type::JSON_INT
    //! \return 'true' if the type is Type::JSON_INT
    bool isInt() const;
    //! \brief Determine if the type is Type::JSON_DOUBLE
    //! \return 'true' if the type is Type::JSON_DOUBLE
    bool isDouble() const;
    //! \brief Determine if the type is Type::JSON_STRING
    //! \return 'true' if the type is Type::JSON_STRING
    bool isString() const;
    //! \brief Determine if the type is Type::JSON_ARRAY
    //! \return 'true' if the type is Type::JSON_ARRAY
    bool isArray() const;
    //! \brief Determine if the type is Type::JSON_OBJECT
    //! \return 'true' if the type is Type::JSON_OBJECT
    bool isObject() const;

    //! \brief Get the bool value
    //! \return 'nullopt' if the type of Json is not Type::JSON_BOOL
    std::optional<bool> valueBool() const;
    //! \brief Get the Int value
    //! \return 'nullopt' if the type of Json is not Type::JSON_INT
    std::optional<int> valueInt() const;
    //! \brief Get the Double value
    //! \return 'nullopt' if the type of Json is not Type::JSON_DOUBLE
    std::optional<double> valueDouble() const;
    //! \brief Get the String value
    //! \return 'nullopt' if the type of Json is not Type::JSON_STRING
    std::optional<std::string> valueString() const;
    //! \brief Get the Array value
    //! \return 'nullopt' if the type of Json is not Type::JSON_ARRAY
    std::optional<std::vector<Json>> valueArray() const;
    //! \brief Get the Object value
    //! \return 'nullopt' if the type of Json is not Type::JSON_OBJECT
    std::optional<std::map<std::string, Json>> valueObject() const;

    //! \brief Get length of String
    //! \return 'nullopt' if the type of Json is not Type::JSON_STRING
    std::optional<std::size_t> length() const;
    //! \brief Get size of Array or Object
    //! \return 'nullopt' if the type of Json is not Type::JSON_ARRAY or
    //! Type::JSON_OBJECT
    std::optional<std::size_t> size() const;

    //! \brief Consistent with vector's [].
    Json &operator[](const std::size_t index) const;
    //! \brief Consistent with map's [].
    Json &operator[](const char *key) const;
    //! \brief Consistent with map's [].
    Json &operator[](const std::string &key) const;

    //! \brief Consistent with vector's push_back().
    void push_back(const Json &other);
    //! \brief Consistent with map's insert.
    void insert(std::pair<const char *, Json> k_v);
    //! \brief Consistent with map's insert.
    void insert(std::pair<const std::string &, Json> k_v);

    //! \brief Consistent with vector's erase.
    void erase(const std::size_t index);
    //! \brief Consistent with map's erase.
    void erase(const char *key);
    //! \brief Consistent with map's erase.
    void erase(const std::string &key);

    //! \brief Consistent with empty of vector or map.
    bool empty() const;
    //! \brief Consistent with find of map.
    std::map<std::string, Json>::iterator find(const char *key) const; // object
    //! \brief Consistent with find of map.
    std::map<std::string, Json>::iterator
    find(const std::string &key) const; // object

    //! \brief Convert JSON to string
    std::string stringify() const;
    //! \brief Convert JSON to string
    std::string fmtStringify() const;
    friend std::ostream &operator<<(std::ostream &a, Json &b) {
        return a << b.stringify();
    }
};

} // namespace eee