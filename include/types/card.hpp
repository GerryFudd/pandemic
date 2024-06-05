#ifndef CARD_TYPE
#define CARD_TYPE
#include <string>
#include <vector>

namespace gerryfudd::types::card {
  enum CardType { player, infect };
  struct Card {
    std::string name;
    CardType type;
    Card(std::string, CardType);
  };
  class Deck {
    std::vector<Card> contents;
    std::vector<Card> discard_contents;
    CardType type;
  public:
    Deck(CardType);
    void discard(Card);
    void shuffle(void);
    Card draw(void);
    Card reveal(int);
    int size(void);
    int remaining(void);
  };
}

#endif
