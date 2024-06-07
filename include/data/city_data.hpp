#ifndef CITY_DATA
#define CITY_DATA

#define CITIES_MAP cities_map

#define MAP_CITY(name, population) (*CITIES_MAP)[#name] = types::city::City(#name, types::disease::COLOR, population);
#define ATTACH(name1, name2) attach(&(*CITIES_MAP)[#name1], &(*CITIES_MAP)[#name2]);

#include <string>
#include <map>
#include "types/city.hpp"

namespace gerryfudd::data::city {
  void load_cities(std::map<std::string, types::city::City> *);
}

#endif