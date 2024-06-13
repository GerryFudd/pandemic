#include <vector>
#include "game.hpp"
#include "data/city_data.hpp"

namespace gerryfudd::core {
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
      player_deck.insert(card::Card(EPIDEMIC, card::player, card::epidemic), cards_per_epidemic * i);
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
