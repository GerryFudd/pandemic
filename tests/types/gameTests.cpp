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

TEST(setup_and_get_cities) {
  gerryfudd::types::Game game;

  game.setup();

  assert_equal(game.get_city_count(), 48);

  gerryfudd::types::city::City istanbul = game.get_city("Istanbul"), 
    montreal = game.get_city("Montréal"),
    ho_chi_minh_city = game.get_city("Ho Chi Minh City"),
    sao_paulo = game.get_city("São Paulo");
  assert_equal<std::string>(istanbul.name, "Istanbul");
  assert_equal(istanbul.color, gerryfudd::types::disease::black);
  assert_equal(istanbul.population, 13576000);

  assert_equal<std::string>(montreal.name, "Montréal");
  assert_equal(montreal.color, gerryfudd::types::disease::blue);
  assert_equal(montreal.population, 3429000);

  assert_equal<std::string>(ho_chi_minh_city.name, "Ho Chi Minh City");
  assert_equal(ho_chi_minh_city.color, gerryfudd::types::disease::red);
  assert_equal(ho_chi_minh_city.population, 8314000);

  assert_equal<std::string>(sao_paulo.name, "São Paulo");
  assert_equal(sao_paulo.color, gerryfudd::types::disease::yellow);
  assert_equal(sao_paulo.population, 20186000);
}

TEST(setup_and_get_state) {
  gerryfudd::types::Game game;

  game.setup();

  gerryfudd::types::city::City current_city;
  gerryfudd::types::city::CityState current_state;
  int with_one = 0, with_two = 0, with_three = 0, current_count;

  for (std::map<std::string, gerryfudd::types::city::City>::iterator cursor = game.city_begin(); cursor != game.city_end(); cursor++) {
    current_city = cursor->second;
    current_state = game.get_state(cursor->first);
    current_count = current_state.disease_count[current_city.color];
    switch (current_count)
    {
    case 1:
      with_one++;
      break;
    case 2:
      with_two++;
      break;
    case 3:
      with_three++;
      break;
    default:
      break;
    }
  }

  assert_equal(with_one, 3);
  assert_equal(with_two, 3);
  assert_equal(with_three, 3);
}

TEST(setup_and_get_reserve) {
  gerryfudd::types::Game game;
  game.setup();

  int total_reserve = 0;
  total_reserve += game.get_reserve(gerryfudd::types::disease::black);
  total_reserve += game.get_reserve(gerryfudd::types::disease::blue);
  total_reserve += game.get_reserve(gerryfudd::types::disease::red);
  total_reserve += game.get_reserve(gerryfudd::types::disease::yellow);

  assert_equal(total_reserve, 4 * DISEASE_RESERVE - 18);
}
