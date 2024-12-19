#include <stdexcept>
#include <algorithm>//std::reverse
#include <iostream>
#include <sstream>
#include <iomanip>
#include "IntEx.h"
#include "utils.h"

//从字符串读入,高位对高位
void IntEx::from_string(const std::string& str) {
    if (str.empty()) {
        throw std::invalid_argument("Empty String Found.");
    }

    if (!isEmpty()) {
        clear();
    }

    isPositive = (str[0] != '-');
    std::string absStr = isPositive ? str : str.substr(1);
    size_t chunk_size = 18;
    std::reverse(absStr.begin(), absStr.end());  // 反转字符串以便从低位到高位处理

    size_t num_chunks = (absStr.size() + chunk_size - 1) / chunk_size;
    data.reserve(num_chunks);  // 为数据预分配空间

    // 将字符串分成多个块，每个块最大为 chunk_size 位，转换为 uint64_t 类型
    for (size_t i = 0, j = 0; i < absStr.size(); i += chunk_size, j++) {
        std::string chunk = absStr.substr(i, chunk_size);
        std::reverse(chunk.begin(), chunk.end());  // 反转每个块
        data.push_back((uint64_t)std::stoull(chunk));  // 转换为 uint64_t 并存储
    }
}

//输出到字符串
std::string IntEx::to_string() const {
    std::ostringstream output;

    if(!this->isPositive) 
    output << "-";

    constexpr size_t BLOCK_SIZE = 18;
    const size_t THIS_SIZE = (*this).data.size();

    output << this->data[THIS_SIZE - 1];

    if (THIS_SIZE > 1)
    {
        for (int i = this->data.size() - 2; i > -1; i--) {
        uint64_t currentValue = this->data[i];

        output << std::setw(BLOCK_SIZE) << std::setfill('0') << currentValue;
        }
    }

    return output.str();
}

__uint128_t IntEx::to_uint128_t() const {
    constexpr uint64_t BLOCK_SIZE = 1000000000000000000;
    const __uint128_t UINT128_LIMIT = __uint128_t(-1);
    const uint64_t UINT64_LIMIT = uint64_t(-1);
    const IntEx INTEX64_LIMIT = create(std::to_string(UINT64_LIMIT));
    const IntEx INTEX128_LIMIT = create(uint128_to_string(UINT128_LIMIT));

    if ((*this) < INTEX128_LIMIT) {
        if ((*this < INTEX64_LIMIT)) {
            return (__uint128_t)std::stoull((*this).to_string());
        } else {
            __uint128_t uint128_1 = (__uint128_t)(*this).data[0] + (__uint128_t)(*this).data[1] * (__uint128_t)BLOCK_SIZE;
            if ((*this).data.size() == 2) {
                return uint128_1;
            } else {
                return uint128_1 + (*this).data[2] * (__uint128_t)BLOCK_SIZE * (__uint128_t)BLOCK_SIZE;
            }
        }
    } else {
        throw std::out_of_range("IntEx::to_uint128_t: Out of Range __uint128_t.");
    }

}

//清空至有唯一块且为0
void IntEx::clear() {
        data.clear();
        isPositive = true;
}

bool IntEx::isEmpty() {
    return data.empty();
}


//基础比较函数
bool IntEx::isGreaterThan(const IntEx& other) const {
    // 1. 处理符号的不同情况
    if (this->isPositive && !other.isPositive) {
        return true;  // 当前数字是正，另一个是负，当前数字肯定大
    }

    if (!this->isPositive && other.isPositive) {
        return false;  // 当前数字是负，另一个是正，当前数字肯定小
    }

    // 2. 如果符号相同，直接比较数据的大小
    size_t thisSize = this->data.size();
    size_t otherSize = other.data.size();

    // 如果数字长度不同，较长的数字一定大
    if (thisSize != otherSize) {
        return this->isPositive ? thisSize > otherSize : thisSize < otherSize;
    }

    // 3. 如果长度相同，逐块比较数据
    for (size_t i = 0; i < thisSize; ++i) {
        size_t idx = thisSize - i - 1;  // 从高位开始比较
        if (this->data[idx] > other.data[idx]) {
            return this->isPositive;  // 当前大
        } else if (this->data[idx] < other.data[idx]) {
            return !this->isPositive;  // 当前小
        }
    }

    // 4. 如果数字完全相等，返回 false
    return false;
}


