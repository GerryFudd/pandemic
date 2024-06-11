#ifndef GAME_TYPE
#define GAME_TYPE
#include <filesystem>
#include <map>
#include <string>
#include "types/disease.hpp"
#include "types/city.hpp"
#include "types/card.hpp"
#include "types/player.hpp"

#define BASE_EPIDEMIC_COUNT 4
#define INFECTION_RATE_SIZE BASE_EPIDEMIC_COUNT + 3
#define INFECTION_RATE_ESCALATION { 2, 2, 2, 3, 3, 4, 4 }
#define MIN_PLAYER_COUNT 2
#define PLAYER_COUNT_OPTIONS 3
#define HAND_SIZES { 4, 3, 2 }
#define RESEARCH_FACILITY_COUNT 6

#define CDC_LOCATION "Atlanta"

  using namespace gerryfudd::types;

namespace gerryfudd::core {
  enum Difficulty { easy, medium, hard };
  class Game {
    static int infection_rate_escalation[INFECTION_RATE_SIZE];
    static int hand_sizes[PLAYER_COUNT_OPTIONS];
    std::map<disease::DiseaseColor, disease::DiseaseStatus> diseases;
    std::map<std::string, city::City> cities;
    std::map<std::string, city::CityState> board;
    std::vector<player::Player> players;
    std::map<player::Role, std::string> player_locations;
    card::Deck infection_deck;
    card::Deck player_deck;
    int outbreaks;
    int infection_rate;
    int research_facility_reserve;
    int place(std::string, disease::DiseaseColor, int);
    int place(std::string, int);
  public:
    Game();
    void setup(Difficulty, int);
    void setup();
    int get_reserve(disease::DiseaseColor);
    bool is_cured(disease::DiseaseColor);
    city::City get_city(std::string);
    std::map<std::string, city::City>::iterator city_begin();
    std::map<std::string, city::City>::iterator city_end();
    int get_city_count(void);
    city::CityState get_state(std::string);
    std::vector<player::Player> get_players_in(std::string);
    int get_research_facility_reserve(void);
    void place_research_facility(std::string);
    void place_research_facility(std::string, std::string);
  };
}

#endif
