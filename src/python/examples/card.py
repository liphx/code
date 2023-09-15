import collections
from random import choice

Card = collections.namedtuple('Card', ['rank', 'suit'])


class FrenchDeck:
    ranks = [str(n) for n in range(2, 11)] + list('JQKA')
    suits = 'spades diamonds clubs hearts'.split()

    def __init__(self):
        self._cards = [Card(rank, suit) for suit in self.suits
                       for rank in self.ranks]

    def __len__(self):
        return len(self._cards)

    def __getitem__(self, position):
        return self._cards[position]


deck = FrenchDeck()
print(len(deck))            # 52
print(deck[0])              # Card(rank='2', suit='spades')
print(deck[-1])             # Card(rank='A', suit='hearts')

card = choice(deck)
print(card)

print(deck[:3])             # 前3张牌
print(deck[12::13])         # 牌面为A的牌

for card in deck:           # 可迭代
    print(card)

for card in reversed(deck):  # 反向迭代
    print(card)

print(Card('Q', 'hearts') in deck)  # True
print(Card('Z', 'hearts') in deck)  # False

# 排序
suit_values = dict(spades=3, hearts=2, diamonds=1, clubs=0)


def spades_high(card):
    rank_value = FrenchDeck.ranks.index(card.rank)
    return rank_value * len(suit_values) + suit_values[card.suit]


for card in sorted(deck, key=spades_high):
    print(card)