bool IntEx::isEqualTo(const IntEx& other) const {
    //如果符号不同，直接返回 false
    if (this->isPositive != other.isPositive) {
        return false;
    }

    size_t thisSize = this->data.size();
    size_t otherSize = other.data.size();

    //如果块数不同，直接返回 false
    if (thisSize != otherSize) {
        return false;
    }

    //逐位比较每一块
    for (size_t i = 0; i < thisSize; ++i) {
        if (this->data[i] != other.data[i]) {
            return false;
        }
    }

    //如果符号相同且数据完全相同，返回 true
    return true;
}


size_t IntEx::length() const {
    std::string str = this->to_string();
    if (!str.empty()) {
        return this->isPositive ? str.size() : str.size() - 1 ;
    } else {
        throw std::invalid_argument("Empty IntEx Has No Length.");
    }
}

// 只处理绝对值加法
IntEx add(const IntEx& num1, const IntEx& num2) {
    // 常量：用于控制进位限制
    constexpr uint64_t BASE = 1000000000000000000ULL;

    const std::vector<uint64_t>& abs1 = num1.data;
    const std::vector<uint64_t>& abs2 = num2.data;

    size_t size1 = abs1.size();
    size_t size2 = abs2.size();

    const std::vector<uint64_t>* pLarger = size1 > size2 ? &abs1 : &abs2;
    const std::vector<uint64_t>* pSmaller = size1 <= size2 ? &abs1 : &abs2;

    size_t largerSize = pLarger->size();
    size_t smallerSize = pSmaller->size();

    IntEx result;
    result.data.reserve(largerSize + 1);  // 预留空间，可能会有进位

    bool isCarry = false;

    // 执行相加并考虑进位
    for (size_t i = 0; i < smallerSize; ++i) {
        uint64_t sum = (*pLarger)[i] + (*pSmaller)[i] + isCarry;
        result.data.push_back(sum % BASE);  // 存储当前块的结果
        isCarry = sum >= BASE;  // 判断是否有进位
    }

    // 处理较长部分的剩余部分
    for (size_t i = smallerSize; i < largerSize; ++i) {
        uint64_t sum = (*pLarger)[i] + isCarry;
        result.data.push_back(sum % BASE);  // 存储当前块的结果
        isCarry = sum >= BASE;  // 判断是否有进位
    }

    // 如果最后有进位，新增一位
    if (isCarry) {
        result.data.push_back(1);
    }

    return result;
}

//要保证abs1大于abs2
IntEx sub(const IntEx& num1, const IntEx& num2) {

    constexpr uint64_t BASE = 1000000000000000000ULL;
    
    const std::vector<uint64_t>& abs1 = num1.data;
    const std::vector<uint64_t>& abs2 = num2.data;

    const size_t BLOCK_SIZE = abs1.size();
    const size_t SMALL_SIZE = abs2.size();
    
    //初始化结果
    IntEx result;

    bool isBorrow = false;

    for (size_t i = 0; i < SMALL_SIZE; i++) {
        if (isBorrow) {
                if (abs1[i] < abs2[i] + 1) {
                    result.data.push_back(BASE - abs2[i] - 1 + abs1[i]);
                    isBorrow = true;
                } else {
                    result.data.push_back(abs1[i] - abs2[i] - 1);
                    isBorrow = false;
                }
        } else {
            if (abs1[i] < abs2[i]) {
                result.data.push_back(BASE - abs2[i] + abs1[i]);
                isBorrow = true;
            } else {
                result.data.push_back(abs1[i] - abs2[i]);
                isBorrow = false;
            }
        }
    }

    for (size_t i = SMALL_SIZE; i < BLOCK_SIZE; i++) {
        if (isBorrow) {
            if (abs1[i] < 1) {
                result.data.push_back(BASE - 1 + abs1[i]);
                isBorrow = true;
            } else {
                result.data.push_back(abs1[i] - 1);
                isBorrow = false;
            }
        } else {
            for (int j = SMALL_SIZE; j < BLOCK_SIZE; j++) {
                result.data.push_back(abs1[j]);
            }
            break;
        }
    }

    for(int i = result.data.size() - 1; i > -1; i--) {
        if (result.data[i] == 0) {
            
            result.data.pop_back();
            continue;
        }
        result.data.shrink_to_fit();
    
        break;
    }

    return result;
}

