#include <iostream>
#include <types/disease.hpp>

int main() {
  types::disease::DiseaseStatus diseaseStatus;
  std::cout << "Initially the disease has " << diseaseStatus << std::endl;
  std::cout << "After placing some cubes " << diseaseStatus.place(3) << " are in reserve." << std::endl;
  std::cout << "Now the disease has " << diseaseStatus << std::endl;
  return 0;
}
