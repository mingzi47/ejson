#include <cstdint>
#include <iostream>
#include <memory>
#include <optional>
#include <string>

#include "json.hh"
#include "parser.hh"

using namespace eee;

using array = std::vector<Json>;
using object = std::map<std::string, Json>;

static int tot_test = 0;
static int acc_test = 0;

#define EQUAL(expectly, actualy)                                 \
    {                                                            \
        tot_test++;                                              \
        if (expectly == actualy) {                               \
            acc_test++;                                          \
        } else {                                                 \
            std::fprintf(stderr, "%s:%d\n", __FILE__, __LINE__); \
        }                                                        \
    }

#define TEST_TYPE(expectly, value) \
    {                              \
        Json j{value};             \
        auto ac = j.type();        \
        EQUAL(expectly, ac);       \
    }

void test_c() {
    Json j0{};
    Json j1{int{1234}};
    EQUAL(false, j1.size().has_value());
    EQUAL(false, j1.length().has_value());
    // {
    Json j3{11222.111};
    Json j7{j3};
    //     // std::cout << (j7.type == Type::JSON_DOUBLE) << "\n";
    //     // std::cout << std::get<double>(j7.value) << "\n";
    // }
    // std::cout << (j0.type == Type::JSON_DOUBLE) << "\n";
    Json j4{(char *)"123455"};
    Json j5{std::string{"123455"}};
    EQUAL(6, j4.size());
    EQUAL(6, j5.size());
    EQUAL(true, j5.size().has_value());
    EQUAL(6, j4.length());
    EQUAL(6, j5.length());
    EQUAL(true, j5.length().has_value());

    Json cj2;
    {
        Json cj1("11111111111111111111111111111111111111111111111111");
        cj2 = cj1;
        // std::cout << " { cj1 = " << *std::get<std::string*>(cj1.value) <<
        // "\n"; std::cout << " { cj2 = " <<  *std::get<std::string*>(cj2.value)
        // << "\n";
    }
    // std::cout << " } cj2 = " <<  *std::get<std::string*>(cj2.value) << "\n";

    Json a1;
    {
        array v = {j1, j4, j5};
        // std::cout << " { v = " << v.size() << "\n";
        Json a2{v};
        EQUAL(true, a2.size().has_value());
        EQUAL(3, a2.size());
        EQUAL(true, a2.isArray());
        TEST_TYPE(Type::JSON_ARRAY, v);
        a2 = 1;
        TEST_TYPE(Type::JSON_INT, a2.type());
        a1 = a2;
        // std::cout << " { a1 = " <<  (*std::get<array*>(a1.value)).size()<<
        // "\n"; std::cout << " { a2 = " <<
        // (*std::get<array*>(a2.value)).size()<< "\n";
    }

    // std::cout << " } a1 = " <<  (*std::get<array*>(a1.value)).size()<< "\n";

    Json m1;
    {
        object v;
        v[std::string("1")] = j1;
        v[std::string("2")] = cj2;
        v[std::string("3")] = j5;
        Json m2(v);
        EQUAL(3, m2.size());
        EQUAL(true, m2.size().has_value());
        EQUAL(true, m2.isObject());
        TEST_TYPE(Type::JSON_OBJECT, v);
        m1 = m2;
        m2 = nullptr;
        TEST_TYPE(Type::JSON_NULL, m2.type());
        // std::cout << " { m1 = " <<  (*std::get<object*>(m1.value)).size()<<
        // "\n"; std::cout << " { m2 = " <<
        // (*std::get<object*>(m2.value)).size()<< "\n";
    }
    // // std::cout << " } m1 = " <<  (*std::get<object*>(m1.value)).size()<<
    // "\n";

    // // test std::move
    Json mv1 = m1;
    {
        // std::cout << " { mv1 = " <<  (*std::get<object*>(mv1.value)).size()<<
        // "\n";
        EQUAL(Type::JSON_OBJECT, mv1.type());
        Json mv2 = std::move(mv1);
        EQUAL(Type::JSON_OBJECT, mv2.type());
        EQUAL(Type::JSON_NULL, mv1.type());
        mv2 = "1222222222222222";
        TEST_TYPE(Type::JSON_STRING, mv2.type());
        // std::cout << " { mv2 = " <<  (*std::get<object*>(mv2.value)).size()<<
        // "\n";
    }
    // std::cout << " } mv1 = " <<  (std::get<std::nullptr_t>(mv1.value) ==
    // nullptr)<< "\n";
    std::vector<Json> v111 = {Json{1}, Json{2}, Json{3}, Json{4}, Json{5}};
    Json v222{v111};
    for (std::size_t i = 0; i < v111.size(); i++) { EQUAL(v111[i], v222[i]); }
    std::vector<Json> v333 = {Json{1}, Json{"2"}, Json{3}, Json{4}, Json{5}};
    Json v444{v333};
    EQUAL(false, (v222 == v444))
    EQUAL(true, (v222 != v444))
    EQUAL(false, (v222.equal(v444)))

    std::map<std::string, Json> m111;
    m111["1"] = 1;
    m111["2"] = 3;
    m111["3"] = "111111111111111111111";
    // m111["3"] = 4;
    Json m222(m111);
    for (int i = 1; i <= 3; i++) {
        std::string key = std::to_string(i);
        EQUAL(Json(m111[key]).type(), m222[key].type());
        EQUAL(m111[key], m222[key]);
    }
    bool t11 = true;
    bool f11 = false;
    int i64 = 111111111;
    double d11 = 1.111111111;
    std::string s11 = "21111111111";
    Json t111{t11};
    Json f111{f11};
    Json i111{i64};
    Json d111{d11};
    Json s111{s11};
    EQUAL(t11, t111.valueBool());
    EQUAL(f11, f111.valueBool());
    EQUAL(i64, i111.valueInt());
    EQUAL(s11, s111.valueString());
    EQUAL(v333, v444.valueArray());
    EQUAL(m111, m222.valueObject());
    EQUAL(false, m222.valueBool().has_value());
    std::vector<Json> v555 = {Json{1}, Json{2}, Json{3}, Json{4}, Json{5}};
    Json v666{v555};
    v666.push_back(t111);
    // std::cout << v666.stringify() << "\n";
    EQUAL(6, v666.size());
    v666.erase(5);
    EQUAL(5, v666.size());
    EQUAL(true, Json(v555).equal(v666));
    // std::cout << v666.stringify() << "\n";
    std::map<std::string, Json> m1111;
    m1111["1"] = 1;
    m1111["2"] = v555;
    m1111["3"] = "111111111111111111111";
    Json mee1(m1111);
    // std::cout << mee1.stringify() << "\n";
    mee1.erase("3");
    // std::cout << mee1.stringify() << "\n";
    mee1.insert(std::pair<std::string, Json>(std::string("xx"), v555));

    // // std::cout << mee1.stringify() << "\n";
    v555.push_back(Json{m1111});
    m222["4"] = v555;
    mee1.insert(std::pair<std::string, Json>(std::string("x"), m222));
    std::cout << mee1.stringify() << "\n";
}

