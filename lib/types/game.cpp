#include <types/game.hpp>
#include <fstream>
#include <vector>
#include <random>

namespace gerryfudd::types {
  int Game::infection_rate_escalation[] = INFECTION_RATE_ESCALATION;
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
    setup(easy);
  }

  void Game::setup(Difficulty difficulty) {
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
