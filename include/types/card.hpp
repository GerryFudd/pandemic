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
  enum DeckType { player, infect };
  std::string name_of(DeckType);
  enum CardType { one_quiet_night, resilient_population, government_grant, airlift, epidemic, city };
  std::string name_of(CardType);
  struct Card {
    std::string name;
    DeckType deck_type;
    CardType type;
    Card(std::string, DeckType);
    Card(std::string, DeckType, CardType);
  };
  struct Hand {
    DeckType deck_type;
    std::vector<Card> contents;
    Hand(DeckType);
    friend std::ostream& operator<<(std::ostream&, const Hand&);
  };
  int random(int);
  class Deck {
    std::vector<Card> contents;
    std::vector<Card> discard_contents;
    DeckType deck_type;
  public:
    Deck(DeckType);
    void discard(Card);
    void shuffle(void);
    void shuffle(int, int);
    void insert(Card, int);
    Card draw(void);
    Card reveal(int);
    int size(void);
    int remaining(void);
    void clear(void);
    std::vector<Card> get_discard_contents(void);
  };
}

#endif