IntEx multTC(const IntEx& num1, const IntEx& num2) {

    constexpr uint64_t BASE = 1000000000000000000ULL;

    const std::vector<uint64_t>& abs1 = num1.data;
    const std::vector<uint64_t>& abs2 = num2.data;

    IntEx result;
    const std::vector<uint64_t>* pLarger = abs1.size() > abs2.size() ? &abs1 : &abs2;
    const std::vector<uint64_t>* pSmaller = pLarger == &abs1 ? &abs2 : &abs1;
    const size_t SIZE = pLarger->size();

    __uint128_t* z = new __uint128_t[SIZE]();
}//IntEx multTC(const IntEx& num1, const IntEx& num2)

IntEx multT(const IntEx& num1, const IntEx& num2) {
    constexpr uint64_t BASE = 1000000000000000000ULL; // 基数
    const std::vector<uint64_t>& abs1 = num1.data;
    const std::vector<uint64_t>& abs2 = num2.data;

    const std::vector<uint64_t>* pLarger = abs1.size() > abs2.size() ? &abs1 : &abs2;
    const std::vector<uint64_t>* pSmaller = pLarger == &abs1 ? &abs2 : &abs1;
    const size_t LSIZE = pLarger->size();
    const size_t SSIZE = pSmaller->size();

    std::vector<__uint128_t> result(LSIZE + SSIZE, 0);

    // 逐块计算乘积并延迟进位处理
    for (size_t i = 0; i < SSIZE; ++i) {
        for (size_t j = 0; j < LSIZE; ++j) {
            result[i + j] += (__uint128_t)(*pSmaller)[i] * (*pLarger)[j];
        }
    }

    // 统一处理进位
    __uint128_t carry = 0;
    std::vector<uint64_t> finalResult(result.size());
    for (size_t i = 0; i < result.size(); ++i) {
        __uint128_t temp = result[i] + carry;
        finalResult[i] = static_cast<uint64_t>(temp % BASE);
        carry = temp / BASE;
    }
    if (carry > 0) {
        finalResult.push_back(carry);
    }

    // 去掉前导零
    while (!finalResult.empty() && finalResult.back() == 0) {
        finalResult.pop_back();
    }

    // 构造返回值
    IntEx resultIntEx;
    resultIntEx.data = std::move(finalResult);
    resultIntEx.isPositive = (num1.isPositive == num2.isPositive);
    return resultIntEx;
}

IntEx divT(const IntEx& num1, const IntEx& num2) {
    const size_t NUM1_LENGTH = num1.length();
    const size_t NUM2_LENGTH = num2.length();
    //数字位数差
    const size_t DIFF = NUM1_LENGTH - NUM2_LENGTH;

    IntEx quotient = IntEx::create();
    IntEx remainder = num1;

    for (int i = DIFF; i > -1 && num2 <= remainder; i--) {

        std::string str;
        for (size_t j = 0; j < i; j++) {
            str += "0";        
        }

        IntEx weightedDivisor = IntEx::create(num2.to_string() + str);

        size_t quo = 0;
        
        while (weightedDivisor <= remainder) {
            remainder = remainder - weightedDivisor;
            quo++;
        }
        quotient = quotient + (std::to_string(quo) + str);
    }

    return quotient;
}//IntEx divT(const IntEx& num1, const IntEx& num2)

IntEx modT(const IntEx& num1, const IntEx& num2) {
    IntEx result = IntEx::create();
    result = num1 - num2 * (num1 / num2);
    return result;
}//IntEx mod(const IntEx& num1, const IntEx& num2)

IntEx IntEx::abs(void) const {
    IntEx result;
    result.data = this->data;
    return result;
}

IntEx& IntEx::operator=(const IntEx& other) {
    if (this!= &other) {
        isPositive = other.isPositive;
        data = other.data;
    }
    return *this;
}

