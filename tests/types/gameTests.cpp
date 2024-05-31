#include <Framework.hpp>
#include <Assertions.inl>
#include <types/game.hpp>

using namespace gerryfudd::test;

TEST(disease_lookup) {
  gerryfudd::types::Game game;
  gerryfudd::types::disease::DiseaseStatus diseaseStatus;

  diseaseStatus = game.lookup_disease(gerryfudd::types::disease::DiseaseColor::black);
  assert_equal(diseaseStatus.get_reserve(), DISEASE_RESERVE);
  assert_false(diseaseStatus.is_cured(), "No diseases should be cured on initialization.");
}
