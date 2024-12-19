#include <iostream>
#include <string>
#include <algorithm>

#include "utils.h"

std::string uint128_to_string(__uint128_t value) {
    if (value == 0) {
        return "0";
    }

    std::string result;
    while (value > 0) {
        // 提取最低位
        result.push_back('0' + (value % 10));
        value /= 10;
    }

    std::reverse(result.begin(), result.end());
    return result;
}