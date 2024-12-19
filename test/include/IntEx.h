#ifndef INTEX_H
#define INTEX_H

#include "NumberEx.h"
#include <vector>
#include <iostream>

class IntEx : NumberEx {
private:
    std::vector<uint64_t> data;
    bool isPositive;

    static constexpr size_t BLOCK_MAX = 18;

    IntEx() : isPositive(true), data() {}

    IntEx(const std::string& str) {
        from_string(str);
    }

    IntEx(const uint64_t& value) : isPositive(true), data() {
        data.push_back(value);
    }

    bool isGreaterThan(const IntEx& other) const;
    bool isEqualTo(const IntEx& other) const;

    friend IntEx add(const IntEx& num1, const IntEx& num2);
    //要保证abs1大于abs2
    friend IntEx sub(const IntEx& num1, const IntEx& num2);
    //采用Toom-Cook算法的乘法
    friend IntEx multTC(const IntEx& num1, const IntEx& num2);
    //采用传统算法的乘法
    friend IntEx multT(const IntEx& num1, const IntEx& num2);
    //采用传统算法的除法
    friend IntEx divT(const IntEx& num1, const IntEx& num2);
    //传统取模
    friend IntEx modT(const IntEx& num1, const IntEx& num2);

public:

    //工厂模式create方法
    static IntEx create(const std::string& str = "0") {
        return IntEx(str);
    }    

    void clear();
    bool isEmpty();

    size_t length() const;
    //拷贝构造函数
    IntEx(const IntEx& other) : isPositive(other.isPositive), data(other.data) {}
    //移动拷贝函数
    IntEx(IntEx&& other) noexcept : isPositive(other.isPositive), data(std::move(other.data)) {}

    IntEx abs(void) const;

    //重载计算符号

    IntEx& operator=(const IntEx& other);
    IntEx& operator=(IntEx&& other) noexcept;
    IntEx operator+(const IntEx& other) const;
    IntEx operator+(const std::string& other) const;
    IntEx operator-(const IntEx& other) const;
    IntEx operator*(const IntEx& other) const;
    IntEx operator/(const IntEx& other) const;
    IntEx operator%(const IntEx& other) const;

    //重载逻辑判断

    bool operator>(const IntEx& other) const;
    bool operator<(const IntEx& other) const;
    bool operator==(const IntEx& other) const;
    //重载==比较IntEx与字符串（利用构造函数）
    bool operator==(const std::string& str) const;
    bool operator!=(const IntEx& other) const;
    bool operator>=(const IntEx& other) const;
    bool operator<=(const IntEx& other) const;


    //从字符串中读入
    void from_string(const std::string& str);
    //输出到字符串
    std::string to_string() const;

    __uint128_t to_uint128_t() const;

    void append(char c, size_t n = 1);
    void append(const std::string& str);

    //测试打印，参数"number"或"raw"
    void print(const std::string& format = "number", bool useThousandsSparator = false) const;
};

#endif