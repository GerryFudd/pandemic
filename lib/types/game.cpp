#include <types/game.hpp>
#include <fstream>
#include <vector>
#include <exception>
#include <random>

namespace gerryfudd::types {
  Card::Card(std::string name, CardType type): name{name}, type{type} {}

  Deck::Deck(CardType type): contents{}, discard{}, type{type} {}
  void Deck::add(Card card) {
    if (type != card.type) {
      throw std::invalid_argument("This deck only accepts one type of card.");
    }
    discard.push_back(card);
  }
  int random(int options) {
    std::random_device generator;
    std::uniform_int_distribution<int> distribution(0,options-1);
    return distribution(generator);
  }
  void Deck::shuffle() {
    int i;
    while (discard.size() > 0) {
      i = random(discard.size());
      contents.push_back(discard[i]);
      discard.erase(discard.begin() + i);
    }
  }
  Card Deck::draw() {
    discard.push_back(contents.back());
    contents.pop_back();
    return discard.back();
  }
  Card Deck::reveal(int position) {
    return contents[contents.size() - position];
  }
  int Deck::size() {
    return contents.size() + discard.size();
  }
  int Deck::remaining() {
    return contents.size();
  }

  Game::Game(): diseases{}, cities{}, board{}, infection_deck{infect} {
    diseases[disease::DiseaseColor::black] = disease::DiseaseStatus();
    diseases[disease::DiseaseColor::blue] = disease::DiseaseStatus();
    diseases[disease::DiseaseColor::red] = disease::DiseaseStatus();
    diseases[disease::DiseaseColor::yellow] = disease::DiseaseStatus();
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

  void Game::setup(std::filesystem::path source_directory) {
    read_cities_file(source_directory, disease::black, &cities);
    read_cities_file(source_directory, disease::blue, &cities);
    read_cities_file(source_directory, disease::red, &cities);
    read_cities_file(source_directory, disease::yellow, &cities);
    for (std::map<std::string, city::City>::iterator cursor = cities.begin(); cursor != cities.end(); ++cursor) {
      board[cursor->first] = city::CityState();
      infection_deck.add(Card(cursor->first, infect));
    }
    infection_deck.shuffle();

    std::string target_name = infection_deck.draw().name;
    diseases[cities[target_name].get_color()].place(board[target_name].add(cities[target_name].get_color(), 1));
    target_name = infection_deck.draw().name;
    diseases[cities[target_name].get_color()].place(board[target_name].add(cities[target_name].get_color(), 1));
    target_name = infection_deck.draw().name;
    diseases[cities[target_name].get_color()].place(board[target_name].add(cities[target_name].get_color(), 1));

    target_name = infection_deck.draw().name;
    diseases[cities[target_name].get_color()].place(board[target_name].add(cities[target_name].get_color(), 2));
    target_name = infection_deck.draw().name;
    diseases[cities[target_name].get_color()].place(board[target_name].add(cities[target_name].get_color(), 2));
    target_name = infection_deck.draw().name;
    diseases[cities[target_name].get_color()].place(board[target_name].add(cities[target_name].get_color(), 2));

    target_name = infection_deck.draw().name;
    diseases[cities[target_name].get_color()].place(board[target_name].add(cities[target_name].get_color(), 3));
    target_name = infection_deck.draw().name;
    diseases[cities[target_name].get_color()].place(board[target_name].add(cities[target_name].get_color(), 3));
    target_name = infection_deck.draw().name;
    diseases[cities[target_name].get_color()].place(board[target_name].add(cities[target_name].get_color(), 3));
  }

  int Game::get_reserve(disease::DiseaseColor disease_color) {
    return diseases[disease_color].get_reserve();
  }

  bool Game::is_cured(disease::DiseaseColor disease_color) {
    return diseases[disease_color].is_cured();
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
