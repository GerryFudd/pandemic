#ifndef CARD_TYPE
#define CARD_TYPE
#include <string>
#include <vector>

#define ONE_QUIET_NIGHT "One Quiet Night"
#define RESILIENT_POPULATION "Resilient Population"
#define GOVERNMENT_GRANT "Government Grant"
#define AIRLIFT "Airlift"
#define EPIDEMIC "Epidemic"

namespace gerryfudd::types::card {
  enum CardType { player, infect };
  std::string name_of(CardType);
  struct Card {
    std::string name;
    CardType type;
    bool city;
    Card(std::string, CardType);
    Card(std::string, CardType, bool);
  };
  struct Hand {
    CardType type;
    std::vector<Card> contents;
    Hand(CardType);
    friend std::ostream& operator<<(std::ostream&, const Hand&);
  };
  class Deck {
    std::vector<Card> contents;
    std::vector<Card> discard_contents;
    CardType type;
  public:
    Deck(CardType);
    void discard(Card);
    void shuffle(void);
    void shuffle(int, int);
    void insert(Card, int);
    Card draw(void);
    Card reveal(int);
    int size(void);
    int remaining(void);
  };
}

#endif