void test_type() {
    TEST_TYPE(Type::JSON_NULL, Type::JSON_NULL);
    TEST_TYPE(Type::JSON_BOOL, true);
    TEST_TYPE(Type::JSON_INT, 100);
    TEST_TYPE(Type::JSON_STRING, (char *)"1234556");
    TEST_TYPE(Type::JSON_STRING, std::string("123456"));
    array v = {Json{1}, Json{2}, Json{3}, Json{4}, Json{5}};
    TEST_TYPE(Type::JSON_ARRAY, v);
    TEST_TYPE(Type::JSON_NULL, nullptr);
    EQUAL(true, Json(1).isInt());
    EQUAL(true, Json(1.1).isDouble());
    EQUAL(true, Json("1111111").isString());
    EQUAL(false, Json("111111").isInt());
    EQUAL(false, Json(1.1).isInt());
    EQUAL(true, Json(nullptr).isNull());
    EQUAL(true, Json().isNull());
}

void test_parser() {
    tot_test++;
    std::string a =
        "[{\"111\" : \"111\", \"222\" : null, \"333\" : [false, true], \"444\" : 1e-5}, false, [true, false]]";
    // std::cout << "parser a" << "\n";
    Parser pr(a);
    // std::cout << "parser b" << "\n";
    object x;
    x.insert({"111", Json{"111"}});
    x.insert({"222", Json{nullptr}});
    x.insert({"333", Json{std::vector<Json>{Json{false}, Json{true}}}});
    x.insert({"444", Json{1e-5}});
    Json b = Json{std::vector<Json>{
        Json{x}, Json{false},
        Json{std::vector<Json>{Json{true}, Json{false}}}}};

    if (pr.getValue() == b) {
        acc_test++;
        // std::cout << "Yes\n";
    } else {
        // std::cout << "No\n";
    }

    std::cout << pr.getValue().fmtStringify() << "\n";
    std::cout << " b = " << b << "\n";
}

void test() {
    test_c();
    test_type();
    test_parser();
}
int main() {
    test();

    printf(
        "\n test result = %0.2f%% pass(%d/%d)\n",
        (acc_test * 1.0) / tot_test * 100.0, acc_test, tot_test);
    return 0;
}