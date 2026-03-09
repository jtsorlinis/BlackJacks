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
        random.shuffle(self.cards)

    def refresh(self):
        self.cards = list(self.original_cards)
