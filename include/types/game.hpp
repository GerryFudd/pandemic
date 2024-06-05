#ifndef GAME_TYPE
#define GAME_TYPE
#include <types/disease.hpp>
#include <types/city.hpp>
#include <types/card.hpp>
#include <filesystem>
#include <map>
#include <string>

#define BASE_EPIDEMIC_COUNT 4
#define INFECTION_RATE_SIZE BASE_EPIDEMIC_COUNT + 3
#define INFECTION_RATE_ESCALATION { 2, 2, 2, 3, 3, 4, 4 }

namespace gerryfudd::types {
  enum Difficulty { easy, medium, hard };
  class Game {
    static int infection_rate_escalation[INFECTION_RATE_SIZE];
    std::map<disease::DiseaseColor, disease::DiseaseStatus> diseases;
    std::map<std::string, city::City> cities;
    std::map<std::string, city::CityState> board;
    card::Deck infection_deck;
    card::Deck player_deck;
    int outbreaks;
    int infection_rate;
    int place(std::string, disease::DiseaseColor, int);
    int place(std::string, int);
  public:
    Game();
    void setup(Difficulty);
    void setup();
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
