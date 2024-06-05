#ifndef GAME_TYPE
#define GAME_TYPE
#include <types/disease.hpp>
#include <types/city.hpp>
#include <types/card.hpp>
#include <filesystem>
#include <map>
#include <string>

namespace gerryfudd::types {
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
