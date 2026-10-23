import random
from deck import Deck


class CardPile:
    def __init__(self, numofdecks):
        self.cards = []
        for _ in range(0, numofdecks):
            self.cards += Deck().cards
        self.original_cards = self.cards.copy()

    def print(self):
        string = ""
        for card in self.cards:
            string += str(card.print() + "\n")
        return string

    def shuffle(self):
        cards = self.cards
        getrandbits = random.getrandbits
        for i in range(len(cards) - 1, 0, -1):
            n = i + 1
            k = n.bit_length()
            j = getrandbits(k)
            while j >= n:
                j = getrandbits(k)
            cards[i], cards[j] = cards[j], cards[i]

    def refresh(self):
        self.cards = list(self.original_cards)
