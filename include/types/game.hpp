#ifndef GAME_TYPE
#define GAME_TYPE
#include <types/disease.hpp>
#include <map>

namespace gerryfudd::types {
  class Game {
    std::map<disease::DiseaseColor, disease::DiseaseStatus> diseases;
  public:
    Game();
    int get_reserve(disease::DiseaseColor);
    bool is_cured(disease::DiseaseColor);
  };
}

#endif