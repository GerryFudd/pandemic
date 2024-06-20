#include <vector>
#include <algorithm>
#include <iostream>
#include <stdexcept>
#include "game.hpp"
#include "data/city_data.hpp"

namespace gerryfudd::core {
  int Game::infection_rate_escalation[] = INFECTION_RATE_ESCALATION;
  int Game::hand_sizes[] = HAND_SIZES;
  GameState::GameState(): infection_deck{card::infect}, player_deck{card::player}, contingency_card{card::player}, outbreaks{0}, infection_rate{Game::infection_rate_escalation[0]}, research_facility_reserve{RESEARCH_FACILITY_COUNT} {}
  Game::Game() {}

  int Game::place(std::string target, disease::DiseaseColor color, int quantity) {
    if (state.board[target].prevent_placement(color)) {
      return 0;
    }
    state.diseases[color].reserve -= quantity;
    state.board[target].disease_count[color] += quantity;
    return quantity;
  }

  int Game::place(std::string target, int quantity) {
    return place(target, state.cities[target].color, quantity);
  }

  void Game::setup() {
    setup(easy, MIN_PLAYER_COUNT);
  }

  void Game::setup(Difficulty difficulty, int player_count) {
    state = GameState();

    data::city::load_cities(&state.cities);
    for (std::map<std::string, city::City>::iterator cursor = state.cities.begin(); cursor != state.cities.end(); ++cursor) {
      state.board[cursor->first] = city::CityState();
      state.infection_deck.discard(card::Card(cursor->first, card::infect));
      state.player_deck.discard(card::Card(cursor->first, card::player));
    }
    place_research_facility(CDC_LOCATION);

    state.infection_deck.shuffle();

    place(state.infection_deck.draw_and_discard().name, 1);
    place(state.infection_deck.draw_and_discard().name, 1);
    place(state.infection_deck.draw_and_discard().name, 1);

    place(state.infection_deck.draw_and_discard().name, 2);
    place(state.infection_deck.draw_and_discard().name, 2);
    place(state.infection_deck.draw_and_discard().name, 2);

    place(state.infection_deck.draw_and_discard().name, 3);
    place(state.infection_deck.draw_and_discard().name, 3);
    place(state.infection_deck.draw_and_discard().name, 3);
    
    state.player_deck.discard(card::Card(ONE_QUIET_NIGHT, card::player));
    state.player_deck.discard(card::Card(RESILIENT_POPULATION, card::player));
    state.player_deck.discard(card::Card(GOVERNMENT_GRANT, card::player));
    state.player_deck.discard(card::Card(AIRLIFT, card::player));

    state.player_deck.shuffle();
    int initial_hand_size = hand_sizes[player_count - MIN_PLAYER_COUNT];
    std::vector<player::Role> roles = player::get_roles(player_count);
    for (std::vector<player::Role>::iterator cursor = roles.begin(); cursor != roles.end(); cursor++) {
      state.players.push_back(player::Player(*cursor));
      while (state.players.back().hand.contents.size() < initial_hand_size) {
        state.players.back().hand.contents.push_back(state.player_deck.draw());
      }
      place_player(*cursor, CDC_LOCATION);
    }
    int epidemics = BASE_EPIDEMIC_COUNT + difficulty;
    int cards_per_epidemic = (state.player_deck.remaining() + epidemics) / epidemics;
    for (int i = 0; i < epidemics; i++) {
      state.player_deck.insert(card::Card(EPIDEMIC, card::player, card::epidemic), cards_per_epidemic * i);
      state.player_deck.shuffle(cards_per_epidemic * i, std::min(cards_per_epidemic * (i+1), state.player_deck.remaining()));
    }
  }

