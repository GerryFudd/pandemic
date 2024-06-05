#include <Framework.hpp>
#include <Assertions.inl>
#include <types/game.hpp>

#define SETUP_DIRECTORY "/home/david/Documents/projects/pandemic/public"

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

  game.setup(SETUP_DIRECTORY);

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

TEST(populate_deck) {
  std::string card_names[] = {"foo", "bar", "baz"};

  gerryfudd::types::card::Deck deck{gerryfudd::types::card::infect};
  assert_equal(deck.size(), 0);
  assert_equal(deck.remaining(), 0);

  for (int i = 0; i < 3; i++) {
    deck.discard(gerryfudd::types::card::Card(card_names[i], gerryfudd::types::card::infect));
    assert_equal(deck.size(), i + 1);
    assert_equal(deck.remaining(), 0);
  }

  deck.shuffle();
  assert_equal(deck.size(), 3);
  assert_equal(deck.remaining(), 3);

  std::vector<std::string> draws;
  while (draws.size() < 3)
  {
    std::string current = deck.draw().name;
    for (auto draw = draws.begin(); draw != draws.end(); draw++) {
      assert_false(current == *draw, (current + " should not have already been drawn.").c_str());
    }
    draws.push_back(current);
    assert_equal(deck.size(), deck.remaining());
    assert_equal<int>(deck.remaining() + draws.size(), 3);
  }
}

TEST(deck_rejects_wrong_type) {
  gerryfudd::types::card::Deck infect_deck{gerryfudd::types::card::infect};
  gerryfudd::types::card::Card infect_card{"Infect card", gerryfudd::types::card::infect}, player_card{"Player Card", gerryfudd::types::card::player};
  bool error_thrown{false};
  try {
    infect_deck.discard(infect_card);
  } catch (std::invalid_argument) {
    error_thrown = true;
  }
  assert_false(error_thrown, "Inserting the correct card type should not throw an error.");

  try {
    infect_deck.discard(player_card);
  } catch (std::invalid_argument) {
    error_thrown = true;
  }
  assert_true(error_thrown, "Inserting the wrong card type should throw an error.");
}

TEST(setup_and_get_state) {
  gerryfudd::types::Game game;

  game.setup(SETUP_DIRECTORY);

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
  game.setup(SETUP_DIRECTORY);

  int total_reserve = 0;
  total_reserve += game.get_reserve(gerryfudd::types::disease::black);
  total_reserve += game.get_reserve(gerryfudd::types::disease::blue);
  total_reserve += game.get_reserve(gerryfudd::types::disease::red);
  total_reserve += game.get_reserve(gerryfudd::types::disease::yellow);

  assert_equal(total_reserve, 4 * DISEASE_RESERVE - 18);
}
