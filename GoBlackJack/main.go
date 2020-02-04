package main

import (
	"fmt"
	"goblackjack/deck"
)

func main() {
	d := deck.New()
	fmt.Println(d.Print())
}
