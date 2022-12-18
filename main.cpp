#include <fstream>
#include <iostream>
#include <sstream>
using namespace std;

#include "json.h"
#include "parser.h"
using namespace easyjson;

void test_Json() {
    // // 构造函数，基本类型->Json
    // Json v1;
    // Json v2(true);
    // Json v3(123);
    // Json v4(3.14);
    // Json v5("hello");

    // // Json -> 基本类型
    // bool b = v2;
    // int i = v3;
    // double d = v4;
    // const string &s = v5;

    // Json数组类型
    // 支持下标访问和append，并且可以转化为需要的值
    Json arr;
    arr[0] = false;
    arr[1] = 456;
    arr.append(3.14159);
    arr.append("world");

    bool b = arr[0];
    int i = arr[1];
    double d = arr[2];
    const string &s = arr[3];

    cout << arr.has(2) << endl;
    std::cout << arr.str() << std::endl;

    arr.remove(2);

    for (auto it = arr.begin(); it != arr.end(); ++it) {
        std::cout << (*it).str() << std::endl;
    }

    // Json 对象
    Json obj;
    obj["bool"] = true;
    obj["bool"] = false;
    obj["int"] = 123;
    obj["double"] = 3.14;
    obj["string"] = "hello";

    cout << obj.has("int") << endl;

    cout << obj.str() << endl;

    obj.remove("int");

    cout << obj.str() << endl;

    // 注意释放内存
    // 若对象中，某个key对应的值为数组，注意避免重复释放
    obj.clear();
}

void test_Parser() {
    // 解析Json字符串
    Json p;
    // const std::string str = "[123, -123, \"test\", [3,4,5, true], false,
    // null]";
    const std::string str =
        "{\"k1\": 123, \"k2\": \"hello\", \"k3\": [1,2,3,4,5]}";
    p.parse(str);
    cout << p.str() << endl;
}

void test_file() {
    // read text from file
    ifstream ifs("example.json");
    stringstream ss;
    ss << ifs.rdbuf();
    const string &str = ss.str();
    ifs.close();
    cout << str.substr(0, 100) << endl;

    // parse json
    Json p;
    p.parse(str);
    cout << p.str() << endl;
}

int main() {
    test_file();
    return 0;
}