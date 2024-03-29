package main

import "strconv"

// StratHard is the hard hand strategy
var stratHard = [][]string{
	{"0", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11"},
	{"2", "H", "H", "H", "H", "H", "H", "H", "H", "H", "H"},
	{"3", "H", "H", "H", "H", "H", "H", "H", "H", "H", "H"},
	{"4", "H", "H", "H", "H", "H", "H", "H", "H", "H", "H"},
	{"5", "H", "H", "H", "H", "H", "H", "H", "H", "H", "H"},
	{"6", "H", "H", "H", "H", "H", "H", "H", "H", "H", "H"},
	{"7", "H", "H", "H", "H", "H", "H", "H", "H", "H", "H"},
	{"8", "H", "H", "H", "H", "H", "H", "H", "H", "H", "H"},
	{"9", "H", "D", "D", "D", "D", "H", "H", "H", "H", "H"},
	{"10", "D", "D", "D", "D", "D", "D", "D", "D", "H", "H"},
	{"11", "D", "D", "D", "D", "D", "D", "D", "D", "D", "H"},
	{"12", "H", "H", "S", "S", "S", "H", "H", "H", "H", "H"},
	{"13", "S", "S", "S", "S", "S", "H", "H", "H", "H", "H"},
	{"14", "S", "S", "S", "S", "S", "H", "H", "H", "H", "H"},
	{"15", "S", "S", "S", "S", "S", "H", "H", "H", "H", "H"},
	{"16", "S", "S", "S", "S", "S", "H", "H", "H", "H", "H"},
	{"17", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S"},
	{"18", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S"},
	{"19", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S"},
	{"20", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S"},
	{"21", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S"},
}

var stratSoft = [][]string{
	{"0", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11"},
	{"13", "H", "H", "H", "D", "D", "H", "H", "H", "H", "H"},
	{"14", "H", "H", "H", "D", "D", "H", "H", "H", "H", "H"},
	{"15", "H", "H", "D", "D", "D", "H", "H", "H", "H", "H"},
	{"16", "H", "H", "D", "D", "D", "H", "H", "H", "H", "H"},
	{"17", "H", "D", "D", "D", "D", "H", "H", "H", "H", "H"},
	{"18", "S", "D", "D", "D", "D", "S", "S", "H", "H", "H"},
	{"19", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S"},
	{"20", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S"},
	{"21", "S", "S", "S", "S", "S", "S", "S", "S", "S", "S"},
}

var stratSplit = [][]string{
	{"0", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11"},
	{"2", "P", "P", "P", "P", "P", "P", "H", "H", "H", "H"},
	{"3", "P", "P", "P", "P", "P", "P", "H", "H", "H", "H"},
	{"4", "H", "H", "H", "P", "P", "H", "H", "H", "H", "H"},
	{"6", "P", "P", "P", "P", "P", "H", "H", "H", "H", "H"},
	{"7", "P", "P", "P", "P", "P", "P", "H", "H", "H", "H"},
	{"8", "P", "P", "P", "P", "P", "P", "P", "P", "P", "P"},
	{"9", "P", "P", "P", "P", "P", "S", "P", "P", "S", "S"},
	{"11", "P", "P", "P", "P", "P", "P", "P", "P", "P", "P"},
}

func getAction(playerVal int32, dealerVal int32, strategy *[]byte) byte {
	var key = playerVal*12 + dealerVal
	return (*strategy)[key]
}

func array2dToMap(array [][]string) []byte {
	temp := make([]byte, 300)
	for row := 0; row < len(array); row++ {
		for col := 0; col < len(array[0]); col++ {
			playerVal, _ := strconv.Atoi(array[row][0])
			dealerVal, _ := strconv.Atoi(array[0][col])
			key := playerVal*12 + dealerVal
			temp[int32(key)] = array[row][col][0]
		}
	}
	return temp
}
