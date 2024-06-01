#ifndef CITY_TYPES
#define CITY_TYPES
#include <types/disease.hpp>
#include <vector>

namespace gerryfudd::types::city {
  class City {
    const std::string name;
    const disease::DiseaseColor *color;
    const int population;
    std::vector<City> neighbors;
  public:
    City(const char*, const disease::DiseaseColor, int);
    const char *get_name(void);
    const char *get_color(void);
    const int get_population(void);
    City get_neighbor(int);
    int get_neighbor_count(void);
    friend void attach(City*, City*);
  };
}
#endif
