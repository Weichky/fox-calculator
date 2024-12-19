#ifndef FLOATEX_H
#define FLOATEX_H

#include "IntEx.h"

class FloatEx : public NumberEx {
private:
    IntEx mantissa;
    int exponent;
    bool isPositive;

    // 私有构造函数，禁止外部调用
    FloatEx() : mantissa(IntEx::create()), exponent(0), isPositive(true) {}

    // 通过字符串创建
    FloatEx(const std::string& str) : mantissa(IntEx::create()) {
        from_string(str);
    }

    //对齐指数，可以对齐后再额外尾缀0
    friend std::pair<FloatEx, FloatEx> align(const FloatEx& num1, const FloatEx& num2, size_t appendZeros);
    friend FloatEx add(const FloatEx& num1, const FloatEx& num2);
    friend FloatEx sub(const FloatEx& num1, const FloatEx& num2);
    friend FloatEx mult(const FloatEx& num1, const FloatEx& num2);
    //默认精度18位。当结果超出18位时，默认精度为操作数。
    friend FloatEx div(const FloatEx& num1, const FloatEx& num2, const size_t THRESHOLD_PRECISION);
    
public:
    // 显式声明复制构造函数
    FloatEx(const FloatEx& other) 
        : mantissa(other.mantissa), exponent(other.exponent), isPositive(other.isPositive) {}

    // 移动构造函数
    FloatEx(FloatEx&& other) noexcept
        : mantissa(std::move(other.mantissa)), exponent(other.exponent), isPositive(other.isPositive) {}

    static FloatEx create(const std::string& str = "0") {
        return FloatEx(str);  // 这里返回一个临时对象
    }

    // 赋值运算符
    FloatEx& operator=(const FloatEx& other);
    FloatEx& operator=(FloatEx&& other) noexcept;

    void from_string(const std::string& str);

    std::string to_string() const;

    // 运算符重载
    FloatEx operator+(const FloatEx& other) const;
    FloatEx operator-(const FloatEx& other) const;
    FloatEx operator*(const FloatEx& other) const;
    FloatEx operator/(const FloatEx& other) const;

    bool operator>(const FloatEx& other) const;
    bool operator<(const FloatEx& other) const;
    bool operator==(const FloatEx& other) const;
    //重载==实现FloatEx与字符串的比较（利用构造函数）
    bool operator==(const std::string& other) const;
    bool operator!=(const FloatEx& other) const;
    bool operator>=(const FloatEx& other) const;
    bool operator<=(const FloatEx& other) const;

    void print(const std::string& format = "number", bool useThousandsSparator = false) const;
};

#endif