#include "json.h"

#include <sstream>
#include <stdexcept>

using namespace easyjson;

Json::Json() : m_type(json_bool) {}

Json::Json(bool value) : m_type(json_bool) { m_value.m_bool = value; }
Json::Json(int value) : m_type(json_int) { m_value.m_int = value; }

Json::Json(double value) : m_type(json_double) { m_value.m_double = value; }

// 以下三个构造函数传入为指针，需要new
Json::Json(const char *value) : m_type(json_string) {
    m_value.m_string = new std::string(value);
}

Json::Json(const std::string &value) : m_type(json_string) {
    m_value.m_string = new std::string(value);
}
Json::Json(Type type) : m_type(type) {
    switch (m_type) {
    case json_null:
        break;
    case json_bool:
        m_value.m_bool = false;
        break;
    case json_int:
        m_value.m_int = 0;
        break;
    case json_double:
        m_value.m_double = 0.0;
        break;
    case json_string:
        m_value.m_string = new std::string("");
        break;
    case json_array:
        m_value.m_array = new std::vector<Json>();
        break;
    case json_object:
        m_value.m_object = new std::map<std::string, Json>();
        break;
    default:
        break;
    }
}

// TODO: 深拷贝和浅拷贝 有待研究
Json::Json(const Json &other) : m_type(other.m_type) { copy(other); }

void Json::copy(const Json &other) {
    m_type = other.m_type;
    switch (m_type) {
    case json_null:
        break;
    case json_bool:
        m_value.m_bool = other.m_value.m_bool;
        break;
    case json_int:
        m_value.m_int = other.m_value.m_int;
        break;
    case json_double:
        m_value.m_double = other.m_value.m_double;
        break;
    case json_string:
        // 采用深拷贝，性能损耗较大
        // m_value.m_string = new std::string(*other.m_value.m_string);
        // 采用浅拷贝，存在内存泄漏风险
        m_value.m_string = other.m_value.m_string;
        break;
    case json_array:
        // 深拷贝
        // m_value.m_array = new std::vector<Json>(*other.m_value.m_array);
        // 浅拷贝
        m_value.m_array = other.m_value.m_array;
        break;
    case json_object:
        // m_value.m_object = new std::map<std::string,
        // Json>(*other.m_value.m_object);
        m_value.m_object = other.m_value.m_object;
        break;
    default:
        break;
    }
}

void Json::clear() {
    switch (m_type) {
    case json_null:
        break;
    case json_bool:
        m_value.m_bool = false;
        break;
    case json_int:
        m_value.m_int = 0;
        break;
    case json_double:
        m_value.m_double = 0.0;
        break;
    case json_string:
        delete m_value.m_string;
        break;
    case json_array:
        for (auto it = (m_value.m_array)->begin();
             it != (m_value.m_array)->end(); ++it) {
            it->clear();
        }
        delete m_value.m_array;
        break;
    case json_object:
        for (auto it = (m_value.m_object)->begin();
             it != (m_value.m_object)->end(); ++it) {
            (it->second).clear();
        }
        delete m_value.m_object;
        break;
    default:
        break;
    }
    m_type = json_null;
}

Json::operator bool() {
    if (m_type != json_bool) {
        throw new std::logic_error("Type error, not bool.");
    }
    return m_value.m_bool;
}

Json::operator int() {
    if (m_type != json_int) {
        throw new std::logic_error("Type error, not int.");
    }
    return m_value.m_int;
}

Json::operator double() {
    if (m_type != json_double) {
        throw new std::logic_error("Type error, not double.");
    }
    return m_value.m_double;
}

Json::operator std::string() {
    if (m_type != json_string) {
        throw new std::logic_error("Type error, not string.");
    }
    return *(m_value.m_string);
}

Json &Json::operator[](int index) {
    if (m_type != json_array) {
        clear();
        m_type = json_array;
        m_value.m_array = new std::vector<Json>();
    }
    if (index < 0) {
        throw new std::out_of_range("Index out of range.");
    }
    if (index >= (m_value.m_array)->size()) {
        (m_value.m_array)->resize(index + 1);
    }
    return (m_value.m_array)->at(index);
}

void Json::append(const Json &other) {
    if (m_type != json_array) {
        clear();
        m_type = json_array;
        m_value.m_array = new std::vector<Json>();
    }
    (m_value.m_array)->push_back(other);
}

