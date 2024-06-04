#ifndef CITY_TYPES
#define CITY_TYPES
#include <types/disease.hpp>
#include <vector>
#include <map>

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
    bool prevent_placement;
    bool research_facility;
    CityState();
  };
}
#endif
