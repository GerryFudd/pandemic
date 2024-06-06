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
#define MIN_PLAYER_COUNT 2
#define PLAYER_COUNT_OPTIONS 3
#define HAND_SIZES { 4, 3, 2 }

namespace gerryfudd::types {
  namespace player {
    enum Role { contingency_planner, dispatcher, medic, operations_expert, quarantine_specialist, scientist, researcher };
    std::string name_of(Role);
    std::string description_of(Role);
    std::vector<Role> get_roles(int);
    enum ActionType { drive, direct_flight, charter_flight, shuttle, build, treat, share, cure, reclaim, conference, company_plane };
    std::string name_of(ActionType);
    std::string description_of(ActionType, Role);
    std::vector<ActionType> get_actions(Role);
  }
  enum Difficulty { easy, medium, hard };
  class Game {
    static int infection_rate_escalation[INFECTION_RATE_SIZE];
    static int hand_sizes[PLAYER_COUNT_OPTIONS];
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
    void setup(Difficulty, int);
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
