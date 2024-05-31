#include <types/disease.hpp>

namespace gerryfudd::types::disease {
  DiseaseStatus::DiseaseStatus(): reserve{DISEASE_RESERVE}, cured{false} {}

  int DiseaseStatus::get_reserve() {
    return reserve;
  }

  bool DiseaseStatus::is_cured() {
    return cured;
  }

  int DiseaseStatus::place(int quantity) {
    if (cured && reserve == DISEASE_RESERVE) {
      return 0;
    }
    reserve -= quantity;
    return quantity;
  }

  int DiseaseStatus::treat(int quantity) {
    if (cured) {
      reserve += quantity;
      return quantity;
    }
    reserve += 1;
    return 1;
  }

  void DiseaseStatus::cure() {
    cured = true;
  }

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
