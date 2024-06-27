#include <vector>
#include <algorithm>
#include <iostream>
#include <stdexcept>
#include "game.hpp"
#include "data/city_data.hpp"

namespace gerryfudd::core {
  int Game::infection_rate_escalation[] = INFECTION_RATE_ESCALATION;
  int Game::hand_sizes[] = HAND_SIZES;

  GameState::GameState(): infection_deck{card::infect}, player_deck{card::player}, contingency_card{card::player}, outbreaks{0}, infection_rate_level{0}, research_facility_reserve{RESEARCH_FACILITY_COUNT} {}
  int GameState::get_infection_rate() {
    return Game::infection_rate_escalation[infection_rate_level];
  }
  player::Player GameState::get_player(player::Role role) {
    for (std::vector<player::Player>::iterator cursor = players.begin(); cursor != players.end(); cursor++) {
      if (cursor->role == role) {
        return *cursor;
      }
    }
    throw std::invalid_argument("There is no player with this role.");
  }
  void GameState::add_card(player::Role role, card::Card card) {
    for (std::vector<player::Player>::iterator cursor = players.begin(); cursor != players.end(); cursor++) {
      if (cursor->role == role) {
        cursor->hand.contents.push_back(card);
        return;
      }
    }
    throw std::invalid_argument("There is no player with this role.");
  }
  card::Card GameState::remove_card(player::Role role, std::string card_name) {
    for (std::vector<player::Player>::iterator player_cursor = players.begin(); player_cursor != players.end(); player_cursor++) {
      if (player_cursor->role == role) {
        for (std::vector<card::Card>::iterator card_cursor = player_cursor->hand.contents.begin(); card_cursor != player_cursor->hand.contents.end(); card_cursor++) {
          if (card_cursor->name == card_name) {
            card::Card result = *card_cursor;
            player_cursor->hand.contents.erase(card_cursor);
            return result;
          }
        }
        throw std::invalid_argument("This player doesn't have this card.");
      }
    }
    throw std::invalid_argument("There is no player with this role.");
  }
  bool GameState::prevent_placement(std::string location, disease::DiseaseColor color) {
    if (player_locations[player::quarantine_specialist] == location) {
      return true;
    }
    if (diseases[color].cured && player_locations[player::medic] == location) {
      return true;
    }

    for (std::vector<city::City>::iterator cursor = cities[location].neighbors.begin(); cursor != cities[location].neighbors.end(); cursor++) {
      if (cursor->name == player_locations[player::quarantine_specialist]) {
        return true;
      }
    }
    return false;
  }

  GameState initialize_state(Difficulty difficulty, int player_count) {
    GameState result{};

    data::city::load_cities(&result.cities);
    for (std::map<std::string, city::City>::iterator cursor = result.cities.begin(); cursor != result.cities.end(); ++cursor) {
      result.board[cursor->first] = city::CityState();
      result.infection_deck.discard(card::Card(cursor->first, card::infect));
      result.player_deck.discard(card::Card(cursor->first, card::player));
    }
    result.research_facility_reserve--;
    result.board[CDC_LOCATION].research_facility = true;

    result.infection_deck.shuffle();

    std::string last_city_drawn;
    for (int i = 3; i < 12; i++) {
      last_city_drawn = result.infection_deck.draw_and_discard().name;
      result.board[last_city_drawn].disease_count[result.cities[last_city_drawn].color] += i / 3;
      result.diseases[result.cities[last_city_drawn].color].reserve -= i / 3;
    }
    
    result.player_deck.discard(card::Card(ONE_QUIET_NIGHT, card::player));
    result.player_deck.discard(card::Card(RESILIENT_POPULATION, card::player));
    result.player_deck.discard(card::Card(GOVERNMENT_GRANT, card::player));
    result.player_deck.discard(card::Card(AIRLIFT, card::player));

    result.player_deck.shuffle();
    int initial_hand_size = Game::hand_sizes[player_count - MIN_PLAYER_COUNT];
    std::vector<player::Role> roles = player::get_roles(player_count);
    for (std::vector<player::Role>::iterator cursor = roles.begin(); cursor != roles.end(); cursor++) {
      result.players.push_back(player::Player(*cursor));
      while (result.players.back().hand.contents.size() < initial_hand_size) {
        result.players.back().hand.contents.push_back(result.player_deck.draw());
      }
      result.player_locations[*cursor] = CDC_LOCATION;
    }
    int epidemics = BASE_EPIDEMIC_COUNT + difficulty;
    int cards_per_epidemic = (result.player_deck.remaining() + epidemics) / epidemics;
    for (int i = 0; i < epidemics; i++) {
      result.player_deck.insert(card::Card(EPIDEMIC, card::player, card::epidemic), cards_per_epidemic * i);
      result.player_deck.shuffle(cards_per_epidemic * i, std::min(cards_per_epidemic * (i+1), result.player_deck.remaining()));
    }
    return result;
  }
  GameState initialize_state() {
    return initialize_state(easy, 2);
  }

