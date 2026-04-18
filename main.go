package main

import (
	"fmt"
	"math"
)

// Функция вычисления Y
func calculateY(x float64) float64 {
	if x > -5 && x <= -3 {
		return -0.5 * math.Pow(x+3, 2) + 2
	} else if x > -3 && x <= -2 {
		return -x - 1
	} else if x > -2 && x <= -1 {
		return math.Sqrt(1 - math.Pow(x+2, 2))
	} else if x > -1 && x <= 1 {
		return -x - 0.5
	} else if x > 1 && x <= 2 {
		return -1 - math.Sqrt(1 - math.Pow(x-2, 2))
	} else if x > 2 && x <= 3 {
		return -x
	} else if x > 3 && x <= 5 {
		return 0.5 * math.Pow(x-3, 2) - 3
	}
	return 0
}

func main() {
	var xn float64 = -5.0
	var xk float64 = 5.0
	var dx float64 = 0.5

	fmt.Println("X начальное = -5.0 ")
	fmt.Println("Х конечное = 5.0 ")

	// Цикл с учетом погрешности (epsilon) для чисел с плавающей точкой
	for x := xn; x <= xk + 0.001; x += dx {
		y := calculateY(x)
		// %v автоматически подберет формат вывода (как cout в C++)
		fmt.Printf("Координаты (X,Y) - X: %v Y: %.4v\n", x, y)
	}
}
