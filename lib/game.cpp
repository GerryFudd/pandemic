#include <vector>
#include <algorithm>
#include <iostream>
#include <stdexcept>
#include "game.hpp"
#include "data/city_data.hpp"

namespace gerryfudd::core {
  int Game::infection_rate_escalation[] = INFECTION_RATE_ESCALATION;
  int Game::hand_sizes[] = HAND_SIZES;
  Game::Game(): infection_deck{card::infect}, player_deck{card::player}, contingency_card{card::player}, outbreaks{0}, infection_rate{0}, research_facility_reserve{RESEARCH_FACILITY_COUNT} {}

  int Game::place(std::string target, disease::DiseaseColor color, int quantity) {
    if (board[target].prevent_placement(color)) {
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

    infection_deck.discard(infection_deck.draw());
    place(infection_deck.get_discard_contents().back().name, 1);
    infection_deck.discard(infection_deck.draw());
    place(infection_deck.get_discard_contents().back().name, 1);
    infection_deck.discard(infection_deck.draw());
    place(infection_deck.get_discard_contents().back().name, 1);


    infection_deck.discard(infection_deck.draw());
    place(infection_deck.get_discard_contents().back().name, 2);
    infection_deck.discard(infection_deck.draw());
    place(infection_deck.get_discard_contents().back().name, 2);
    infection_deck.discard(infection_deck.draw());
    place(infection_deck.get_discard_contents().back().name, 2);


    infection_deck.discard(infection_deck.draw());
    place(infection_deck.get_discard_contents().back().name, 3);
    infection_deck.discard(infection_deck.draw());
    place(infection_deck.get_discard_contents().back().name, 3);
    infection_deck.discard(infection_deck.draw());
    place(infection_deck.get_discard_contents().back().name, 3);
    
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
      place_player(*cursor, CDC_LOCATION);
    }
    int epidemics = BASE_EPIDEMIC_COUNT + difficulty;
    int cards_per_epidemic = (player_deck.remaining() + epidemics) / epidemics;
    for (int i = 0; i < epidemics; i++) {
      player_deck.insert(card::Card(EPIDEMIC, card::player, card::epidemic), cards_per_epidemic * i);
      player_deck.shuffle(cards_per_epidemic * i, std::min(cards_per_epidemic * (i+1), player_deck.remaining()));
    }
  }

  void Game::update_protections(std::string location) {
    board[location].protected_colors.resize(0);
    if (player_locations[player::quarantine_specialist] == location) {
      board[location].protected_colors.push_back(disease::black);
      board[location].protected_colors.push_back(disease::blue);
      board[location].protected_colors.push_back(disease::red);
      board[location].protected_colors.push_back(disease::yellow);

      for (std::vector<city::City>::iterator cursor = cities[location].neighbors.begin(); cursor != cities[location].neighbors.end(); cursor++) {
        board[cursor->name].protected_colors.resize(0);
        board[cursor->name].protected_colors.push_back(disease::black);
        board[cursor->name].protected_colors.push_back(disease::blue);
        board[cursor->name].protected_colors.push_back(disease::red);
        board[cursor->name].protected_colors.push_back(disease::yellow);
      }
    } else if (player_locations[player::medic] == location) {
      if (diseases[disease::black].cured) {
        board[location].protected_colors.push_back(disease::black);
      }
      if (diseases[disease::blue].cured) {
        board[location].protected_colors.push_back(disease::blue);
      }
      if (diseases[disease::red].cured) {
        board[location].protected_colors.push_back(disease::red);
      }
      if (diseases[disease::yellow].cured) {
        board[location].protected_colors.push_back(disease::yellow);
      }
    }
  }

