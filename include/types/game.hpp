#ifndef GAME_TYPE
#define GAME_TYPE
#include <types/disease.hpp>
#include <map>

namespace gerryfudd::types {
  class Game {
    std::map<disease::DiseaseColor, disease::DiseaseStatus> diseases;
  public:
    Game();
    disease::DiseaseStatus lookup_disease(disease::DiseaseColor);
  };
}

#endif