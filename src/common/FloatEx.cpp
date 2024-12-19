#include "FloatEx.h"

void FloatEx::from_string(const std::string& str) {
    if(str.empty()) {
        throw std::invalid_argument("Empty String Found.");
        }
    if (!mantissa.isEmpty()) {
        mantissa.clear();
    }
    isPositive = (str[0] != '-');
    std::string absStr = isPositive ? str : str.substr(1);

    size_t absSize = absStr.size();

    size_t hasPoint = 0;
    size_t pointPos = absSize - 1;

    for (size_t i = 0; i < absStr.size() && hasPoint < 2; i++) {
        if (absStr[i] == '.') {
            hasPoint++;
            pointPos = i;
        }
    }

    if (hasPoint > 1) {
        throw std::invalid_argument("More Than One Point Found.");
    }
    if (hasPoint) {
        mantissa.from_string(absStr.erase(pointPos, 1));
    } else {
        mantissa.from_string(absStr);
    }
    exponent = absSize - pointPos - 1;
}

std::string FloatEx::to_string() const {
    std::string outputStr;
    outputStr += mantissa.to_string();

    if (exponent !=0) {
        int POS =  outputStr.size() - exponent;

        if (POS < 0) {
            for (POS; POS < 0; POS++) {
                outputStr.insert(outputStr.begin(), '0');
            }
        }

        outputStr.insert(POS, 1, '.');
    
        if ((*this).mantissa.length() <= (*this).exponent) {
        outputStr.insert(POS, 1,'0');
        }
    }

    outputStr = isPositive ? outputStr : "-" + outputStr;

    return outputStr;
}

std::pair<FloatEx, FloatEx> align(const FloatEx& num1, const FloatEx& num2, size_t appendZeros = 0) {
    const FloatEx* pLARGE_EXP = num1.exponent > num2.exponent ? &num1 : &num2;
    const FloatEx* pSMALL_EXP = num1.exponent > num2.exponent ? &num2 : &num1;

    // 创建副本以便修改
    FloatEx alignedSmall = *pSMALL_EXP;
    FloatEx alignedLarge = *pLARGE_EXP;

    size_t expDiff = alignedLarge.exponent - alignedSmall.exponent;

    //对齐时末尾额外添加0
    for (size_t i = 0; i < appendZeros; i++) {
        alignedLarge.mantissa.append('0');
        alignedLarge.exponent++;
    }

    // 对小数进行对齐
    for (size_t i = 0; i < expDiff; ++i) {
        alignedSmall.mantissa.append('0'); 
        std::cout << i <<"\n";
    }
    alignedSmall.exponent = alignedLarge.exponent;

    // 返回对齐后的两个 FloatEx
    return (pSMALL_EXP == &num1) ?
        std::pair<FloatEx, FloatEx>(alignedSmall, alignedLarge) :
        std::pair<FloatEx, FloatEx>(alignedLarge, alignedSmall);

}

FloatEx add(const FloatEx& num1, const FloatEx& num2) {
    // 对齐两个 FloatEx
    auto [aligned1, aligned2] = align(num1, num2);

    // 处理符号的影响
    IntEx resultMantissa = IntEx::create();
    bool resultIsPositive;

    if (aligned1.isPositive == aligned2.isPositive) {
        // 同号，直接相加
        resultMantissa = aligned1.mantissa + aligned2.mantissa;
        resultIsPositive = aligned1.isPositive;
    } else {
        // 异号，相减
        if (aligned1.mantissa > aligned2.mantissa) {
            resultMantissa = aligned1.mantissa - aligned2.mantissa;
            resultIsPositive = aligned1.isPositive;
        } else {
            resultMantissa = aligned2.mantissa - aligned1.mantissa;
            resultIsPositive = aligned2.isPositive;
        }
    }

    // 构造结果 FloatEx
    FloatEx result;
    result.mantissa = resultMantissa;
    result.exponent = aligned1.exponent; // 对齐后指数一致
    result.isPositive = resultIsPositive;

    return result;
}


FloatEx sub(const FloatEx& num1, const FloatEx& num2) {
    // 创建一个符号相反的 num2
    FloatEx negatedNum2 = num2;
    negatedNum2.isPositive = !num2.isPositive;

    // 使用相同的逻辑处理减法
    return add(num1, negatedNum2);
}

FloatEx mult(const FloatEx& num1, const FloatEx& num2) {
    // 计算尾数的乘积
    IntEx resultMantissa = num1.mantissa * num2.mantissa;

    // 计算指数的相加
    int resultExponent = num1.exponent + num2.exponent;

    // 计算符号
    bool resultIsPositive = (num1.isPositive == num2.isPositive);

    // 构造结果 FloatEx
    FloatEx result;
    result.mantissa = resultMantissa;
    result.exponent = resultExponent;
    result.isPositive = resultIsPositive;

    return result;
}

