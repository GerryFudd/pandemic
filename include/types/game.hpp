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

#define CONTINGENCY_PLANNER "Contingency Planner"
#define DISPATCHER "Dispatcher"
#define MEDIC "Medic"
#define OPERATIONS_EXPERT "Operations Expert"
#define QUARANTINE_SPECIALIST "Quarantine Specialist"
#define SCIENTIST "Scientist"
#define RESEARCHER "Researcher"

#define CONTINGENCY_PLANNER_DESCRIPTION "As an action you may move a discarded event card to your role card. You may play event cards from your role card as though they were in your hand, but then they are removed from the game. You may only have one event card on your role card at a time."
#define DISPATCHER_DESCRIPTION "You may move other players' pawns as though they were your own. You may also move another players' pawn to a city with another pawn in it.\nYou must get permission from a pawn's owner before moving it."
#define MEDIC_DESCRIPTION "When treating a disease, remove all cubes of a single color from the city you are in. If a disease is cured, remove all cubes of that color from any city you are in without taking any action."
#define OPERATIONS_EXPERT_DESCRIPTION "Placing a research facility doesn't require discarding a city card. You may move to any city from a research facility by discarding any city card."
#define QUARANTINE_SPECIALIST_DESCRIPTION "No disease cubes are placed in the city where your pawn is or in any city connected to that city."
#define SCIENTIST_DESCRIPTION "You only need to discard four cards of a single color, rather than five, to cure a disease."
#define RESEARCHER_DESCRIPTION "You may pass any city card to another player as long as your pawn is in the same city as their pawn."

#define DRIVE "Drive / Ferry"
#define DIRECT_FLIGHT "Direct Flight"
#define CHARTER_FLIGHT "Charter Flight"
#define SHUTTLE "Shuttle Flight"
#define BUILD "Build a Research Station"
#define TREAT "Treat Disease"
#define SHARE "Share Knowledge"
#define CURE "Discover a Cure"
#define RECLAIM "Reclaim a discarded card"
#define CONFERENCE "Arrange a conference"
#define COMPANY_PLANE "Use the company plane"

#define DRIVE_DESCRIPTION "Move to a city connected by a white line to the one you are in.\n\nSome cities are connected to cities on the other side of the world. For example, Sydney and Los Angeles are connected."
#define DRIVE_DISPATCHER_DESCRIPTION "Move any pawn, with the consent of its owner, to a city connected by a white line to the one it is in.\n\nSome cities are connected to cities on the other side of the world. For example, Sydney and Los Angeles are connected."
#define DIRECT_FLIGHT_DESCRIPTION "Discard a City card to move to the city named on the card."
#define DIRECT_FLIGHT_DISPATCHER_DESCRIPTION "Discard a City card to move any pawn, with the consent of its owner, to the city named on the card."
#define CHARTER_FLIGHT_DESCRIPTION "Discard the City card that matches the city you are in to move to any city."
#define CHARTER_FLIGHT_DISPATCHER_DESCRIPTION "Discard the City card that matches the city any pawn is in to move it, with the consent of its owner, to any city."
#define SHUTTLE_DESCRIPTION "Move from a city with a research station to any other city that has a research station."
#define SHUTTLE_DISPATCHER_DESCRIPTION "Move any pawn, with the consent of its owner, from a city with a research station to any other city that has a research station."
#define BUILD_DESCRIPTION "Discard the City card that matches the city you are in to place a research station there. Take the research station from the pile next to the board. If all 6 research stations have been built, take a research station from anywhere on the board."
#define BUILD_OPERATIONS_EXPERT_DESCRIPTION "Place a research station in the city you are in. Take the research station from the pile next to the board. If all 6 research stations have been built, take a research station from anywhere on the board."
#define TREAT_DESCRIPTION "Remove 1 disease cube from the city you are in, placing it in the cube supply next to the board. If this disease color has been cured (see Discover a Cure below), remove all cubes of that color from the city you are in. If the last cube of a cured disease is removed from the board, this disease is eradicated."
#define TREAT_MEDIC_DESCRIPTION "Remove all cubes of one color from the city you are in. If the last cube of a cured disease is removed from the board, this disease is eradicated. Flip its cure marker from its blank side to its crossed out side."
#define SHARE_DESCRIPTION "You can do this action in two ways:\n\t-give the City card that matches the city you are in to another player, or\n\t-take a City card from another player. This card must match the city you are in unless the other player is the Researcher.\n\nThe other player must also be in the city with you. Both of you need to agree to do this."
#define SHARE_RESEARCHER_DESCRIPTION "You can do this action in two ways:\n\t-give any City card to another player, or\n\t-take the City card that matches the city you are in from another player.\n\nThe other player must also be in the city with you. Both of you need to agree to do this."
#define CURE_DESCRIPTION "At any research station, discard 5 City cards of the same color from your hand to cure the disease of that color. Move the disease's cure marker to its Cure Indicator.\nIf no cubes of this color are on the board, this disease is now eradicated.\n\nWhen a disease is cured, its cubes remain on the board and new cubes can still be placed during epidemics or infections. However, treating this disease is now easier and you are closer to winning."
#define CURE_SCIENTIST_DESCRIPTION "At any research station, discard 4 City cards of the same color from your hand to cure the disease of that color. Move the disease's cure marker to its Cure Indicator.\nIf no cubes of this color are on the board, this disease is now eradicated.\n\nWhen a disease is cured, its cubes remain on the board and new cubes can still be placed during epidemics or infections. However, treating this disease is now easier and you are closer to winning."
#define RECLAIM_CONTINGENCY_PLANNER_DESCRIPTION "Move a discarded event card from the discard pile to your role card. You may use this event card at any time as though it were in your hand. When you use this discarded card, remove it from the game."
#define CONFERENCE_DISPATCHER_DESCRIPTION "Move any pawn to the city where another pawn already is."
#define COMPANY_PLANE_OPERATIONS_EXPERT_DESCRIPTION "If you are in a city with a research facility, discard any city card to move to any city."

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
