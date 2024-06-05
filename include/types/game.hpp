#ifndef GAME_TYPE
#define GAME_TYPE
#include <types/disease.hpp>
#include <types/city.hpp>
#include <filesystem>
#include <map>
#include <string>
#include <vector>

namespace gerryfudd::types {
  namespace card {
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

  class Game {
    std::map<disease::DiseaseColor, disease::DiseaseStatus> diseases;
    std::map<std::string, city::City> cities;
    std::map<std::string, city::CityState> board;
    card::Deck infection_deck;
    int place(std::string, disease::DiseaseColor, int);
    int place(std::string, int);
  public:
    Game();
    void setup(std::filesystem::path);
    int get_reserve(disease::DiseaseColor);
    bool is_cured(disease::DiseaseColor);
    city::City get_city(std::string);
    std::map<std::string, city::City>::iterator city_begin();
    std::map<std::string, city::City>::iterator city_end();
    int get_city_count(void);
    city::CityState get_state(std::string);
  };
}

#endif
