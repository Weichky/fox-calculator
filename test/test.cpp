#include <iostream>
#include <chrono>
#include <fstream>
#include <string>
#include "IntEx.h"
#include <gmpxx.h>
#include <random>

std::string generate_test_data(int length) {
    std::string result;
    std::random_device rd;
    std::mt19937 gen(rd());
    
    // 生成每一位：从 1 到 9 随机选择
    std::uniform_int_distribution<> digit_dist(1, 9);
    for (int i = 0; i < length; ++i) {
        result += std::to_string(digit_dist(gen));
    }

    return result;
}

// 加法 GMP 测试
void test_addition_gmp(int len ,int iterations, std::ofstream& output) {

    std::chrono::nanoseconds total_duration(0);

    for (int i = 0; i < iterations; ++i) {
        mpz_class a(generate_test_data(len));
        mpz_class b(generate_test_data(len));
        auto start = std::chrono::high_resolution_clock::now();
        mpz_class sum = a + b;
        auto end = std::chrono::high_resolution_clock::now();
        total_duration += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    }

    // 计算每组的平均时间
    auto average_duration = total_duration.count() / iterations;
    output << "GMP add average time for " << len << " digits: " << average_duration << " ns\n";
    
}

// 加法 IntEx 测试
void test_addition_intEx(int len ,int iterations, std::ofstream& output) {
    IntEx a = IntEx::create(generate_test_data(len));
    IntEx b = IntEx::create(generate_test_data(len));
    std::chrono::nanoseconds total_duration(0);

    for (int i = 0; i < iterations; ++i) {
        auto start = std::chrono::high_resolution_clock::now();
        IntEx sum = a + b;
        auto end = std::chrono::high_resolution_clock::now();
        total_duration += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    }

    // 计算每组的平均时间
    auto average_duration = total_duration.count() / iterations;
    output << "IntEx add average time for " << len << " digits: " << average_duration << " ns\n";
    
}

// 减法 GMP 测试
void test_subtraction_gmp(int len ,int iterations, std::ofstream& output) {
    mpz_class a(generate_test_data(len));
    mpz_class b(generate_test_data(len));
    std::chrono::nanoseconds total_duration(0);

    for (int i = 0; i < iterations; ++i) {
        auto start = std::chrono::high_resolution_clock::now();
        mpz_class diff = a - b;
        auto end = std::chrono::high_resolution_clock::now();
        total_duration += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    }

    // 计算每组的平均时间
    auto average_duration = total_duration.count() / iterations;
    output << "GMP sub average time for " << len << " digits: " << average_duration << " ns\n";
    
}

// 减法 IntEx 测试
void test_subtraction_intEx(int len ,int iterations, std::ofstream& output) {
    IntEx a = IntEx::create(generate_test_data(len));
    IntEx b = IntEx::create(generate_test_data(len));
    std::chrono::nanoseconds total_duration(0);

    for (int i = 0; i < iterations; ++i) {
        auto start = std::chrono::high_resolution_clock::now();
        IntEx diff = a - b;
        auto end = std::chrono::high_resolution_clock::now();
        total_duration += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    }

    // 计算每组的平均时间
    auto average_duration = total_duration.count() / iterations;
    output << "IntEx sub average time for " << len << " digits: " << average_duration << " ns\n";
    
}

// 乘法 GMP 测试
void test_multiplication_gmp(int len ,int iterations, std::ofstream& output) {
    mpz_class a(generate_test_data(len));
    mpz_class b(generate_test_data(len));
    std::chrono::nanoseconds total_duration(0);

    for (int i = 0; i < iterations; ++i) {
        auto start = std::chrono::high_resolution_clock::now();
        mpz_class prod = a * b;
        auto end = std::chrono::high_resolution_clock::now();
        total_duration += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    }

    // 计算每组的平均时间
    auto average_duration = total_duration.count() / iterations;
    output << "GMP mul average time for " << len << " digits: " << average_duration << " ns\n";
    
}

// 乘法 IntEx 测试
void test_multiplication_intEx(int len ,int iterations, std::ofstream& output) {
    IntEx a = IntEx::create(generate_test_data(len));
    IntEx b = IntEx::create(generate_test_data(len));
    std::chrono::nanoseconds total_duration(0);

    for (int i = 0; i < iterations; ++i) {
        auto start = std::chrono::high_resolution_clock::now();
        IntEx prod = a * b;
        auto end = std::chrono::high_resolution_clock::now();
        total_duration += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    }

    // 计算每组的平均时间
    auto average_duration = total_duration.count() / iterations;
    output << "IntEx mul average time for " << len << " digits: " << average_duration << " ns\n";
    
}

// 除法 GMP 测试
void test_division_gmp(int len ,int iterations, std::ofstream& output) {
    mpz_class a(generate_test_data(len));
    mpz_class b(generate_test_data(len));
    std::chrono::nanoseconds total_duration(0);

    for (int i = 0; i < iterations; ++i) {
        auto start = std::chrono::high_resolution_clock::now();
        mpz_class quotient = a / b;
        auto end = std::chrono::high_resolution_clock::now();
        total_duration += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    }

    // 计算每组的平均时间
    auto average_duration = total_duration.count() / iterations;
    output << "GMP div average time for " << len << " digits: " << average_duration << " ns\n";
    
}

// 除法 IntEx 测试
void test_division_intEx(int len ,int iterations, std::ofstream& output) {
    IntEx a = IntEx::create(generate_test_data(len));
    IntEx b = IntEx::create(generate_test_data(len));
    std::chrono::nanoseconds total_duration(0);

    for (int i = 0; i < iterations; ++i) {
        auto start = std::chrono::high_resolution_clock::now();
        IntEx quotient = a / b;
        auto end = std::chrono::high_resolution_clock::now();
        total_duration += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
    }

    // 计算每组的平均时间
    auto average_duration = total_duration.count() / iterations;
    output << "IntEx div average time for " << len << " digits: " << average_duration << " ns\n";
    
}

int main() {
    std::ofstream output("test_results.txt");

    int iterations = 1000000;
    int lengths[] = {10, 20, 30, 50, 100, 150, 200, 300, 500, 1000};

    for (int len : lengths) {

        // 加法测试
        test_addition_gmp(len, iterations, output);
        test_addition_intEx(len, iterations, output);

        // 减法测试
        test_subtraction_gmp(len, iterations, output);
        test_subtraction_intEx(len, iterations, output);

        // 乘法测试
        test_multiplication_gmp(len, iterations, output);
        test_multiplication_intEx(len, iterations, output);

        // 除法测试
        test_division_gmp(len, iterations, output);
        test_division_intEx(len, iterations, output);

        output << "-------------------------------------\n";
    }

    output.close();
    return 0;
}