  void Game::update_protections(std::string location) {
    state.board[location].protected_colors.resize(0);
    if (state.player_locations[player::quarantine_specialist] == location) {
      state.board[location].protected_colors.push_back(disease::black);
      state.board[location].protected_colors.push_back(disease::blue);
      state.board[location].protected_colors.push_back(disease::red);
      state.board[location].protected_colors.push_back(disease::yellow);

      for (std::vector<city::City>::iterator cursor = state.cities[location].neighbors.begin(); cursor != state.cities[location].neighbors.end(); cursor++) {
        state.board[cursor->name].protected_colors.resize(0);
        state.board[cursor->name].protected_colors.push_back(disease::black);
        state.board[cursor->name].protected_colors.push_back(disease::blue);
        state.board[cursor->name].protected_colors.push_back(disease::red);
        state.board[cursor->name].protected_colors.push_back(disease::yellow);
      }
    } else if (state.player_locations[player::medic] == location) {
      if (state.diseases[disease::black].cured) {
        state.board[location].protected_colors.push_back(disease::black);
      }
      if (state.diseases[disease::blue].cured) {
        state.board[location].protected_colors.push_back(disease::blue);
      }
      if (state.diseases[disease::red].cured) {
        state.board[location].protected_colors.push_back(disease::red);
      }
      if (state.diseases[disease::yellow].cured) {
        state.board[location].protected_colors.push_back(disease::yellow);
      }
    }
  }

  void Game::remove_player_with_role(player::Role role) {
    std::string player_location = state.player_locations[role];

    state.player_locations.erase(state.player_locations.find(role));

    update_protections(player_location);
    if (role == player::quarantine_specialist) {
      for (std::vector<city::City>::iterator cursor = state.cities[player_location].neighbors.begin(); cursor != state.cities[player_location].neighbors.end(); cursor++) {
        update_protections(cursor->name);
      }
    }
  }
  void Game::place_player(player::Role role, std::string destination) {
    state.player_locations[role] = destination;
    update_protections(destination);
  }
  void Game::move(player::Role role, std::string destination) {
    remove_player_with_role(role);
    place_player(role, destination);
  }
  void Game::add_role(player::Role role) {
    for (std::vector<player::Player>::iterator cursor = state.players.begin(); cursor != state.players.end(); cursor++) {
      if (cursor->role == role) {
        return;
      }
    }
    player::Role role_to_replace = state.players.back().role;
    card::Hand hand = state.players.back().hand;
    remove_player_with_role(role_to_replace);
    state.players.pop_back();
    state.players.push_back(player::Player(role));
    state.players.back().hand = hand;
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
    std::vector<player::Player>::iterator to_replace = state.players.end();
    for (std::vector<player::Player>::iterator cursor = state.players.begin(); cursor != state.players.end(); cursor++) {
      unused_roles.erase(std::find(unused_roles.begin(), unused_roles.end(), cursor->role));
      if (cursor->role == role) {
        to_replace = cursor;
      }
    }
    if (to_replace == state.players.end()) {
      // There is nothing to do.
      return;
    }
    player::Role role_to_insert = unused_roles.back();
    card::Hand hand = to_replace->hand;
    remove_player_with_role(role);
    state.players.erase(to_replace);
    state.players.push_back(player::Player(role_to_insert));
    state.players.back().hand = hand;
    place_player(role_to_insert, CDC_LOCATION);
  }
  void Game::add_card(player::Role role, card::Card card) {
    for (std::vector<player::Player>::iterator player_cursor = state.players.begin(); player_cursor != state.players.end(); player_cursor++) {
      if (player_cursor->role == role) {
        if (card::contains(player_cursor->hand, card.name)) {
          return;
        }
        player_cursor->hand.contents.push_back(card);
        return;
      }
    }
  }
  GameState Game::get_state() {
    return state;
  }
  void Game::discard(card::Card card) {
    switch (card.deck_type)
    {
    case card::player:
      state.player_deck.discard(card);
      break;
    case card::infect:
      state.infection_deck.discard(card);
      break;
    default:
      throw std::invalid_argument("");
    }
  }

  int Game::get_city_count() {
    return state.cities.size();
  }

  city::CityState Game::get_state(std::string city_name) {
    return state.board[city_name];
  }

  std::vector<player::Player> Game::get_players_in(std::string city_name) {
    std::vector<player::Player> result;
    for (std::vector<player::Player>::iterator cursor = state.players.begin(); cursor != state.players.end(); cursor++) {
      if (state.player_locations[cursor->role] == city_name) {
        result.push_back(*cursor);
      }
    }
    return result;
  }
  std::string Game::get_location(player::Role role) {
    return state.player_locations[role];
  }

  int Game::get_research_facility_reserve() {
    return state.research_facility_reserve;
  }

  void Game::place_research_facility(std::string city_name) {
    state.board[city_name].research_facility = true;
    state.research_facility_reserve--;
  }

