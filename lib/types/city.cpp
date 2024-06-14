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

  CityState::CityState(): research_facility{false} {}
  bool CityState::prevent_placement(disease::DiseaseColor color) {
    for (std::vector<disease::DiseaseColor>::iterator cursor = protected_colors.begin(); cursor != protected_colors.end(); cursor++) {
      if (color == *cursor) {
        return true;
      }
    }
    return false;
  }
}