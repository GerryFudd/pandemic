#include <types/city.hpp>
#include <stdexcept>


namespace gerryfudd::types::city {
  City::City(std::string name, disease::DiseaseColor color, int population):
    name{name}, color{color}, population{population} {}

  City::City(): City::City("", disease::none, -1) {}

  const char *City::get_name() {
    return name.c_str();
  }
  disease::DiseaseColor City::get_color() {
    return color;
  }
  int City::get_population() {
    return population;
  }
  City City::get_neighbor(int n) {
    return neighbors[n];
  }
  int City::get_neighbor_count() {
    return neighbors.size();
  }
  void attach(City *a, City *b) {
    (*a).neighbors.push_back(*b);
    (*b).neighbors.push_back(*a);
  }

  CityState::CityState(): prevent_placement{false}, research_facility{false} {}

  int CityState::get_count(disease::DiseaseColor color) {
    return disease_count[color];
  }

  int CityState::add(disease::DiseaseColor color, int quantity) {
    if (prevent_placement) {
      return 0;
    }
    disease_count[color] += quantity;
    return quantity;
  }
}