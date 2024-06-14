#ifndef CITY_TYPES
#define CITY_TYPES
#include <vector>
#include <map>
#include "types/disease.hpp"

namespace gerryfudd::types::city {
  struct City {
    std::string name;
    disease::DiseaseColor color;
    int population;
    std::vector<City> neighbors;
    City();
    City(std::string, disease::DiseaseColor, int);
    friend void attach(City *, City *);
  };

  struct CityState {
    std::map<disease::DiseaseColor, int> disease_count;
    std::vector<disease::DiseaseColor> protected_colors;
    bool research_facility;
    CityState();
    bool prevent_placement(disease::DiseaseColor);
  };
}
#endif
