#include <iostream>
#include <vector>
#include <numeric>
using namespace std;

int simulateGame(int n, int m, const vector<int>& arr) {
    int pos = 0, pavelScore = 0, vikaScore = 0;
    int pavelLastMove = 0;  // Сколько Павел взял в СВОЙ предыдущий ход
    int vikaLastMove = 0;   // Сколько Вика взяла в СВОЙ предыдущий ход
    bool pavelTurn = true;

    while (pos < n) {
        int remaining = n - pos;
        int maxTake = min(m, remaining);
        int bestK = 1, maxSum = -2e9;

        for (int k = 1; k <= maxTake; ++k) {
            // ВАРИАНТ 1: нельзя брать столько же, сколько взял САМ в свой предыдущий ход
            if (pavelTurn && k == pavelLastMove) continue;  // Павел не может повторить свой ход
            if (!pavelTurn && k == vikaLastMove) continue;  // Вика не может повторить свой ход

            int currentSum = 0;
            for (int i = pos; i < pos + k; ++i) currentSum += arr[i];

            if (currentSum > maxSum || (currentSum == maxSum && k < bestK)) {
                maxSum = currentSum;
                bestK = k;
            }
        }

        if (pavelTurn) {
            pavelScore += maxSum;
            pavelLastMove = bestK;  // Запоминаем ход Павла
        } else {
            vikaScore += maxSum;
            vikaLastMove = bestK;   // Запоминаем ход Вики
        }
        pos += bestK;
        pavelTurn = !pavelTurn;
    }
    return (pavelScore > vikaScore) ? 1 : 0;
}
int main() {
    int n, m;
    cout << "Введите n и m: ";
    cin >> n >> m;
    vector<int> arr(n);
    cout << "Введите последовательность: ";
    for (int &x : arr) cin >> x;
    cout << simulateGame(n, m, arr) << endl;
    return 0; 
}
