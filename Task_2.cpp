#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <iomanip>
#include <cmath>

using namespace std;

// Используем unsigned long long для положительных чисел
typedef unsigned long long uint64;

// Модульное возведение в степень: (base^exp) % mod
// Используем __int128 для защиты от переполнения при умножении
uint64 pow_mod(uint64 base, uint64 exp, uint64 mod) {
    uint64 res = 1;
    base %= mod;
    while (exp > 0) {
        if (exp % 2 == 1) res = (uint64)((__int128)res * base % mod);
        base = (uint64)((__int128)base * base % mod);
        exp /= 2;
    }
    return res;
}

// 2. Вычисление битовой длины числа (стандартный способ)
int get_bit_length(uint64 n) {
    if (n == 0) return 0;
    int bits = 0;
    while (n > 0) {
        n >>= 1;
        bits++;
    }
    return bits;
}

// 3. Решето Эратосфена для поиска простых чисел до 500
vector<int> sieve(int limit) {
    vector<bool> is_prime(limit + 1, true);
    is_prime[0] = is_prime[1] = false;
    for (int i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (int j = i * i; j <= limit; j += i) is_prime[j] = false;
        }
    }
    vector<int> primes;
    for (int i = 2; i <= limit; ++i) {
        if (is_prime[i]) primes.push_back(i);
    }
    return primes;
}

// 4. Генерация кандидата n = 2m + 1
// Возвращает n и список простых множителей числа n-1 (т.е. множителей m и числа 2)
struct Candidate {
    uint64 n;
    vector<uint64> factors;
};

Candidate generate_candidate(int bits, const vector<int>& primes, mt19937& rng) {
    int target_m_bits = bits - 1; // n-1 = 2m, поэтому m должно быть на 1 бит меньше n
    uint64 m = 1;
    vector<uint64> factors;
    factors.push_back(2); // 2 всегда является множителем n-1

    uniform_int_distribution<int> prime_dist(0, primes.size() - 1);
    uniform_int_distribution<int> exp_dist(1, 3); // Случайная степень 1, 2 или 3

    // Пытаемся собрать m нужной длины
    while (get_bit_length(m) < target_m_bits) {
        int p = primes[prime_dist(rng)];
        int exp = exp_dist(rng);
        
        uint64 term = 1;
        for (int k = 0; k < exp; ++k) term *= p;

        // Проверка, чтобы не переполнить uint64 и не уйти далеко за target
        if (get_bit_length(m) + get_bit_length(term) > target_m_bits + 5) {
            continue; // Пропускаем, если множитель слишком большой
        }
        
        m *= term;
        
        // Добавляем p в список множителей, если его там еще нет
        if (find(factors.begin(), factors.end(), (uint64)p) == factors.end()) {
            factors.push_back(p);
        }
    }

    // Если m получилось слишком большим, перезапускаем (возвращаем n=0 как сигнал ошибки)
    if (get_bit_length(m) != target_m_bits) {
        // В реальном приложении нужен retry, здесь для простоты вернем 0
        // Но в цикле main мы обработаем это
        return {0, {}}; 
    }

    uint64 n = 2 * m + 1;
    return {n, factors};
}

// 5. Тест Миллера (по теореме Сэлфриджа из методички)
// factors - это простые множители числа n-1
bool miller_test(uint64 n, const vector<uint64>& factors, int t, mt19937& rng) {
    if (n < 2) return false;
    
    uniform_int_distribution<uint64> a_dist(2, n - 2);
    vector<uint64> bases;
    
    // Шаг 1: Выбираем t различных оснований a
    for (int i = 0; i < t; ++i) {
        bases.push_back(a_dist(rng));
    }

    // Шаг 2: Проверка малой теоремы Ферма (a^(n-1) = 1 mod n)
    // Если ХОТЯ БЫ ОДНО a не проходит, число составное
    for (uint64 a : bases) {
        if (pow_mod(a, n - 1, n) != 1) {
            return false;
        }
    }

    // Для каждого простого множителя q числа n-1
    // Нужно найти ХОТЯ БЫ ОДНО a, такое что a^((n-1)/q) != 1 mod n
    for (uint64 q : factors) {
        bool witness_found = false;
        for (uint64 a : bases) {
            uint64 val = pow_mod(a, (n - 1) / q, n);
            if (val != 1) {
                witness_found = true;
                break; // Для этого q свидетель найден, переходим к следующему q
            }
        }
        // Если для какого-то q свидетеля не нашлось (все a дали 1), то число составное
        if (!witness_found) return false;
    }

    return true; // Простое
}

int main() {
    setlocale(LC_ALL, "Russian");
    
    int bits;
    cout << "Введите требуемую разрядность (bits): ";
    if (!(cin >> bits) || bits < 2 || bits > 20) {
        cout << "Ошибка: введите целое число от 2 до 20." << endl;
        return 1;
    }

    vector<int> primes = sieve(500);
    mt19937 rng(42); // Фиксированный генератор для воспроизводимости

    cout << "\nТаблица результатов (Тест Миллера):\n";
    cout << "------------------------------------------------------------------------\n";
    cout << "| №  | Число-кандидат (n)   |    Простое?  | Кол-во отвергнутых (rejected) |\n";
    cout << "------------------------------------------------------------------------\n";

    // Нужно построить 10 простых чисел
    for (int i = 1; i <= 10; ++i) {
        int rejected_count = 0;
        Candidate cand;

        while (true) {
            cand = generate_candidate(bits, primes, rng);
            
            // Если генерация не удалась (например, переполнение), считаем за rejected
            if (cand.n == 0) {
                rejected_count++;
                continue;
            }

            // Проверка с параметром надежности t=1
            if (!miller_test(cand.n, cand.factors, 1, rng)) {
                rejected_count++;
                continue; // Пробуем следующее число
            }

            // Дополнительная проверка t=2 (согласно методичке)
            if (!miller_test(cand.n, cand.factors, 2, rng)) {
                rejected_count++;
                continue;
            }

            // Если прошли оба теста - число считается простым
            break;
        }

        cout << "| " << setw(2) << i << " | " 
             << setw(20) << cand.n << " | " 
             << setw(8) << "true" << "     | " 
             << setw(29) << rejected_count << " |\n";
    }
    cout << "------------------------------------------------------------------------\n";

    return 0;
}