  void Game::place_research_facility(std::string city_name, std::string source_city_name) {
    state.board[city_name].research_facility = true;
    state.board[source_city_name].research_facility = false;
  }

  int Game::get_player_count() {
    return state.players.size();
  }
  player::Player Game::get_player(player::Role role) {
    for (std::vector<player::Player>::iterator cursor = state.players.begin(); cursor != state.players.end(); cursor++) {
      if (cursor->role == role) {
        return *cursor;
      }
    }
    throw std::invalid_argument("There is no player with this role");
  }
  player::Player Game::get_player(int n) {
    return state.players[n % state.players.size()];
  }
  city::City Game::get_city(player::Role role) {
    return state.cities[state.player_locations[role]];
  }

  int Game::get_infection_rate() {
    return infection_rate_escalation[state.infection_rate];
  }
  disease::DiseaseStatus Game::get_status(disease::DiseaseColor color) {
    return state.diseases[color];
  }
  bool Game::place_disease(std::string city_name, disease::DiseaseColor color, std::vector<std::string>& executed_outbreaks) {
    if (state.board[city_name].prevent_placement(color)) {
      return false;
    }
    if (state.diseases[color].reserve <= 0) {
      return true;
    }
    if (state.board[city_name].disease_count[color] >= 3) {
      for (std::vector<std::string>::iterator cursor = executed_outbreaks.begin(); cursor != executed_outbreaks.end(); cursor++) {
        if (city_name == *cursor) {
          return false;
        }
      }
      if (++state.outbreaks >= 10) {
        return true;
      }
      executed_outbreaks.push_back(city_name);
      for (std::vector<city::City>::iterator cursor = state.cities[city_name].neighbors.begin(); cursor != state.cities[city_name].neighbors.end(); cursor++) {
        if (place_disease(cursor->name, color, executed_outbreaks)) {
          return true;
        }
      }
      return false;
    }
    state.diseases[color].reserve--;
    state.board[city_name].disease_count[color]++;
    return false;
  }
  bool Game::draw_infection_card() {
    std::vector<std::string> executed_outbreaks;
    card::Card infection_card = state.infection_deck.draw();
    if (place_disease(infection_card.name, state.cities[infection_card.name].color, executed_outbreaks)) {
      return true;
    }
    state.infection_deck.discard(infection_card);
    return false;
  }
  card::Card Game::remove_player_card(player::Role role, std::string card_name) {
    for (std::vector<player::Player>::iterator player_cursor = state.players.begin(); player_cursor != state.players.end(); player_cursor++) {
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
    return state.infection_deck.get_discard_contents();
  }
  std::vector<card::Card> Game::get_player_discard() {
    return state.player_deck.get_discard_contents();
  }

  void Game::drive(player::Role role, std::string destination) {
    std::string origin = state.player_locations[role];
    for (std::vector<city::City>::iterator cursor = state.cities[origin].neighbors.begin(); cursor != state.cities[origin].neighbors.end(); cursor++) {
      if (cursor->name == destination) {
        move(role, destination);
        return;
      }
    }
    throw std::invalid_argument("This player can't drive to " + destination);
  }

  void Game::direct_flight(player::Role role, std::string destination) {
    state.player_deck.discard(remove_player_card(role, destination));
    move(role, destination);
  }

  void Game::charter_flight(player::Role role, std::string destination) {
    state.player_deck.discard(remove_player_card(role, state.player_locations[role]));
    move(role, destination);
  }
  void Game::shuttle(player::Role role, std::string destination) {
    if (!state.board[state.player_locations[role]].research_facility || !state.board[destination].research_facility) {
      throw std::invalid_argument("You may only shuttle between research facilities.");
    }
    move(role, destination);
  }

  void Game::dispatcher_direct_flight(player::Role role, std::string destination) {
    state.player_deck.discard(remove_player_card(player::dispatcher, destination));
    move(role, destination);
  }
  void Game::dispatcher_charter_flight(player::Role role, std::string destination) {
    state.player_deck.discard(remove_player_card(player::dispatcher, state.player_locations[role]));
    move(role, destination);
  }
  void Game::dispatcher_conference(player::Role guest, player::Role host) {
    move(guest, state.player_locations[host]);
  }

  void Game::treat(player::Role role, disease::DiseaseColor color) {
    if (state.diseases[color].cured || role == player::medic) {
      state.diseases[color].reserve += state.board[state.player_locations[role]].disease_count[color];
      state.board[state.player_locations[role]].disease_count[color] = 0;
    } else if (state.board[state.player_locations[role]].disease_count[color] > 0) {
      state.diseases[color].reserve++;
      state.board[state.player_locations[role]].disease_count[color]--;
    }
  }
  void Game::share(player::Role source, player::Role target) {
    if (state.player_locations[source] != state.player_locations[target]) {
      throw std::invalid_argument("Players must be in the same city to share cards.");
    }
    add_card(target, remove_player_card(source, state.player_locations[source]));
  }
  void Game::researcher_share(std::string card_name, player::Role target) {
    if (state.player_locations[player::researcher] != state.player_locations[target]) {
      throw std::invalid_argument("Players must be in the same city to share cards.");
    }
    add_card(target, remove_player_card(player::researcher, card_name));
  }
  void Game::cure(player::Role role, std::string matching_cards[5]) {
    if (!state.board[state.player_locations[role]].research_facility) {
      throw std::invalid_argument("Curing a disease may only occur in a city with a research facility.");
    }
    disease::DiseaseColor disease_to_cure;
    int i;
    for (i = 0; i < 5; i++) {
      if (i == 0) {
        disease_to_cure = state.cities[matching_cards[i]].color;
      } else if (disease_to_cure != state.cities[matching_cards[i]].color) {
        throw std::invalid_argument("Curing a disease requires that all discarded cards share a color.");
      }
      if (!card::contains(get_player(role).hand, matching_cards[i])) {
        throw std::invalid_argument("This player doesn't have this card.");
      }
    }
    for (i = 0; i < 5; i++) {
      state.player_deck.discard(remove_player_card(role, matching_cards[i]));
    }
    state.diseases[disease_to_cure].cured = true;
  }
  void Game::scientist_cure(std::string matching_cards[4]) {
    if (!state.board[state.player_locations[player::scientist]].research_facility) {
      throw std::invalid_argument("Curing a disease may only occur in a city with a research facility.");
    }
    disease::DiseaseColor disease_to_cure;
    int i;
    for (i = 0; i < 4; i++) {
      if (i == 0) {
        disease_to_cure = state.cities[matching_cards[i]].color;
      } else if (disease_to_cure != state.cities[matching_cards[i]].color) {
        throw std::invalid_argument("Curing a disease requires that all discarded cards share a color.");
      }
      if (!card::contains(get_player(player::scientist).hand, matching_cards[i])) {
        throw std::invalid_argument("This player doesn't have this card.");
      }
    }
    for (i = 0; i < 4; i++) {
      state.player_deck.discard(remove_player_card(player::scientist, matching_cards[i]));
    }
    state.diseases[disease_to_cure].cured = true;
  }
  card::Hand Game::get_contingency_card() {
    return state.contingency_card;
  }
  void Game::reclaim(std::string event_card) {
    if (state.player_locations[player::contingency_planner] == "") {
      throw std::invalid_argument("Reclaim is not usable without a Contingency Planner.");
    }
    card::Card discarded_card = state.player_deck.remove_from_discard(event_card);
    if (discarded_card.type == card::city) {
      state.player_deck.discard(discarded_card);
      throw std::invalid_argument("City cards may not be reclaimed.");
    }
    state.contingency_card.contents.resize(0, discarded_card);
    state.contingency_card.contents.push_back(discarded_card);
  }
  void Game::company_plane(std::string destination, std::string to_discard) {
    if (state.player_locations[player::operations_expert] == "") {
      throw std::invalid_argument("Company plane is not usable without an Operations Expert.");
    } else if (!get_state(state.player_locations[player::operations_expert]).research_facility) {
      throw std::invalid_argument("Company plane requires the Operations Expert to be in a city with a research facility.");
    }
    if (!card::contains(get_player(player::operations_expert).hand, to_discard)) {
      throw std::invalid_argument("This player does not have this card.");
    }
    state.player_deck.discard(remove_player_card(player::operations_expert, to_discard));
    move(player::operations_expert, destination);
  }

  bool Game::draw_player_card(player::Role role) {
    if (state.player_deck.size() == 0) {
      return true;
    }
    get_player(role).hand.contents.push_back(state.player_deck.draw());
    return false;
  }
}
