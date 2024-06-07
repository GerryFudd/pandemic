#include <vector>
#include <random>
#include "types/game.hpp"
#include "data/city_data.hpp"

namespace gerryfudd::types {
  namespace player {
    std::string name_of(Role role) {
      switch (role)
      {
      case contingency_planner:
        return CONTINGENCY_PLANNER;
      case dispatcher:
        return DISPATCHER;
      case medic:
        return MEDIC;
      case operations_expert:
        return OPERATIONS_EXPERT;
      case quarantine_specialist:
        return QUARANTINE_SPECIALIST;
      case scientist:
        return SCIENTIST;
      case researcher:
        return RESEARCHER;
      default:
        throw std::invalid_argument("This role doesn't have a name.");
      }
    }
    std::string description_of(Role role) {
      switch (role)
      {
      case contingency_planner:
        return CONTINGENCY_PLANNER_DESCRIPTION;
      case dispatcher:
        return DISPATCHER_DESCRIPTION;
      case medic:
        return MEDIC_DESCRIPTION;
      case operations_expert:
        return OPERATIONS_EXPERT_DESCRIPTION;
      case quarantine_specialist:
        return QUARANTINE_SPECIALIST_DESCRIPTION;
      case scientist:
        return SCIENTIST_DESCRIPTION;
      case researcher:
        return RESEARCHER_DESCRIPTION;
      default:
        throw std::invalid_argument("This role doesn't have a description.");
      }
    }
    std::string name_of(ActionType action) {
      switch (action)
      {
      case drive:
        return DRIVE;
      case direct_flight:
        return DIRECT_FLIGHT;
      case charter_flight:
        return CHARTER_FLIGHT;
      case shuttle:
        return SHUTTLE;
      case build:
        return BUILD;
      case treat:
        return TREAT;
      case share:
        return SHARE;
      case cure:
        return CURE;
      case reclaim:
        return RECLAIM;
      case conference:
        return CONFERENCE;
      case company_plane:
        return COMPANY_PLANE;
      default:
        throw std::invalid_argument("This action doesn't have a name.");
      }
    }
    std::string description_of(ActionType action, Role role) {
      switch (action)
      {
      case drive:
        if (role == dispatcher) {
          return DRIVE_DISPATCHER_DESCRIPTION;
        }
        return DRIVE_DESCRIPTION;
      case direct_flight:
        if (role == dispatcher) {
          return DRIVE_DISPATCHER_DESCRIPTION;
        }
        return DIRECT_FLIGHT_DESCRIPTION;
      case charter_flight:
        if (role == dispatcher) {
          return CHARTER_FLIGHT_DISPATCHER_DESCRIPTION;
        }
        return CHARTER_FLIGHT_DESCRIPTION;
      case shuttle:
        if (role == dispatcher) {
          return SHUTTLE_DISPATCHER_DESCRIPTION;
        }
        return SHUTTLE_DESCRIPTION;
      case build:
        if (role == operations_expert) {
          return BUILD_OPERATIONS_EXPERT_DESCRIPTION;
        }
        return BUILD_DESCRIPTION;
      case treat:
        if (role == medic) {
          return TREAT_MEDIC_DESCRIPTION;
        }
        return TREAT_DESCRIPTION;
      case share:
        if (role == researcher) {
          return SHARE_RESEARCHER_DESCRIPTION;
        }
        return SHARE_DESCRIPTION;
      case cure:
        if (role == scientist) {
          return CURE_SCIENTIST_DESCRIPTION;
        }
        return CURE_DESCRIPTION;
      case reclaim:
        if (role == contingency_planner) {
          return RECLAIM_CONTINGENCY_PLANNER_DESCRIPTION;
        }
      case conference:
        if (role == dispatcher) {
          return CONFERENCE_DISPATCHER_DESCRIPTION;
        }
      case company_plane:
        if (role == operations_expert) {
          return COMPANY_PLANE_OPERATIONS_EXPERT_DESCRIPTION;
        }
      default:
        throw std::invalid_argument("This is an invalid combination of role and action.");
      }
    }
    std::vector<ActionType> get_actions(Role role) {
      std::vector<ActionType> result;
      result.push_back(drive);
      result.push_back(direct_flight);
      result.push_back(charter_flight);
      result.push_back(shuttle);
      result.push_back(build);
      result.push_back(treat);
      result.push_back(share);
      result.push_back(cure);
      switch(role) {
        case contingency_planner:
          result.push_back(reclaim);
          break;
        case dispatcher:
          result.push_back(conference);
          break;
        case operations_expert:
          result.push_back(company_plane);
          break;
        default:
          break;
      }
      return result;
    }
    std::vector<Role> get_roles(int player_count) {
      std::vector<Role> result;
      player::Role first = player::contingency_planner;
      std::vector<Role> remaining;
      remaining.push_back(contingency_planner);
      remaining.push_back(dispatcher);
      remaining.push_back(medic);
      remaining.push_back(operations_expert);
      remaining.push_back(quarantine_specialist);
      remaining.push_back(scientist);
      remaining.push_back(researcher);
      while (remaining.size() < 7) {
        remaining.push_back(*(&first + remaining.size()));
      }
      int i;
      while (result.size() < player_count) {
        i = card::random(remaining.size());
        result.push_back(remaining[i]);
        remaining.erase(remaining.begin() + i);
      }
      return result;
    }

