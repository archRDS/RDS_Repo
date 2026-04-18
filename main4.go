package main

import "fmt"

func min(a, b int) int {
	if a < b { return a }
	return b
}

func simulateGame(n, m int, arr []int) int {
	pos, pavelScore, vikaScore := 0, 0, 0
	lastMovePavel, lastMoveVika := 0, 0
	pavelTurn := true

	for pos < n {
		remaining := n - pos
		maxTake := min(m, remaining)
		bestK, maxSum := 1, -2000000000

		for k := 1; k <= maxTake; k++ {
			if !pavelTurn && k == lastMovePavel { continue }
			if pavelTurn && k == lastMoveVika { continue }

			currentSum := 0
			for i := pos; i < pos+k; i++ { currentSum += arr[i] }

			if currentSum > maxSum || (currentSum == maxSum && k < bestK) {
				maxSum, bestK = currentSum, k
			}
		}

		if pavelTurn {
			pavelScore += maxSum
			lastMovePavel = bestK
		} else {
			vikaScore += maxSum
			lastMoveVika = bestK
		}
		pos += bestK
		pavelTurn = !pavelTurn
	}
	if pavelScore > vikaScore { return 1 }
	return 0
}

func main() {
	var n, m int
	fmt.Print("Введите n и m: ")
	fmt.Scan(&n, &m)
	arr := make([]int, n)
	fmt.Print("Введите последовательность: ")
	for i := range arr { fmt.Scan(&arr[i]) }
	fmt.Println(simulateGame(n, m, arr))
}
