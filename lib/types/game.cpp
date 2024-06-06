#include <types/game.hpp>
#include <fstream>
#include <vector>
#include <random>

/*

MEDIC

If a disease has been cured, he automatically removes all cubes of that color from a city, simply by entering it or being there. This does not take an action.

The Medic also prevents placing disease cubes (and outbreaks) of cured diseases in his location.

Quarantine Specialist

The Quarantine Specialist prevents both outbreaks and the placement of disease cubes in the city she is in and all cities connected to that city. She does not affect cubes placed during setup.
* Prevent disease cube placements (and outbreaks) in the city you are in and all cities connected to it.
*/

namespace gerryfudd::types {
  namespace player {
    std::string name_of(Role role) {
      switch (role)
      {
      case contingency_planner:
        return "Contingency Planner";
      case dispatcher:
        return "Dispatcher";
      case medic:
        return "Medic";
      case operations_expert:
        return "Operations Expert";
      case quarantine_specialist:
        return "Quarantine Specialist";
      case scientist:
        return "Scientist";
      case researcher:
        return "Researcher";
      default:
        throw std::invalid_argument("This role doesn't have a name.");
      }
    }
    std::string description_of(Role role) {
      switch (role)
      {
      case contingency_planner:
        return "As an action you may move a discarded event card to your role card. You may play event cards from your role card as though they were in your hand, but then they are removed from the game. You may only have one event card on your role card at a time.";
      case dispatcher:
        return "You may move other players' pawns as though they were your own. You may also move another players' pawn to a city with another pawn in it.\nYou must get permission from a pawn's owner before moving it.";
      case medic:
        return "When treating a disease, remove all cubes of a single color from the city you are in. If a disease is cured, remove all cubes of that color from any city you are in without taking any action.";
      case operations_expert:
        return "Placing a research facility doesn't require discarding a city card. You may move to any city from a research facility by discarding any city card.";
      case quarantine_specialist:
        return "No disease cubes are placed in the city where your pawn is or in any city connected to that city.";
      case scientist:
        return "You only need to discard four cards of a single color, rather than five, to cure a disease.";
      case researcher:
        return "You may pass any city card to another player as long as your pawn is in the same city as their pawn.";
      default:
        throw std::invalid_argument("This role doesn't have a description.");
      }
    }
    std::string name_of(ActionType action) {
      switch (action)
      {
      case drive:
        return "Drive / Ferry";
      case direct_flight:
        return "Direct Flight";
      case charter_flight:
        return "Charter Flight";
      case shuttle:
        return "Shuttle Flight";
      case build:
        return "Build a Research Station";
      case treat:
        return "Treat Disease";
      case share:
        return "Share Knowledge";
      case cure:
        return "Discover a Cure";
      case reclaim:
        return "Reclaim a discarded card";
      case conference:
        return "Arrange a conference";
      case company_plane:
        return "Use the company plane";
      default:
        throw std::invalid_argument("This action doesn't have a name.");
      }
    }
    std::string description_of(ActionType action, Role role) {
      switch (action)
      {
      case drive:
        if (role == dispatcher) {
          return "Move any pawn, with the consent of its owner, to a city connected by a white line to the one it is in.\n\nSome cities are connected to cities on the other side of the world. For example, Sydney and Los Angeles are connected.";
        }
        return "Move to a city connected by a white line to the one you are in.\n\nSome cities are connected to cities on the other side of the world. For example, Sydney and Los Angeles are connected.";
      case direct_flight:
        if (role == dispatcher) {
          return "Discard a City card to move any pawn, with the consent of its owner, to the city named on the card.";
        }
        return "Discard a City card to move to the city named on the card.";
      case charter_flight:
        if (role == dispatcher) {
          return "Discard the City card that matches the city any pawn is in to move it, with the consent of its owner, to any city.";
        }
        return "Discard the City card that matches the city you are in to move to any city.";
      case shuttle:
        if (role == dispatcher) {
          return "Move any pawn, with the consent of its owner, from a city with a research station to any other city that has a research station.";
        }
        return "Move from a city with a research station to any other city that has a research station.";
      case build:
        if (role == operations_expert) {
          return "Place a research station in the city you are in. Take the research station from the pile next to the board. If all 6 research stations have been built, take a research station from anywhere on the board.";
        }
        return "Discard the City card that matches the city you are in to place a research station there. Take the research station from the pile next to the board. If all 6 research stations have been built, take a research station from anywhere on the board.";
      case treat:
        if (role == medic) {
          return "Remove all cubes of one color from the city you are in. If the last cube of a cured disease is removed from the board, this disease is eradicated. Flip its cure marker from its blank side to its crossed out side.";
        }
        return "Remove 1 disease cube from the city you are in, placing it in the cube supply next to the board. If this disease color has been cured (see Discover a Cure below), remove all cubes of that color from the city you are in. If the last cube of a cured disease is removed from the board, this disease is eradicated.";
      case share:
        if (role == researcher) {
          return "You can do this action in two ways:\n\t-give any City card to another player, or\n\t-take the City card that matches the city you are in from another player.\n\nThe other player must also be in the city with you. Both of you need to agree to do this.";
        }
        return "You can do this action in two ways:\n\t-give the City card that matches the city you are in to another player, or\n\t-take a City card from another player. This card must match the city you are in unless the other player is the Researcher.\n\nThe other player must also be in the city with you. Both of you need to agree to do this.";
      case cure:
        if (role == scientist) {
          return "At any research station, discard 4 City cards of the same color from your hand to cure the disease of that color. Move the disease's cure marker to its Cure Indicator.\nIf no cubes of this color are on the board, this disease is now eradicated.\n\nWhen a disease is cured, its cubes remain on the board and new cubes can still be placed during epidemics or infections. However, treating this disease is now easier and you are closer to winning.";
        }
        return "At any research station, discard 5 City cards of the same color from your hand to cure the disease of that color. Move the disease's cure marker to its Cure Indicator.\nIf no cubes of this color are on the board, this disease is now eradicated.\n\nWhen a disease is cured, its cubes remain on the board and new cubes can still be placed during epidemics or infections. However, treating this disease is now easier and you are closer to winning.";
      case reclaim:
        if (role == contingency_planner) {
          return "Move a discarded event card from the discard pile to your role card. You may use this event card at any time as though it were in your hand. When you use this discarded card, remove it from the game.";
        }
      case conference:
        if (role == dispatcher) {
          return "Move any pawn to the city where another pawn already is.";
        }
      case company_plane:
        if (role == operations_expert) {
          return "If you are in a city with a research facility, discard any city card to move to any city.";
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
  }

  int Game::infection_rate_escalation[] = INFECTION_RATE_ESCALATION;
  int Game::hand_sizes[] = HAND_SIZES;
  Game::Game(): infection_deck{card::infect}, player_deck{card::player}, outbreaks{0}, infection_rate{0} {
    diseases[disease::DiseaseColor::black] = disease::DiseaseStatus();
    diseases[disease::DiseaseColor::blue] = disease::DiseaseStatus();
    diseases[disease::DiseaseColor::red] = disease::DiseaseStatus();
    diseases[disease::DiseaseColor::yellow] = disease::DiseaseStatus();
  }

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

  void read_cities_file(std::filesystem::path source_directory, disease::DiseaseColor color, std::map<std::string, city::City> *target) {
    std::ifstream fin;
    fin.open(source_directory / (disease::name_of(color) + "_cities.tsv"), std::ifstream::in);

    char c;
    std::string name, population;
    int line_index = 0;
    c = fin.get();
    while (fin.good()) {
      if (c == '\n') {
        if (line_index > 0) {
          (*target)[name] = city::City{name, color, std::stoi(population)};
        }
        line_index = 0;
        name = "";
        population = "";
        line_index = 0;
      } else if (c == '\t') {
        line_index++;
      } else if (line_index == 0) {
        if (name.size() > 0 || !std::isspace(c)) {
          name += c;
        }
      } else if (line_index == 1) {
        population += c;
      }
      c = fin.get();
    }

    if (line_index > 0) {
      (*target)[name] = city::City{name, color, std::stoi(population)};
    }
    fin.close();
  }

  std::filesystem::path resolve_public_path() {
    std::filesystem::path this_file = __FILE__;
    return this_file.parent_path().parent_path().parent_path() / "public";
  }

  void Game::setup() {
    setup(easy, MIN_PLAYER_COUNT);
  }

  void Game::setup(Difficulty difficulty, int player_count) {
    std::filesystem::path public_path = resolve_public_path();
    read_cities_file(public_path, disease::black, &cities);
    read_cities_file(public_path, disease::blue, &cities);
    read_cities_file(public_path, disease::red, &cities);
    read_cities_file(public_path, disease::yellow, &cities);
    for (std::map<std::string, city::City>::iterator cursor = cities.begin(); cursor != cities.end(); ++cursor) {
      board[cursor->first] = city::CityState();
      infection_deck.discard(card::Card(cursor->first, card::infect));
      player_deck.discard(card::Card(cursor->first, card::player));
    }
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
}
