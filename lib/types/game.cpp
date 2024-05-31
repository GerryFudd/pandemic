#include <types/game.hpp>

namespace gerryfudd::types {
  Game::Game() {
    std::map<disease::DiseaseColor, disease::DiseaseStatus> diseases;
    diseases[disease::DiseaseColor::black] = disease::DiseaseStatus();
    diseases[disease::DiseaseColor::blue] = disease::DiseaseStatus();
    diseases[disease::DiseaseColor::red] = disease::DiseaseStatus();
    diseases[disease::DiseaseColor::yellow] = disease::DiseaseStatus();
  }

  disease::DiseaseStatus Game::lookup_disease(disease::DiseaseColor disease_color) {
    return diseases[disease_color];
  }
}
