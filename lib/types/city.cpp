#include <types/city.hpp>
#include <stdexcept>


namespace gerryfudd::types::city {
  City::City(const char* name, const disease::DiseaseColor color, int population):
    name{name}, color{&color}, population{population} {}

  const char *City::get_name() {
    return name.c_str();
  }
  const char *City::get_color() {
    return disease::name_of(*color);
  }
  const int City::get_population() {
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
}