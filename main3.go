package main

import (
	"fmt"
)

// Функция НОД
func gcd(a, b int) int {
	for b != 0 {
		a, b = b, a%b
	}
	if a < 0 {
		return -a
	}
	return a
}

// Проверка на расходимость
func isDivergent(b int) bool {
	return b == 1
}

// Возведение в целую степень
func power(base, exp int) int {
	res := 1
	for i := 0; i < exp; i++ {
		res *= base
	}
	return res
}

// Основная функция вычисления суммы
func calculateSum(a, b int) (int, int) {
	den := power(b-1, a+1)
	var num int

	if a == 1 {
		num = b
	} else if a == 2 {
		num = b * (b + 1)
	} else if a == 3 {
		num = b * (b*b + 4*b + 1)
	}

	return num, den
}

func main() {
	var a, b int
	fmt.Print("Введите a и b (1-10): ")
	fmt.Scan(&a, &b)

	if isDivergent(b) {
		fmt.Println("infinity")
	} else {
		num, den := calculateSum(a, b)
		common := gcd(num, den)
		fmt.Printf("%d/%d\n", num/common, den/common)
	}
}
