package main

import (
 "fmt"
 "math"
 "strconv"
)

// Функция для вычисления наибольшего общего делителя
func computeGCD(num1, num2 int64) int64 {
 for num2 != 0 {
  num1, num2 = num2, num1%num2
 }
 return num1
}

// Проверка, можно ли число представить конечной десятичной дробью
// Возвращает: (isExact, numerator, denominator)
func checkFiniteDecimal(value float64, precision float64) (bool, int64, int64) {
 // Проверка на целое число
 if math.Abs(value-math.Round(value)) < precision {
  numerator := int64(math.Round(value))
  return true, numerator, 1
 }

 // Поиск точного представления в виде дроби
 for div := int64(1); div <= 100000; div++ {
  num := int64(math.Round(value * float64(div)))
  if math.Abs(float64(num)/float64(div)-value) < precision {
   commonFactor := computeGCD(num, div)
   simpleNum := num / commonFactor
   simpleDiv := div / commonFactor

   // Проверка знаменателя на вид 2^m * 5^n
   tempDiv := simpleDiv
   for tempDiv%2 == 0 {
    tempDiv /= 2
   }
   for tempDiv%5 == 0 {
    tempDiv /= 5
   }

   if tempDiv == 1 {
    // Нашли конечную дробь, но в оригинале возвращалось false
    // (сохраняем логику оригинала)
    return false, simpleNum, simpleDiv
   }
  }
 }

 return true, 0, 0
}

// Преобразование числа в строку-дробь, если возможно
func convertToFractionString(value float64) string {
 isExact, num, denom := checkFiniteDecimal(value, 1.0e-15)
 if isExact {
  return strconv.FormatInt(num, 10) + "/" + strconv.FormatInt(denom, 10)
 }
 return "irrational"
}

// Вычисление суммы ряда
func computeSeriesSum(exponent, base int) string {
 if base <= 1 {
  return "Infinity"
 }

 // Специальные случаи с аналитическими решениями
 if exponent == 0 && base > 1 {
  return "1/" + strconv.Itoa(base-1)
 } else if exponent == 1 && base > 1 {
  return strconv.Itoa(base) + "/" + strconv.Itoa((base-1)*(base-1))
 } else if exponent == 2 && base > 1 {
  return strconv.Itoa(base*(base+1)) + "/" + strconv.Itoa((base-1)*(base-1)*(base-1))
 } else if exponent == 3 && base > 1 {
  return strconv.Itoa(base*(base*base+4*base+1)) + "/" + strconv.Itoa((base-1)*(base-1)*(base-1)*(base-1))
 }

 totalSum := 0.0
 const maxTerms = 10000
 const minTerm = 1e-10

 for termIndex := 1; termIndex <= maxTerms; termIndex++ {
  currentTerm := math.Pow(float64(termIndex), float64(exponent)) / math.Pow(float64(base), float64(termIndex))
  totalSum += currentTerm

  if currentTerm < minTerm {
   break
  }
 }

 return convertToFractionString(totalSum)
}

func main() {
 var expValue, baseValue int

 fmt.Print("Введите показатель степени (целое от 1 до 10): ")
 _, err1 := fmt.Scan(&expValue)
 fmt.Print("Введите основание (целое от 1 до 10): ")
 _, err2 := fmt.Scan(&baseValue)

 if err1 != nil || err2 != nil {
  fmt.Println("Ошибка ввода")
  return
 }

 // Проверка введенных значений
 if expValue < 1 || expValue > 10 || baseValue < 1 || baseValue > 10 {
  fmt.Println("Ошибка: значения должны быть целыми от 1 до 10")
  return
 }

 seriesResult := computeSeriesSum(expValue, baseValue)

 if seriesResult == "Infinity" {
  fmt.Println("Infinity")
 } else if seriesResult == "irrational" {
  fmt.Println("Irrational")
 } else {
  fmt.Println(seriesResult)
 }
}