  TurnState::TurnState(player::Role active_role, int infection_rate): active_role{active_role}, event_cards_played{false}, remaining_actions{4}, remaining_player_card_draws{2}, remaining_infection_card_draws{infection_rate} {}

  Game::Game() {}
  Game::Game(GameState game_state): state{game_state} {}

  int Game::place(std::string target, disease::DiseaseColor color, int quantity) {
    if (state.prevent_placement(target, color)) {
      return 0;
    }
    state.diseases[color].reserve -= quantity;
    state.board[target].disease_count[color] += quantity;
    return quantity;
  }

  int Game::place(std::string target, int quantity) {
    return place(target, state.cities[target].color, quantity);
  }

  GameState Game::get_state() {
    return state;
  }
  void Game::discard(card::Card card) {
    switch (card.deck_type)
    {
    case card::infect:
      state.infection_deck.discard(card);
      break;
    case card::player:
      state.player_deck.discard(card);
      break;
    default:
      throw std::invalid_argument("");
    }
  } 
  void Game::remove_from_discard(card::Card card) {
    switch (card.deck_type)
    {
    case card::player:
      state.player_deck.remove_from_discard(card.name);
      break;
    case card::infect:
      state.infection_deck.remove_from_discard(card.name);
      break;
    default:
      throw std::invalid_argument("");
    }
  }

  void Game::place_research_facility(std::string city_name) {
    state.board[city_name].research_facility = true;
    state.research_facility_reserve--;
  }

  void Game::place_research_facility(std::string city_name, std::string source_city_name) {
    state.board[city_name].research_facility = true;
    state.board[source_city_name].research_facility = false;
  }

  bool Game::place_disease(std::string city_name, disease::DiseaseColor color, std::vector<std::string>& executed_outbreaks) {
    if (state.prevent_placement(city_name, color)) {
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
    return state.remove_card(role, card_name);
  }

  void Game::drive(player::Role role, std::string destination) {
    std::string origin = state.player_locations[role];
    for (std::vector<city::City>::iterator cursor = state.cities[origin].neighbors.begin(); cursor != state.cities[origin].neighbors.end(); cursor++) {
      if (cursor->name == destination) {
        state.player_locations[role] = destination;
        return;
      }
    }
    throw std::invalid_argument("This player can't drive to " + destination);
  }

  void Game::direct_flight(player::Role role, std::string destination) {
    state.player_deck.discard(remove_player_card(role, destination));
    state.player_locations[role] = destination;
  }

  void Game::charter_flight(player::Role role, std::string destination) {
    state.player_deck.discard(remove_player_card(role, state.player_locations[role]));
    state.player_locations[role] = destination;
  }
  void Game::shuttle(player::Role role, std::string destination) {
    if (!state.board[state.player_locations[role]].research_facility || !state.board[destination].research_facility) {
      throw std::invalid_argument("You may only shuttle between research facilities.");
    }
    state.player_locations[role] = destination;
  }

  void Game::dispatcher_direct_flight(player::Role role, std::string destination) {
    state.player_deck.discard(remove_player_card(player::dispatcher, destination));
    state.player_locations[role] = destination;
  }
  void Game::dispatcher_charter_flight(player::Role role, std::string destination) {
    state.player_deck.discard(remove_player_card(player::dispatcher, state.player_locations[role]));
    state.player_locations[role] = destination;
  }
  void Game::dispatcher_conference(player::Role guest, player::Role host) {
    state.player_locations[guest] = state.player_locations[host];
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
    state.add_card(target, remove_player_card(source, state.player_locations[source]));
  }
  void Game::researcher_share(std::string card_name, player::Role target) {
    if (state.player_locations[player::researcher] != state.player_locations[target]) {
      throw std::invalid_argument("Players must be in the same city to share cards.");
    }
    state.add_card(target, remove_player_card(player::researcher, card_name));
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
      if (!card::contains(state.get_player(role).hand, matching_cards[i])) {
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
      if (!card::contains(state.get_player(player::scientist).hand, matching_cards[i])) {
        throw std::invalid_argument("This player doesn't have this card.");
      }
    }
    for (i = 0; i < 4; i++) {
      state.player_deck.discard(remove_player_card(player::scientist, matching_cards[i]));
    }
    state.diseases[disease_to_cure].cured = true;
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
    } else if (!state.board[state.player_locations[player::operations_expert]].research_facility) {
      throw std::invalid_argument("Company plane requires the Operations Expert to be in a city with a research facility.");
    }
    if (!card::contains(state.get_player(player::operations_expert).hand, to_discard)) {
      throw std::invalid_argument("This player does not have this card.");
    }
    state.player_deck.discard(remove_player_card(player::operations_expert, to_discard));
    state.player_locations[player::operations_expert] = destination;
  }

