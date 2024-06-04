#include <Framework.hpp>
#include <Assertions.inl>
#include <types/disease.hpp>

using namespace gerryfudd::test;
using namespace gerryfudd::types;

TEST(default_constructor) {
  disease::DiseaseStatus diseaseStatus;
  assert_equal(diseaseStatus.reserve, DISEASE_RESERVE);
  assert_false(diseaseStatus.cured, "Diseases should not be cured on initialization.");
}

TEST(stream_operator_on_init) {
  std::stringstream stream_captor;
  stream_captor << disease::DiseaseStatus();
  assert_equal(stream_captor.str(), std::string("0 remaining"));
}

TEST(stream_operator_after_placement) {
  disease::DiseaseStatus diseaseStatus;
  std::stringstream stream_captor;


  diseaseStatus.reserve -= 3;
  stream_captor << diseaseStatus;
  assert_equal(stream_captor.str(), std::string("3 remaining"));
  stream_captor.str("");

  diseaseStatus.reserve -= 2;
  stream_captor << diseaseStatus;
  assert_equal(stream_captor.str(), std::string("5 remaining"));
}

TEST(stream_operator_after_cure) {
  disease::DiseaseStatus diseaseStatus;
  std::stringstream stream_captor;


  diseaseStatus.reserve -= 5;
  diseaseStatus.cured = true;
  stream_captor << diseaseStatus;
  assert_equal(stream_captor.str(), std::string("Cured, 5 remaining"));
}

TEST(stream_operator_after_eradicated) {
  disease::DiseaseStatus diseaseStatus;
  std::stringstream stream_captor;
  diseaseStatus.cured = true;
  stream_captor << diseaseStatus;
  assert_equal(stream_captor.str(), std::string("Eradicated"));
}