FloatEx div(const FloatEx& num1, const FloatEx& num2, const size_t THRESHOLD = 9) {
    const size_t LOCAL_PRECISION = std::max(num1.mantissa.length(), num2.mantissa.length());
    const size_t DEFAULT_PRECISION = 18;

    auto [aligned1, aligned2] = align(num1, num2);

    aligned1.mantissa.print();
    aligned2.mantissa.print();

    bool isDefaultPrecision = THRESHOLD > LOCAL_PRECISION;

    if (isDefaultPrecision) {
        for (size_t i = DEFAULT_PRECISION - aligned1.mantissa.length(); i != 0; i--) {
            aligned1.mantissa.append('0');
            aligned1.exponent++;            
        }
    } else {
        for (size_t i = LOCAL_PRECISION - aligned1.mantissa.length(); i != 0; i--) {
            aligned1.mantissa.append("00");
            aligned1.exponent += 2;
        }
    }

    FloatEx result = FloatEx::create();

    result.mantissa = aligned1.mantissa / aligned2.mantissa;
    aligned1.mantissa.print();
    result.exponent = aligned1.exponent;
    result.isPositive = (aligned1.isPositive == aligned2.isPositive);

    return result;
}

FloatEx FloatEx::operator+(const FloatEx& other) const {
    return add((*this), other);
}

FloatEx FloatEx::operator-(const FloatEx& other) const {
    return sub((*this), other);
}

FloatEx FloatEx::operator*(const FloatEx& other) const {
    return mult((*this), other);
}

FloatEx FloatEx::operator/(const FloatEx& other) const {
    if (other == "0") {
        throw std::invalid_argument("Divided by zero is not allowed.");
    }
    return div((*this), other);
}

void FloatEx::print(const std::string& format, bool useThousandsSparator) const {
    if (format == "number") {
        if (useThousandsSparator) {
            std::string tempStr = to_string();
            const size_t STR_SIZE = tempStr.length() - exponent;
            
            if (STR_SIZE > 3) {
                int pos = exponent == 0 ? 
                    STR_SIZE - 3 : 
                    STR_SIZE - 4;
                do
                {
                    tempStr.insert(pos, 1, ',');
                    pos -= 3;
                } while (pos > 0);
            }
            
            std::cout<< tempStr << std::endl;
        }
        else {
            std::cout << to_string() << std::endl;
        }
    } else if (format == "raw"){
        this->mantissa.print();
        std::cout << exponent << std::endl;
    }
}

bool FloatEx::operator>(const FloatEx& other) const {
    // 符号判断
    if (this->isPositive && !other.isPositive) {
        return true;
    } else if (!this->isPositive && other.isPositive) {
        return false;
    }

    // 对齐后比较
    auto [aligned1, aligned2] = align(*this, other);

    if (aligned1.mantissa > aligned2.mantissa) {
        return this->isPositive; // 正数大于，负数小于
    } else if (aligned1.mantissa < aligned2.mantissa) {
        return !this->isPositive; // 正数小于，负数大于
    }

    // mantissa 相等，返回 false
    return false;
}


bool FloatEx::operator<(const FloatEx& other) const {
    // 符号判断
    if (this->isPositive && !other.isPositive) {
        return false;
    } else if (!this->isPositive && other.isPositive) {
        return true;
    }

    // 对齐后比较
    auto [aligned1, aligned2] = align(*this, other);

    if (aligned1.mantissa < aligned2.mantissa) {
        return this->isPositive; // 正数小于，负数大于
    } else if (aligned1.mantissa > aligned2.mantissa) {
        return !this->isPositive; // 正数大于，负数小于
    }

    // mantissa 相等，返回 false
    return false;
}

bool FloatEx::operator==(const FloatEx& other) const {
    // 符号不同直接返回 false
    if (this->isPositive != other.isPositive) {
        return false;
    }

    // 对齐后比较
    auto [aligned1, aligned2] = align(*this, other);

    // 比较 mantissa 和 exponent
    return aligned1.mantissa == aligned2.mantissa && aligned1.exponent == aligned2.exponent;
}

bool FloatEx::operator==(const std::string& str) const {
    return (*this) == create(str);
}

bool FloatEx::operator!=(const FloatEx& other) const {
    return !((*this) == other);
}

bool FloatEx::operator>=(const FloatEx& other) const {
    return !((*this) < other);
}

bool FloatEx::operator<=(const FloatEx& other) const {
    return !((*this) > other);
}


FloatEx& FloatEx::operator=(const FloatEx& other) {
    if (this!= &other) {
        isPositive = other.isPositive;
        mantissa = other.mantissa;
    }
    return *this;
}

FloatEx& FloatEx::operator=(FloatEx&& other) noexcept {
    if (this != &other) {
        isPositive = other.isPositive;
        mantissa = std::move(other.mantissa);
    }
    return *this;
}