    Player::Player(Role role): role{role}, hand{card::Hand(card::player)} {}
  }

  int Game::infection_rate_escalation[] = INFECTION_RATE_ESCALATION;
  int Game::hand_sizes[] = HAND_SIZES;
  Game::Game(): infection_deck{card::infect}, player_deck{card::player}, outbreaks{0}, infection_rate{0}, research_facility_reserve{RESEARCH_FACILITY_COUNT} {}

  int Game::place(std::string target, disease::DiseaseColor color, int quantity) {
    if (board[target].prevent_placement) {
      return 0;
    }
    diseases[color].reserve -= quantity;
    board[target].disease_count[color] += quantity;
    return quantity;
  }

  int Game::place(std::string target, int quantity) {
    return place(target, cities[target].color, quantity);
  }

  void Game::setup() {
    setup(easy, MIN_PLAYER_COUNT);
  }

  void Game::setup(Difficulty difficulty, int player_count) {
    diseases.clear();
    cities.clear();
    board.clear();
    infection_deck.clear();
    player_deck.clear();
    players.clear();
    player_locations.clear();

    data::city::load_cities(&cities);
    for (std::map<std::string, city::City>::iterator cursor = cities.begin(); cursor != cities.end(); ++cursor) {
      board[cursor->first] = city::CityState();
      infection_deck.discard(card::Card(cursor->first, card::infect));
      player_deck.discard(card::Card(cursor->first, card::player));
    }
    place_research_facility(CDC_LOCATION);

    infection_deck.shuffle();

    place(infection_deck.draw().name, 1);
    place(infection_deck.draw().name, 1);
    place(infection_deck.draw().name, 1);

    place(infection_deck.draw().name, 2);
    place(infection_deck.draw().name, 2);
    place(infection_deck.draw().name, 2);

    place(infection_deck.draw().name, 3);
    place(infection_deck.draw().name, 3);
    place(infection_deck.draw().name, 3);
    
    player_deck.discard(card::Card(ONE_QUIET_NIGHT, card::player));
    player_deck.discard(card::Card(RESILIENT_POPULATION, card::player));
    player_deck.discard(card::Card(GOVERNMENT_GRANT, card::player));
    player_deck.discard(card::Card(AIRLIFT, card::player));

    player_deck.shuffle();
    int initial_hand_size = hand_sizes[player_count - MIN_PLAYER_COUNT];
    std::vector<player::Role> roles = player::get_roles(player_count);
    for (std::vector<player::Role>::iterator cursor = roles.begin(); cursor != roles.end(); cursor++) {
      players.push_back(player::Player(*cursor));
      while (players.back().hand.contents.size() < initial_hand_size) {
        players.back().hand.contents.push_back(player_deck.draw());
      }
      player_locations[*cursor] = CDC_LOCATION;
    }
    int epidemics = BASE_EPIDEMIC_COUNT + difficulty;
    int cards_per_epidemic = (player_deck.remaining() + epidemics) / epidemics;
    for (int i = 0; i < epidemics; i++) {
      player_deck.insert(card::Card(EPIDEMIC, card::player, false), cards_per_epidemic * i);
      player_deck.shuffle(cards_per_epidemic * i, std::min(cards_per_epidemic * (i+1), player_deck.remaining()));
    }
  }

  int Game::get_reserve(disease::DiseaseColor disease_color) {
    return diseases[disease_color].reserve;
  }

  bool Game::is_cured(disease::DiseaseColor disease_color) {
    return diseases[disease_color].cured;
  }

  city::City Game::get_city(std::string city_name) {
    return cities[city_name];
  }

  std::map<std::string, city::City>::iterator Game::city_begin() {
    return cities.begin();
  }

  std::map<std::string, city::City>::iterator Game::city_end() {
    return cities.end();
  }

  int Game::get_city_count() {
    return cities.size();
  }

  city::CityState Game::get_state(std::string city_name) {
    return board[city_name];
  }

  std::vector<player::Player> Game::get_players_in(std::string city_name) {
    std::vector<player::Player> result;
    for (std::vector<player::Player>::iterator cursor = players.begin(); cursor != players.end(); cursor++) {
      if (player_locations[cursor->role] == city_name) {
        result.push_back(*cursor);
      }
    }
    return result;
  }

  int Game::get_research_facility_reserve() {
    return research_facility_reserve;
  }

  void Game::place_research_facility(std::string city_name) {
    board[city_name].research_facility = true;
    research_facility_reserve--;
  }

  void Game::place_research_facility(std::string city_name, std::string source_city_name) {
    board[city_name].research_facility = true;
    board[source_city_name].research_facility = false;
  }
}
