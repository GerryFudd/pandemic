#include <types/game.hpp>
#include <fstream>
#include <vector>

namespace gerryfudd::types {
  Game::Game(): diseases{}, cities{}, board{} {
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
      board[&(cursor->second)] = city::CityState();
    }
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

  city::CityState Game::get_state(city::City *city) {
    return board[city];
  }
}
