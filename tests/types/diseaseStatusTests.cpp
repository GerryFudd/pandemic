#include <Framework.hpp>
#include <Assertions.inl>
#include <types/disease.hpp>

using namespace gerryfudd::test;
using namespace gerryfudd::types;

TEST(default_constructor) {
  disease::DiseaseStatus diseaseStatus;
  assert_equal(diseaseStatus.get_reserve(), DISEASE_RESERVE);
  assert_false(diseaseStatus.is_cured(), "Diseases should not be cured on initialization.");
}

TEST(stream_operator_on_init) {
  std::stringstream stream_captor;
  stream_captor << disease::DiseaseStatus();
  assert_equal(stream_captor.str(), std::string("0 remaining"));
}

TEST(place_disease) {
  disease::DiseaseStatus diseaseStatus;
  assert_equal(diseaseStatus.place(1), 1);
  assert_equal(diseaseStatus.get_reserve(), DISEASE_RESERVE - 1);
  assert_equal(diseaseStatus.place(3), 3);
  assert_equal(diseaseStatus.get_reserve(), DISEASE_RESERVE - 4);
}

TEST(cure_disease) {
  disease::DiseaseStatus diseaseStatus;
  diseaseStatus.cure();
  assert_true(diseaseStatus.is_cured(), "Calling DiseaseStatus::cure() should set cured to true.");
}

TEST(stream_operator_after_placement) {
  disease::DiseaseStatus diseaseStatus;
  std::stringstream stream_captor;


  diseaseStatus.place(3);
  stream_captor << diseaseStatus;
  assert_equal(stream_captor.str(), std::string("3 remaining"));
  stream_captor.str("");

  diseaseStatus.place(2);
  stream_captor << diseaseStatus;
  assert_equal(stream_captor.str(), std::string("5 remaining"));
}

TEST(stream_operator_after_cure) {
  disease::DiseaseStatus diseaseStatus;
  std::stringstream stream_captor;


  diseaseStatus.place(3);
  diseaseStatus.place(2);
  diseaseStatus.cure();
  stream_captor << diseaseStatus;
  assert_equal(stream_captor.str(), std::string("Cured, 5 remaining"));
}

TEST(place_with_disease_eradicated) {
  disease::DiseaseStatus diseaseStatus;
  diseaseStatus.cure();
  assert_equal(diseaseStatus.place(1), 0);
  assert_equal(diseaseStatus.get_reserve(), DISEASE_RESERVE);
}

TEST(stream_operator_after_eradicated) {
  disease::DiseaseStatus diseaseStatus;
  std::stringstream stream_captor;
  diseaseStatus.cure();
  stream_captor << diseaseStatus;
  assert_equal(stream_captor.str(), std::string("Eradicated"));
}

TEST(treat_disease) {
  disease::DiseaseStatus diseaseStatus;
  diseaseStatus.place(3);
  assert_equal(diseaseStatus.treat(3), 1);
  assert_equal(diseaseStatus.get_reserve(), DISEASE_RESERVE - 2);
}

TEST(treat_cured_disease) {
  disease::DiseaseStatus diseaseStatus;
  diseaseStatus.place(3);
  diseaseStatus.place(2);
  diseaseStatus.place(1);
  diseaseStatus.cure();
  assert_equal(diseaseStatus.treat(3), 3);
  assert_equal(diseaseStatus.get_reserve(), DISEASE_RESERVE - 3);
  assert_equal(diseaseStatus.treat(2), 2);
  assert_equal(diseaseStatus.get_reserve(), DISEASE_RESERVE - 1);
  assert_equal(diseaseStatus.treat(1), 1);
  assert_equal(diseaseStatus.get_reserve(), DISEASE_RESERVE);
}
