#include <types/city.hpp>
#include <stdexcept>


namespace gerryfudd::types::city {
  City::City(std::string name, disease::DiseaseColor color, int population):
    name{name}, color{color}, population{population} {}
  
  City::City(): City::City("", disease::none, -1) {}

  void attach(City *a, City *b) {
    (*a).neighbors.push_back(*b);
    (*b).neighbors.push_back(*a);
  }

  CityState::CityState(): prevent_placement{false}, research_facility{false} {}
}