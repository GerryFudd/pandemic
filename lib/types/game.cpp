#include <types/game.hpp>

namespace gerryfudd::types {
  Game::Game() {
    std::map<disease::DiseaseColor, disease::DiseaseStatus> diseases;
    diseases[disease::DiseaseColor::black] = disease::DiseaseStatus();
    diseases[disease::DiseaseColor::blue] = disease::DiseaseStatus();
    diseases[disease::DiseaseColor::red] = disease::DiseaseStatus();
    diseases[disease::DiseaseColor::yellow] = disease::DiseaseStatus();
  }

  int Game::get_reserve(disease::DiseaseColor disease_color) {
    return diseases[disease_color].get_reserve();
  }

  bool Game::is_cured(disease::DiseaseColor disease_color) {
    return diseases[disease_color].is_cured();
  }
}