  void Game::remove_player_with_role(player::Role role) {
    std::string player_location = player_locations[role];

    player_locations.erase(player_locations.find(role));

    update_protections(player_location);
    if (role == player::quarantine_specialist) {
      for (std::vector<city::City>::iterator cursor = cities[player_location].neighbors.begin(); cursor != cities[player_location].neighbors.end(); cursor++) {
        update_protections(cursor->name);
      }
    }
  }
  void Game::place_player(player::Role role, std::string destination) {
    player_locations[role] = destination;
    update_protections(destination);
  }
  void Game::move(player::Role role, std::string destination) {
    remove_player_with_role(role);
    place_player(role, destination);
  }
  void Game::add_role(player::Role role) {
    for (std::vector<player::Player>::iterator cursor = players.begin(); cursor != players.end(); cursor++) {
      if (cursor->role == role) {
        return;
      }
    }
    player::Role role_to_replace = players.back().role;
    card::Hand hand = players.back().hand;
    remove_player_with_role(role_to_replace);
    players.pop_back();
    players.push_back(player::Player(role));
    players.back().hand = hand;
    place_player(role, CDC_LOCATION);
  }
  void Game::remove_role(player::Role role) {
    std::vector<player::Role> unused_roles;
    unused_roles.push_back(player::contingency_planner);
    unused_roles.push_back(player::dispatcher);
    unused_roles.push_back(player::medic); 
    unused_roles.push_back(player::operations_expert); 
    unused_roles.push_back(player::quarantine_specialist); 
    unused_roles.push_back(player::scientist); 
    unused_roles.push_back(player::researcher);
    std::vector<player::Player>::iterator to_replace = players.end();
    for (std::vector<player::Player>::iterator cursor = players.begin(); cursor != players.end(); cursor++) {
      unused_roles.erase(std::find(unused_roles.begin(), unused_roles.end(), cursor->role));
      if (cursor->role == role) {
        to_replace = cursor;
      }
    }
    if (to_replace == players.end()) {
      // There is nothing to do.
      return;
    }
    player::Role role_to_insert = unused_roles.back();
    card::Hand hand = to_replace->hand;
    remove_player_with_role(role);
    players.erase(to_replace);
    players.push_back(player::Player(role_to_insert));
    players.back().hand = hand;
    place_player(role_to_insert, CDC_LOCATION);
  }
  void Game::add_card(player::Role role, card::Card card) {
    for (std::vector<player::Player>::iterator player_cursor = players.begin(); player_cursor != players.end(); player_cursor++) {
      if (player_cursor->role == role) {
        if (card::contains(player_cursor->hand, card.name)) {
          return;
        }
        player_cursor->hand.contents.push_back(card);
        return;
      }
    }
  }
  void Game::discard(card::Card card) {
    switch (card.deck_type)
    {
    case card::player:
      player_deck.discard(card);
      break;
    case card::infect:
      infection_deck.discard(card);
      break;
    default:
      throw std::invalid_argument("");
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
  std::string Game::get_location(player::Role role) {
    return player_locations[role];
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

  int Game::get_player_count() {
    return players.size();
  }
  player::Player Game::get_player(player::Role role) {
    for (std::vector<player::Player>::iterator cursor = players.begin(); cursor != players.end(); cursor++) {
      if (cursor->role == role) {
        return *cursor;
      }
    }
    throw std::invalid_argument("There is no player with this role");
  }
  player::Player Game::get_player(int n) {
    return players[n % players.size()];
  }
  city::City Game::get_city(player::Role role) {
    return cities[player_locations[role]];
  }

  int Game::get_infection_rate() {
    return infection_rate_escalation[infection_rate];
  }
  disease::DiseaseStatus Game::get_status(disease::DiseaseColor color) {
    return diseases[color];
  }
  bool Game::place_disease(std::string city_name, disease::DiseaseColor color, std::vector<std::string>& executed_outbreaks) {
    if (board[city_name].prevent_placement(color)) {
      return false;
    }
    if (diseases[color].reserve <= 0) {
      return true;
    }
    if (board[city_name].disease_count[color] >= 3) {
      for (std::vector<std::string>::iterator cursor = executed_outbreaks.begin(); cursor != executed_outbreaks.end(); cursor++) {
        if (city_name == *cursor) {
          return false;
        }
      }
      if (++outbreaks >= 10) {
        return true;
      }
      executed_outbreaks.push_back(city_name);
      for (std::vector<city::City>::iterator cursor = cities[city_name].neighbors.begin(); cursor != cities[city_name].neighbors.end(); cursor++) {
        if (place_disease(cursor->name, color, executed_outbreaks)) {
          return true;
        }
      }
      return false;
    }
    diseases[color].reserve--;
    board[city_name].disease_count[color]++;
    return false;
  }
  bool Game::draw_infection_card() {
    std::vector<std::string> executed_outbreaks;
    card::Card infection_card = infection_deck.draw();
    if (place_disease(infection_card.name, cities[infection_card.name].color, executed_outbreaks)) {
      return true;
    }
    infection_deck.discard(infection_card);
    return false;
  }
  card::Card Game::remove_player_card(player::Role role, std::string card_name) {
    for (std::vector<player::Player>::iterator player_cursor = players.begin(); player_cursor != players.end(); player_cursor++) {
      if (player_cursor->role == role) {
        for (std::vector<card::Card>::iterator card_cursor = player_cursor->hand.contents.begin(); card_cursor != player_cursor->hand.contents.end(); card_cursor++) {
          if (card_cursor->name == card_name) {
            card::Card result = *card_cursor;
            player_cursor->hand.contents.erase(card_cursor);
            return result;
          }
        }
        break;
      }
    }
    throw std::invalid_argument("This player doesn't have this card.");
  }
  std::vector<card::Card> Game::get_infection_discard() {
    return infection_deck.get_discard_contents();
  }
  std::vector<card::Card> Game::get_player_discard() {
    return player_deck.get_discard_contents();
  }

  void Game::drive(player::Role role, std::string destination) {
    std::string origin = player_locations[role];
    for (std::vector<city::City>::iterator cursor = cities[origin].neighbors.begin(); cursor != cities[origin].neighbors.end(); cursor++) {
      if (cursor->name == destination) {
        move(role, destination);
        return;
      }
    }
    throw std::invalid_argument("This player can't drive to " + destination);
  }

  void Game::direct_flight(player::Role role, std::string destination) {
    player_deck.discard(remove_player_card(role, destination));
    move(role, destination);
  }

  void Game::charter_flight(player::Role role, std::string destination) {
    player_deck.discard(remove_player_card(role, player_locations[role]));
    move(role, destination);
  }
  void Game::shuttle(player::Role role, std::string destination) {
    if (!board[player_locations[role]].research_facility || !board[destination].research_facility) {
      throw std::invalid_argument("You may only shuttle between research facilities.");
    }
    move(role, destination);
  }

  void Game::dispatcher_direct_flight(player::Role role, std::string destination) {
    player_deck.discard(remove_player_card(player::dispatcher, destination));
    move(role, destination);
  }
  void Game::dispatcher_charter_flight(player::Role role, std::string destination) {
    player_deck.discard(remove_player_card(player::dispatcher, player_locations[role]));
    move(role, destination);
  }
  void Game::dispatcher_conference(player::Role guest, player::Role host) {
    move(guest, player_locations[host]);
  }

  void Game::treat(player::Role role, disease::DiseaseColor color) {
    if (diseases[color].cured || role == player::medic) {
      diseases[color].reserve += board[player_locations[role]].disease_count[color];
      board[player_locations[role]].disease_count[color] = 0;
    } else if (board[player_locations[role]].disease_count[color] > 0) {
      diseases[color].reserve++;
      board[player_locations[role]].disease_count[color]--;
    }
  }
  void Game::share(player::Role source, player::Role target) {
    if (player_locations[source] != player_locations[target]) {
      throw std::invalid_argument("Players must be in the same city to share cards.");
    }
    add_card(target, remove_player_card(source, player_locations[source]));
  }
  void Game::researcher_share(std::string card_name, player::Role target) {
    if (player_locations[player::researcher] != player_locations[target]) {
      throw std::invalid_argument("Players must be in the same city to share cards.");
    }
    add_card(target, remove_player_card(player::researcher, card_name));
  }
  void Game::cure(player::Role role, std::string matching_cards[5]) {
    if (!board[player_locations[role]].research_facility) {
      throw std::invalid_argument("Curing a disease may only occur in a city with a research facility.");
    }
    disease::DiseaseColor disease_to_cure;
    int i;
    for (i = 0; i < 5; i++) {
      if (i == 0) {
        disease_to_cure = cities[matching_cards[i]].color;
      } else if (disease_to_cure != cities[matching_cards[i]].color) {
        throw std::invalid_argument("Curing a disease requires that all discarded cards share a color.");
      }
      if (!card::contains(get_player(role).hand, matching_cards[i])) {
        throw std::invalid_argument("This player doesn't have this card.");
      }
    }
    for (i = 0; i < 5; i++) {
      player_deck.discard(remove_player_card(role, matching_cards[i]));
    }
    diseases[disease_to_cure].cured = true;
  }
  void Game::scientist_cure(std::string matching_cards[4]) {
    if (!board[player_locations[player::scientist]].research_facility) {
      throw std::invalid_argument("Curing a disease may only occur in a city with a research facility.");
    }
    disease::DiseaseColor disease_to_cure;
    int i;
    for (i = 0; i < 4; i++) {
      if (i == 0) {
        disease_to_cure = cities[matching_cards[i]].color;
      } else if (disease_to_cure != cities[matching_cards[i]].color) {
        throw std::invalid_argument("Curing a disease requires that all discarded cards share a color.");
      }
      if (!card::contains(get_player(player::scientist).hand, matching_cards[i])) {
        throw std::invalid_argument("This player doesn't have this card.");
      }
    }
    for (i = 0; i < 4; i++) {
      player_deck.discard(remove_player_card(player::scientist, matching_cards[i]));
    }
    diseases[disease_to_cure].cured = true;
  }
  card::Hand Game::get_contingency_card() {
    return contingency_card;
  }
  void Game::reclaim(std::string event_card) {
    if (player_locations[player::contingency_planner] == "") {
      throw std::invalid_argument("Reclaim is not usable without a Contingency Planner.");
    }
    card::Card discarded_card = player_deck.remove_from_discard(event_card);
    if (discarded_card.type == card::city) {
      player_deck.discard(discarded_card);
      throw std::invalid_argument("City cards may not be reclaimed.");
    }
    contingency_card.contents.resize(0, discarded_card);
    contingency_card.contents.push_back(discarded_card);
  }
  void Game::company_plane(std::string destination, std::string to_discard) {
    if (player_locations[player::operations_expert] == "") {
      throw std::invalid_argument("Company plane is not usable without an Operations Expert.");
    } else if (!get_state(player_locations[player::operations_expert]).research_facility) {
      throw std::invalid_argument("Company plane requires the Operations Expert to be in a city with a research facility.");
    }
    if (!card::contains(get_player(player::operations_expert).hand, to_discard)) {
      throw std::invalid_argument("This player does not have this card.");
    }
    player_deck.discard(remove_player_card(player::operations_expert, to_discard));
    move(player::operations_expert, destination);
  }

  bool Game::draw_player_card(player::Role role) {
    if (player_deck.size() == 0) {
      return true;
    }
    get_player(role).hand.contents.push_back(player_deck.draw());
    return false;
  }
}