  bool Game::epidemic() {
    card::Card infection_card = state.infection_deck.draw_and_discard(-1);
    state.infection_deck.shuffle();
    place(infection_card.name, 3);
    state.infection_rate_level++;
    return false;
  }

  bool Game::draw_player_card(player::Role role) {
    if (state.player_deck.size() == 0) {
      return true;
    }
    state.get_player(role).hand.contents.push_back(state.player_deck.draw());
    return false;
  }

  PlayerChoice create_one_quiet_night(player::Role role) {
    PlayerChoice choice;
    choice.prompt = "Play ";
    choice.prompt += ONE_QUIET_NIGHT;
    choice.prompt += " to skip the next infect step.";
    choice.effect = [role](Game &game, TurnState &turn_state) -> bool {
      game.discard(game.remove_player_card(role, ONE_QUIET_NIGHT));
      turn_state.remaining_infection_card_draws = 0;
      return false;
    };
    return choice;
  }

  PlayerChoice create_resilient_population(player::Role role, card::Card card_to_remove) {
    PlayerChoice choice;
    choice.prompt = "Play ";
    choice.prompt += RESILIENT_POPULATION;
    choice.prompt += " to remove ";
    choice.prompt += card_to_remove.name;
    choice.prompt += " from the infection discard.";
    choice.effect = [role, card_to_remove](Game &game, TurnState &) -> bool {
      game.discard(game.remove_player_card(role, RESILIENT_POPULATION));
      game.remove_from_discard(card_to_remove);
      return false;
    };
    return choice;
  }

  PlayerChoice create_government_grant(player::Role role, std::string target_city) {
    PlayerChoice choice;
    choice.prompt = "Play ";
    choice.prompt += GOVERNMENT_GRANT;
    choice.prompt += " to place a research facility in ";
    choice.prompt += target_city;
    choice.prompt += ".";
    choice.effect = [role, target_city](Game &game, TurnState &) -> bool {
      game.discard(game.remove_player_card(role, GOVERNMENT_GRANT));
      game.place_research_facility(target_city);
      return false;
    };
    return choice;
  }

  void add_choices_for_card_type(std::vector<PlayerChoice> *player_choices, player::Role role, card::CardType type, GameState game_state) {
    switch (type)
    {
    case card::one_quiet_night:
      (*player_choices).push_back(create_one_quiet_night(role));
      break;
    case card::resilient_population:
      for (int i = 0; i < game_state.infection_deck.get_discard_contents().size(); i++) {
        (*player_choices).push_back(create_resilient_population(role, game_state.infection_deck.get_discard_contents()[i]));
      }
      break;
    case card::government_grant:
      for (std::map<std::string, city::CityState>::iterator cursor = game_state.board.begin(); cursor != game_state.board.end(); cursor++) {
        if (!cursor->second.research_facility) {
          (*player_choices).push_back(create_government_grant(role, cursor->first));
        }
      }
      break;
    case card::airlift:
      break;
    default:
      break;
    }
  }

  std::vector<PlayerChoice> get_player_choices(player::Role role, GameState game_state, TurnState turn_state) {
    if (turn_state.event_cards_played) {
      return std::vector<PlayerChoice>{};
    }

    std::vector<PlayerChoice> result;
    player::Player player = game_state.get_player(role);
    for (std::vector<card::Card>::iterator cursor = player.hand.contents.begin(); cursor != player.hand.contents.end(); cursor++) {
      add_choices_for_card_type(&result, role, cursor->type, game_state);
    }
    return result;
  }
}
