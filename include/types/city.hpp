#ifndef CITY_TYPES
#define CITY_TYPES
#include <types/disease.hpp>
#include <vector>
#include <map>

namespace gerryfudd::types::city {
  class City {
    std::string name;
    disease::DiseaseColor color;
    int population;
    std::vector<City> neighbors;
  public:
    City();
    City(std::string, disease::DiseaseColor, int);
    const char *get_name(void);
    disease::DiseaseColor get_color(void);
    int get_population(void);
    City get_neighbor(int);
    int get_neighbor_count(void);
    friend void attach(City*, City*);
  };

  class CityState {
    std::map<disease::DiseaseColor, int> disease_count;
    bool prevent_placement;
    bool research_facility;
  public:
    CityState();
    int get_count(disease::DiseaseColor);
  };
}
#endif
