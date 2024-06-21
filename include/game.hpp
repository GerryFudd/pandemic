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
  struct GameState {
    std::map<disease::DiseaseColor, disease::DiseaseStatus> diseases;
    std::map<std::string, city::City> cities;
    std::map<std::string, city::CityState> board;
    std::vector<player::Player> players;
    std::map<player::Role, std::string> player_locations;
    card::Deck infection_deck;
    card::Deck player_deck;
    card::Hand contingency_card;
    int outbreaks;
    int infection_rate_level;
    int research_facility_reserve;
    GameState();
    int get_infection_rate(void);
  };
  class Game {
    GameState state;
    int place(std::string, disease::DiseaseColor, int);
    int place(std::string, int);
    void update_protections(std::string);
    void remove_player_with_role(player::Role);
    void place_player(player::Role, std::string);
    void move(player::Role, std::string);
    bool place_disease(std::string, disease::DiseaseColor, std::vector<std::string>&);
  public:
    static int infection_rate_escalation[INFECTION_RATE_SIZE];
    static int hand_sizes[PLAYER_COUNT_OPTIONS];
    Game();
    void setup(Difficulty, int);
    void setup();
    void add_role(player::Role); // For testing
    void remove_role(player::Role); // For testing
    void add_card(player::Role, card::Card); // For testing
    void discard(card::Card); // For testing
    GameState get_state(void);
    void place_research_facility(std::string);
    void place_research_facility(std::string, std::string);
    int get_player_count(void);
    player::Player get_player(player::Role);
    card::Card remove_player_card(player::Role, std::string);
    bool draw_infection_card(void);

    void drive(player::Role, std::string);
    void direct_flight(player::Role, std::string);
    void charter_flight(player::Role, std::string);
    void shuttle(player::Role, std::string);

    void dispatcher_direct_flight(player::Role, std::string);
    void dispatcher_charter_flight(player::Role, std::string);
    void dispatcher_conference(player::Role, player::Role);

    void treat(player::Role, disease::DiseaseColor);
    void share(player::Role, player::Role);
    void researcher_share(std::string, player::Role);
    void cure(player::Role, std::string[5]);
    void scientist_cure(std::string[4]);

    void reclaim(std::string);

    void company_plane(std::string, std::string);

    bool epidemic(void);

    bool draw_player_card(player::Role);
  };
}

#endif
