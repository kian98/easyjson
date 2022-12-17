#pragma once

#include <map>
#include <string>
#include <vector>

// 命名空间隔离
namespace easyjson {
class Json {
public:
    // json类型 枚举
    enum Type {
        json_null = 0,
        json_bool,
        json_int,
        json_double,
        json_string,
        json_array,
        json_object
    };

    // 构造函数
    Json();
    Json(bool vlaue);
    Json(int value);
    Json(double value);
    Json(const char *value);
    Json(const std::string &value);
    Json(Type type);
    Json(const Json &other);

    operator bool();
    operator int();
    operator double();
    operator std::string();

    Json &operator[](int index);
    void append(const Json &other);

    // C格式
    Json &operator[](const char *key);
    // C++格式
    Json &operator[](const std::string &key);

    // 重载赋值运算符，避免动态分配的内存出现内存泄漏
    void operator=(const Json &other);
    void clear();

    // 判断相等或不相等
    bool operator==(const Json &other) const;
    bool operator!=(const Json &other) const;

    // 可视化Json对象内容
    std::string str() const;

    // 迭代器，用于遍历Array对象
    typedef std::vector<Json>::iterator iterator;
    iterator begin() { return m_value.m_array->begin(); }
    iterator end() { return m_value.m_array->end(); }

    bool isNull() const { return m_type == json_null; }
    bool isBool() const { return m_type == json_bool; }
    bool isInt() const { return m_type == json_int; }
    bool isDouble() const { return m_type == json_double; }
    bool isString() const { return m_type == json_string; }
    bool isArray() const { return m_type == json_array; }
    bool isObject() const { return m_type == json_object; }

    // 对于基本类型Json对象，获取对应类型的值
    // const 作用：保证函数内部不会修改成员变量
    bool asBool() const;
    int asInt() const;
    double asDouble() const;
    std::string asString() const;

    // 判断数组/对象里是否有key
    bool has(int index) const;
    bool has(const char *key) const;
    bool has(const std::string &key) const;

    // 删除数组/对象里的key
    void remove(int index);
    void remove(const char *key);
    void remove(const std::string &key);

private:
    void copy(const Json &other);

private:
    // 联合体
    // 内存占用共享，大小由最大的决定，即double 8字节
    union Value {
        bool m_bool;
        int m_int;
        double m_double;
        std::string *m_string;
        std::vector<Json> *m_array;
        std::map<std::string, Json> *m_object;
    };

    // Json对象包含两个字段： Type和Value
    Type m_type;
    Value m_value;
};
}  // namespace easyjson
