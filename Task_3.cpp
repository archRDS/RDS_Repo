#include <iostream>
#include <cmath>
#include <string>
#include <cstdint>  // Для uint64_t

using namespace std;

// Функция для вычисления наибольшего общего делителя
uint64_t computeGCD(uint64_t num1, uint64_t num2) {
    return num2 == 0 ? num1 : computeGCD(num2, num1 % num2);
}

// Проверка, можно ли число представить конечной десятичной дробью
bool checkFiniteDecimal(long double value, uint64_t& numerator, uint64_t& denominator, long double precision = 1.0e-15) {
    // Проверка на целое число
    if (abs(value - round(value)) < precision) {
        numerator = static_cast<uint64_t>(round(value));
        denominator = 1;
        return true;
    }

    // Поиск точного представления в виде дроби
    for (uint64_t div = 1; div <= 100000; ++div) {
        uint64_t num = static_cast<uint64_t>(round(value * static_cast<long double>(div)));
        if (abs(static_cast<long double>(num) / static_cast<long double>(div) - value) < precision) {
            uint64_t commonFactor = computeGCD(num, div);
            uint64_t simpleNum = num / commonFactor;
            uint64_t simpleDiv = div / commonFactor;

            // Проверка знаменателя на вид 2^m * 5^n
            uint64_t tempDiv = simpleDiv;
            while (tempDiv % 2 == 0) tempDiv /= 2;
            while (tempDiv % 5 == 0) tempDiv /= 5;

            if (tempDiv == 1) {
                numerator = simpleNum;
                denominator = simpleDiv;
                return false;
            }
        }
    }

    return true;
}

// Преобразование числа в строку-дробь, если возможно
string convertToFractionString(long double value) {
    uint64_t num, denom;
    if (checkFiniteDecimal(value, num, denom)) {
        return to_string(num) + "/" + to_string(denom);
    }
    return "irrational";
}

// Вычисление суммы ряда
string computeSeriesSum(int exponent, int base) {
    if (base <= 1) {
        return "Infinity";
    }

    // Специальные случаи с аналитическими решениями
    if (exponent == 0 && base > 1) {
        return "1/" + to_string(base - 1);
    }
    else if (exponent == 1 && base > 1) {
        return to_string(base) + "/" + to_string((base - 1) * (base - 1));
    }
    else if (exponent == 2 && base > 1) {
        return to_string(base * (base + 1)) + "/" + to_string((base - 1) * (base - 1) * (base - 1));
    }
    else if (exponent == 3 && base > 1) {
        return to_string(base * (base * base + 4 * base + 1)) + "/" + to_string((base - 1) * (base - 1) * (base - 1) * (base - 1));
    }
    
    long double totalSum = 0.0;
    const int maxTerms = 10000;
    const long double minTerm = 1e-10;

    for (int termIndex = 1; termIndex <= maxTerms; ++termIndex) {
        long double currentTerm = pow(termIndex, exponent) / pow(base, termIndex);
        totalSum += currentTerm;

        if (currentTerm < minTerm) {
            break;
        }
    }

    return convertToFractionString(totalSum);
}

int main() {
    int expValue, baseValue;

    cout << "Введите показатель степени (целое от 1 до 10): ";
    cin >> expValue;
    cout << "Введите основание (целое от 1 до 10): ";
    cin >> baseValue;

    // Проверка введенных значений
    if (expValue < 1 || expValue > 10 || baseValue < 1 || baseValue > 10) {
        cout << "Ошибка: значения должны быть целыми от 1 до 10" << endl;
        return 1;
    }

    string seriesResult = computeSeriesSum(expValue, baseValue);

    if (seriesResult == "Infinity") {
        cout << "Infinity" << endl;
    }
    else if (seriesResult == "irrational") {
        cout << "Irrational" << endl;
    }
    else {
        cout << seriesResult << endl;
    }
    return 0;
}
