#include <Framework.hpp>
#include <Assertions.inl>
#include <types/game.hpp>

using namespace gerryfudd::test;

TEST(get_reserve) {
  gerryfudd::types::Game game;

  assert_equal(game.get_reserve(gerryfudd::types::disease::DiseaseColor::black), DISEASE_RESERVE);
  assert_equal(game.get_reserve(gerryfudd::types::disease::DiseaseColor::blue), DISEASE_RESERVE);
  assert_equal(game.get_reserve(gerryfudd::types::disease::DiseaseColor::red), DISEASE_RESERVE);
  assert_equal(game.get_reserve(gerryfudd::types::disease::DiseaseColor::yellow), DISEASE_RESERVE);
}

TEST(is_cured) {
  gerryfudd::types::Game game;

  assert_false(game.is_cured(gerryfudd::types::disease::DiseaseColor::black), "Black is not cured at the start of the game.");
  assert_false(game.is_cured(gerryfudd::types::disease::DiseaseColor::blue), "Blue is not cured at the start of the game.");
  assert_false(game.is_cured(gerryfudd::types::disease::DiseaseColor::red), "Red is not cured at the start of the game.");
  assert_false(game.is_cured(gerryfudd::types::disease::DiseaseColor::yellow), "Yellow is not cured at the start of the game.");
}
