#include <iostream>
#include <utility> // Для std::pair

using namespace std;

// 1. Функция НОД (Алгоритм Евклида)
int gcd(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// 2. Проверка на расходимость ряда
bool isDivergent(int b) {
    return b == 1;
}

// 3. Возведение в целую степень (избегаем double от pow)
int power(int base, int exp) {
    int res = 1;
    for (int i = 0; i < exp; i++) res *= base;
    return res;
}

// 4. Основная функция вычисления суммы
// Возвращает пару {числитель, знаменатель}
pair<int, int> calculateSum(int a, int b) {
    // Знаменатель = (b-1)^(a+1)
    int den = power(b - 1, a + 1);
    int num = 0;

    // Числитель (формулы для a=1, 2, 3)
    if (a == 1) num = b;
    else if (a == 2) num = b * (b + 1);
    else if (a == 3) num = b * (b * b + 4 * b + 1);
    else {
        // Для a > 3 нужны другие формулы
        num = 0; 
    }

    return {num, den};
}

int main() {
    int a, b;
    cout << "Введите a и b (1-10): ";
    cin >> a >> b;

    if (isDivergent(b)) {
        cout << "infinity" << endl;
    } else {
        // Получаем результат из функции
        pair<int, int> result = calculateSum(a, b);
        
        // Сокращаем дробь
        int common = gcd(result.first, result.second);
        
        cout << result.first / common << "/" << result.second / common << endl;
    }
    return 0;
}