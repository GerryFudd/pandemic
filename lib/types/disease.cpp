#include <types/disease.hpp>
#include <exception>

namespace gerryfudd::types::disease {
  std::string name_of(DiseaseColor color) {
    switch (color)
    {
    case black:
      return "black";
    case blue:
      return "blue";
    case red:
      return "red";
    case yellow:
      return "yellow";
    default:
      throw std::invalid_argument("The supplied disease is not named.");
    }
  }
  DiseaseStatus::DiseaseStatus(): reserve{DISEASE_RESERVE}, cured{false} {}

  std::ostream& operator<<(std::ostream& out, const DiseaseStatus& diseaseStatus) {
    if (diseaseStatus.cured) {
      if (diseaseStatus.reserve == DISEASE_RESERVE) {
        out << "Eradicated";
      } else {
        out << "Cured, " << DISEASE_RESERVE - diseaseStatus.reserve << " remaining";
      }
    } else {
      out << DISEASE_RESERVE - diseaseStatus.reserve << " remaining";
    }
    return out;
  }
}