Json &Json::operator[](const char *key) {
    std::string name(key);
    return (*this)[name];
}

Json &Json::operator[](const std::string &key) {
    if (m_type != json_object) {
        clear();
        m_type = json_object;
        m_value.m_object = new std::map<std::string, Json>();
    }
    return (*(m_value.m_object))[key];
}

void Json::operator=(const Json &other)
/*
    赋值重载作用是避免原本Json对象的内存泄漏
*/
{
    if (this != &other) {
        clear();
        copy(other);
    }
}

bool Json::operator==(const Json &other) const {
    if (m_type != other.m_type) {
        return false;
    }
    switch (m_type) {
    case json_null:
        return true;
    case json_bool:
        return m_value.m_bool == other.m_value.m_bool;
    case json_int:
        return m_value.m_int == other.m_value.m_int;
    case json_double:
        return m_value.m_double == other.m_value.m_double;
    case json_string:
        return *(m_value.m_string) == *(other.m_value.m_string);
    // 对于数组/对象，此处直接判断指针是否相同
    case json_array:
        return *(m_value.m_array) == *(other.m_value.m_array);
    case json_object:
        return *(m_value.m_object) == *(other.m_value.m_object);
    default:
        return false;
    }
}
bool Json::operator!=(const Json &other) const { return !((*this) == other); }

std::string Json::str() const
/*
    打印Json对象内容，便于调试
*/
{
    std::stringstream ss;
    switch (m_type) {
    case json_null:
        ss << "null";
        break;
    case json_bool:
        ss << (m_value.m_bool ? "true" : "false");
        break;
    case json_int:
        ss << m_value.m_int;
        break;
    case json_double:
        ss << m_value.m_double;
        break;
    case json_string:
        ss << "\"" << *(m_value.m_string) << "\"";
        break;
    case json_array:
        ss << "[";
        for (int i = 0; i < (m_value.m_array)->size(); i++) {
            if (i > 0) {
                ss << ",";
            }
            ss << (m_value.m_array)->at(i).str();
        }
        ss << "]";
        break;
    case json_object:
        ss << "{";
        for (auto it = (m_value.m_object)->begin();
             it != (m_value.m_object)->end(); it++) {
            if (it != (m_value.m_object)->begin()) {
                ss << ",";
            }
            ss << "\"" << it->first << "\":" << it->second.str();
        }
        ss << "}";
        break;
    default:
        break;
    }
    return ss.str();
}

bool Json::asBool() const {
    if (m_type != json_bool) {
        throw new std::logic_error("Type error, not bool.");
    }
    return m_value.m_bool;
}
int Json::asInt() const {
    if (m_type != json_int) {
        throw new std::logic_error("Type error, not int.");
    }
    return m_value.m_int;
}
double Json::asDouble() const {
    if (m_type != json_double) {
        throw new std::logic_error("Type error, not double.");
    }
    return m_value.m_double;
}
std::string Json::asString() const {
    if (m_type != json_string) {
        throw new std::logic_error("Type error, not string.");
    }
    return *(m_value.m_string);
}

bool Json::has(int index) const {
    if (m_type != json_array) {
        return false;
    }
    return index >= 0 && index < (m_value.m_array)->size();
}
bool Json::has(const char *key) const {
    std::string name(key);
    return has(name);
}
bool Json::has(const std::string &key) const {
    if (m_type != json_object) {
        return false;
    }
    return (m_value.m_object)->find(key) != (m_value.m_object)->end();
}

// void remove(int index);
//     void remove(const char *key);
//     void remove(const std::string &key);

void Json::remove(int index) {
    if (m_type != json_array) {
        throw new std::logic_error("Type error, not array.");
    }
    if (this->has(index)) {
        (*(m_value.m_array))[index].clear();
        (m_value.m_array)->erase((m_value.m_array)->begin() + index);
    }
}

void Json::remove(const char *key) {
    std::string name(key);
    remove(name);
}

void Json::remove(const std::string &key) {
    if (m_type != json_object) {
        throw new std::logic_error("Type error, not object.");
    }
    if (this->has(key)) {
        // 释放动态分配内存
        (*(m_value.m_object))[key].clear();
        // 对于map，erase会返回0或1，表示是否成功删除
        (m_value.m_object)->erase(key);
    }
}