IntEx& IntEx::operator=(IntEx&& other) noexcept {
    if (this != &other) {
        isPositive = other.isPositive;
        data = std::move(other.data);
    }
    return *this;
}

IntEx IntEx::operator+(const IntEx& other) const {
    if (this->isPositive == other.isPositive) {
        // 同号加法
        IntEx result = add(*this, other);
        result.isPositive = this->isPositive;
        return result;
    } else {
        // 异号加法
        bool thisIsLarger = this->abs() > other.abs();
        bool otherIsLarger = this->abs() < other.abs();

        // 减法：较大数减去较小数
        if (thisIsLarger) {
            IntEx result = sub(*this, other);
            result.isPositive = this->isPositive;
            return result;
        } else if (otherIsLarger) {
            IntEx result = sub(other, *this);
            result.isPositive = other.isPositive;
            return result;
        } else {
            IntEx result = create("0");
            result.isPositive = true;
            return result;            
        }

    }
}

IntEx IntEx::operator+(const std::string& str) const {
    IntEx other = create(str);
    return (*this) + other;
}

IntEx IntEx::operator-(const IntEx& other) const {
    if (this->isPositive == other.isPositive) {
        // 同号减法
        bool thisIsLarger = this->abs() > other.abs();
        bool otherIsLarger = this->abs() < other.abs();

        if (thisIsLarger) {
            IntEx result = sub(*this, other);
            result.isPositive = this->isPositive;
            return result;
        } else if (otherIsLarger) {
            IntEx result = sub(other, *this);
            result.isPositive = !this->isPositive;
            return result;
        } else {
            IntEx result = create("0");
            result.isPositive = true;
            return result;
        }

    } else {
        // 异号减法，相当于加法
        IntEx result = add(*this, other);
        result.isPositive = this->isPositive;
        return result;
    }
}

IntEx IntEx::operator*(const IntEx& other) const {
    //WIP 后续添加Karatsuba算法和Toom-Cook算法
    IntEx result = multT(*this, other);

    result.isPositive = (this->isPositive == other.isPositive);

    return result;
}

IntEx IntEx::operator/(const IntEx& other) const {
    if (other == create()) {
        throw std::invalid_argument("Divided by zero is not allowed.");
    } else {
        IntEx result = create();
        if ((*this) < other) {
        result.isPositive = (this->isPositive == other.isPositive);
            return result;
        }        
            result = divT((*this), other);
            result.isPositive = (this->isPositive == other.isPositive);
            return result;
    }
}

IntEx IntEx::operator%(const IntEx& other) const {
    return modT((*this), other);
}

bool IntEx::operator>(const IntEx& other) const {
    return isGreaterThan(other);
}

bool IntEx::operator==(const IntEx& other) const {
    return isEqualTo(other);
}

bool IntEx::operator==(const std::string& str) const {
    return (*this) == create(str);
}

bool IntEx::operator!=(const IntEx& other) const {
    return !isEqualTo(other);
}

bool IntEx::operator<(const IntEx& other) const {
    return !isGreaterThan(other) && !isEqualTo(other);
}

bool IntEx::operator>=(const IntEx& other) const {
    return isGreaterThan(other) || isEqualTo(other);
}

bool IntEx::operator<=(const IntEx& other) const {
    return !isGreaterThan(other);
}

void IntEx::append(char c, size_t n) {
    std::string tempStr = this->to_string();

    for (size_t i = n; i != 0; i--) {
        tempStr += c;
    }

    this->from_string(tempStr);
}

void IntEx::append(const std::string& str) {
    this->from_string(
        this->to_string() + str
    );

}

//格式化测试打印
void IntEx::print(const std::string& format, bool useThousandsSparator) const {

    if (format == "raw") {
        for (int i = 0; i < data.size(); i++) {
            std::cout << data[i] << std::endl;
        }
    } else if (format == "number") {
        if (useThousandsSparator) {
            std::string tempStr = to_string();
            const size_t STR_SIZE = tempStr.length();

            if (STR_SIZE > 3) {
                int pos =  STR_SIZE - 3;
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
    } else {
        throw std::invalid_argument("IntEx::Print() Argument Missing Or Illegal.");
    }
}