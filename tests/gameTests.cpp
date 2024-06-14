#include <Framework.hpp>
#include <Assertions.inl>
#include <game.hpp>

using namespace gerryfudd::test;
using namespace gerryfudd::core;

TEST(get_reserve) {
  Game game;

  assert_equal(game.get_reserve(disease::DiseaseColor::black), DISEASE_RESERVE);
  assert_equal(game.get_reserve(disease::DiseaseColor::blue), DISEASE_RESERVE);
  assert_equal(game.get_reserve(disease::DiseaseColor::red), DISEASE_RESERVE);
  assert_equal(game.get_reserve(disease::DiseaseColor::yellow), DISEASE_RESERVE);
}

TEST(is_cured) {
  Game game;

  assert_false(game.is_cured(disease::DiseaseColor::black), "Black is not cured at the start of the game.");
  assert_false(game.is_cured(disease::DiseaseColor::blue), "Blue is not cured at the start of the game.");
  assert_false(game.is_cured(disease::DiseaseColor::red), "Red is not cured at the start of the game.");
  assert_false(game.is_cured(disease::DiseaseColor::yellow), "Yellow is not cured at the start of the game.");
}

TEST(setup_and_get_cities) {
  Game game;

  game.setup();

  assert_equal(game.get_city_count(), 48);

  city::City istanbul = game.get_city("Istanbul"), 
    montreal = game.get_city("Montréal"),
    ho_chi_minh_city = game.get_city("Ho Chi Minh City"),
    sao_paulo = game.get_city("São Paulo");
  assert_equal<std::string>(istanbul.name, "Istanbul");
  assert_equal(istanbul.color, disease::black);
  assert_equal(istanbul.population, 13576000);

  assert_equal<std::string>(montreal.name, "Montréal");
  assert_equal(montreal.color, disease::blue);
  assert_equal(montreal.population, 3429000);

  assert_equal<std::string>(ho_chi_minh_city.name, "Ho Chi Minh City");
  assert_equal(ho_chi_minh_city.color, disease::red);
  assert_equal(ho_chi_minh_city.population, 8314000);

  assert_equal<std::string>(sao_paulo.name, "São Paulo");
  assert_equal(sao_paulo.color, disease::yellow);
  assert_equal(sao_paulo.population, 20186000);
}

TEST(setup_and_view_city_neighbors) {
  Game game;

  game.setup();

  assert_equal(game.get_city_count(), 48);

  city::City istanbul = game.get_city("Istanbul"), 
    montreal = game.get_city("Montréal"),
    ho_chi_minh_city = game.get_city("Ho Chi Minh City"),
    sao_paulo = game.get_city("São Paulo");
  assert_equal<int>(istanbul.neighbors.size(), 5);
  assert_equal<int>(montreal.neighbors.size(), 2);
  assert_equal<int>(ho_chi_minh_city.neighbors.size(), 4);
  assert_equal<int>(sao_paulo.neighbors.size(), 4);
}

TEST(setup_and_get_state) {
  Game game;

  game.setup();

  city::City current_city;
  city::CityState current_state;
  int with_one = 0, with_two = 0, with_three = 0, current_count;

  for (std::map<std::string, city::City>::iterator cursor = game.city_begin(); cursor != game.city_end(); cursor++) {
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
  Game game;
  game.setup();

  int total_reserve = 0;
  total_reserve += game.get_reserve(disease::black);
  total_reserve += game.get_reserve(disease::blue);
  total_reserve += game.get_reserve(disease::red);
  total_reserve += game.get_reserve(disease::yellow);

  assert_equal(total_reserve, 4 * DISEASE_RESERVE - 18);
}

TEST(setup_and_get_players) {
  Game game;
  game.setup(Difficulty::easy, 2);
  assert_equal<int>(game.get_players_in(CDC_LOCATION).size(), 2);
  game.setup(Difficulty::easy, 3);
  assert_equal<int>(game.get_players_in(CDC_LOCATION).size(), 3);
  game.setup(Difficulty::easy, 4);
  assert_equal<int>(game.get_players_in(CDC_LOCATION).size(), 4);
}

TEST(setup_and_get_research_facility) {
  Game game;
  game.setup();
  assert_true(game.get_state(CDC_LOCATION).research_facility, "The CDC location starts with a research facility.");
  assert_equal<int>(game.get_research_facility_reserve(), 5);
}

TEST(get_player) {
  Game game;
  game.setup(Difficulty::easy, 4);

  assert_equal<int>(game.get_player_count(), 4);

  for (int i = 0; i < 4; i++) {
    player::Player player_by_index = game.get_player(i);
    assert_equal(game.get_player(i + 4).role, player_by_index.role);
    player::Player player_by_role = game.get_player(player_by_index.role);
    assert_equal(player_by_role.role, player_by_index.role);
    assert_equal<int>(player_by_index.hand.contents.size(), 2);
    assert_equal<int>(player_by_role.hand.contents.size(), 2);
    for (int j = 0; j < 2; j++) {
      assert_equal(player_by_index.hand.contents[j].name, player_by_role.hand.contents[j].name); 
    }
  }

}

TEST(get_initial_infection_rate) {
  Game game;
  game.setup();
  assert_equal(game.get_infection_rate(), 2);
}

TEST(draw_infection_card) {
  Game game;
  game.setup(easy, 4);

  int reserve_black{game.get_reserve(disease::black)},
  reserve_blue{game.get_reserve(disease::blue)},
  reserve_red{game.get_reserve(disease::red)},
  reserve_yellow{game.get_reserve(disease::yellow)};

  assert_false(game.draw_infection_card(), "The game should not end in a loss after one infection card.");
  assert_false(game.draw_infection_card(), "The game should not end in a loss after two infection cards.");

  assert_true(
    game.get_reserve(disease::black) + game.get_reserve(disease::blue) + game.get_reserve(disease::red) + game.get_reserve(disease::yellow) >=
    reserve_black + reserve_blue + reserve_red + reserve_yellow - 2,
    "There should be at most 2 disease cubes placed."
  );

  city::City current_city;
  city::CityState current_state;
  int with_one = 0, with_two = 0, with_three = 0, current_count;

  for (std::map<std::string, city::City>::iterator cursor = game.city_begin(); cursor != game.city_end(); cursor++) {
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

  assert_equal(with_one, 5);
  assert_equal(with_two, 3);
  assert_equal(with_three, 3);
}

TEST(drive) {
  Game game;
  game.setup(easy, 4);
  
  player::Role first_role = game.get_player(0).role;
  city::City first_player_location = game.get_city(first_role);

  assert_equal<std::string>(first_player_location.name, CDC_LOCATION);

  std::string destination = first_player_location.neighbors[0].name;
  game.drive(game.get_player(0).role, destination);

  assert_equal(game.get_city(first_role).name, destination);

  std::vector<player::Player> players_at_cdc =  game.get_players_in(CDC_LOCATION);
  for (std::vector<player::Player>::iterator cursor = players_at_cdc.begin(); cursor != players_at_cdc.end(); cursor++) {
    assert_false(cursor->role == first_role, "Player 0 should not be at the CDC anymore.");
  }
}

TEST(drive_requires_neighboring_city) {
  Game game;
  game.setup(easy, 4);
  
  player::Role first_role = game.get_player(0).role;

  bool exception_thrown = false;

  try {
    game.drive(first_role, "Istanbul");
  } catch (std::invalid_argument e) {
    exception_thrown = true;
    assert_equal<std::string>(e.what(), "This player can't drive to Istanbul");
  }
  assert_true(exception_thrown, "An exception should be thrown if you attempt an illegal drive.");
}

TEST(drive_updates_protection) {
  Game game;
  game.setup(easy, 4);
  game.add_role(player::quarantine_specialist);

  std::string destination = game.get_city(CDC_LOCATION).neighbors[0].name;
  game.drive(player::quarantine_specialist, destination);

  // The destination is protected
  assert_true(game.get_state(destination).prevent_placement(disease::black), "Quarantine specialist should prevent black cubes in the city they occupy.");
  assert_true(game.get_state(destination).prevent_placement(disease::blue), "Quarantine specialist should prevent blue cubes in the city they occupy.");
  assert_true(game.get_state(destination).prevent_placement(disease::red), "Quarantine specialist should prevent red cubes in the city they occupy.");
  assert_true(game.get_state(destination).prevent_placement(disease::yellow), "Quarantine specialist should prevent yellow cubes in the city they occupy.");

  city::City destination_city = game.get_city(destination);
  for (std::vector<city::City>::iterator cursor = destination_city.neighbors.begin(); cursor != destination_city.neighbors.end(); cursor++) {
    assert_true(game.get_state(cursor->name).prevent_placement(disease::black), "Quarantine specialist should prevent black cubes in all neighboring cities.");
    assert_true(game.get_state(cursor->name).prevent_placement(disease::blue), "Quarantine specialist should prevent blue cubes in all neighboring cities.");
    assert_true(game.get_state(cursor->name).prevent_placement(disease::red), "Quarantine specialist should prevent red cubes in all neighboring cities.");
    assert_true(game.get_state(cursor->name).prevent_placement(disease::yellow), "Quarantine specialist should prevent yellow cubes in all neighboring cities.");
  }
}

TEST(setup_with_quarantine_specialist) {
  Game game;
  game.setup();
  game.add_role(player::quarantine_specialist);

  city::City cdc_location = game.get_city(CDC_LOCATION);
  assert_true(game.get_state(CDC_LOCATION).prevent_placement(disease::black), "Quarantine specialist should prevent black cubes in the city they occupy.");
  assert_true(game.get_state(CDC_LOCATION).prevent_placement(disease::blue), "Quarantine specialist should prevent blue cubes in the city they occupy.");
  assert_true(game.get_state(CDC_LOCATION).prevent_placement(disease::red), "Quarantine specialist should prevent red cubes in the city they occupy.");
  assert_true(game.get_state(CDC_LOCATION).prevent_placement(disease::yellow), "Quarantine specialist should prevent yellow cubes in the city they occupy.");

  for (std::vector<city::City>::iterator cursor = cdc_location.neighbors.begin(); cursor != cdc_location.neighbors.end(); cursor++) {
    assert_true(game.get_state(cursor->name).prevent_placement(disease::black), "Quarantine specialist should prevent black cubes in all neighboring cities.");
    assert_true(game.get_state(cursor->name).prevent_placement(disease::blue), "Quarantine specialist should prevent blue cubes in all neighboring cities.");
    assert_true(game.get_state(cursor->name).prevent_placement(disease::red), "Quarantine specialist should prevent red cubes in all neighboring cities.");
    assert_true(game.get_state(cursor->name).prevent_placement(disease::yellow), "Quarantine specialist should prevent yellow cubes in all neighboring cities.");
  }
}

TEST(direct_flight_quarantine_specialist) {
  Game game;
  game.setup();
  game.add_role(player::quarantine_specialist);
  game.add_card(player::quarantine_specialist, card::Card("Istanbul", card::player, card::city));
  std::string destination = "Istanbul";
  game.direct_flight(player::quarantine_specialist, destination);

  // The starting CDC location is unprotected
  assert_false(game.get_state(CDC_LOCATION).prevent_placement(disease::black), "Quarantine specialist no longer prevents black cubes in the city they leave.");
  assert_false(game.get_state(CDC_LOCATION).prevent_placement(disease::blue), "Quarantine specialist no longer prevents blue cubes in the city they leave.");
  assert_false(game.get_state(CDC_LOCATION).prevent_placement(disease::red), "Quarantine specialist no longer prevents red cubes in the city they leave.");
  assert_false(game.get_state(CDC_LOCATION).prevent_placement(disease::yellow), "Quarantine specialist no longer prevents yellow cubes in the city they leave.");

  city::City cdc_location = game.get_city(CDC_LOCATION);
  for (std::vector<city::City>::iterator cursor = cdc_location.neighbors.begin(); cursor != cdc_location.neighbors.end(); cursor++) {
    assert_false(game.get_state(cursor->name).prevent_placement(disease::black), "Quarantine specialist no longer prevents black cubes in the neighbors of the city they leave.");
    assert_false(game.get_state(cursor->name).prevent_placement(disease::blue), "Quarantine specialist no longer prevents prevent blue cubes in the neighbors of the city they leave.");
    assert_false(game.get_state(cursor->name).prevent_placement(disease::red), "Quarantine specialist no longer prevents prevent red cubes in the neighbors of the city they leave.");
    assert_false(game.get_state(cursor->name).prevent_placement(disease::yellow), "Quarantine specialist no longer prevents prevent yellow cubes in the neighbors of the city they leave.");
  }

  // The destination is protected
  assert_true(game.get_state(destination).prevent_placement(disease::black), "Quarantine specialist should prevent black cubes in the city they occupy.");
  assert_true(game.get_state(destination).prevent_placement(disease::blue), "Quarantine specialist should prevent blue cubes in the city they occupy.");
  assert_true(game.get_state(destination).prevent_placement(disease::red), "Quarantine specialist should prevent red cubes in the city they occupy.");
  assert_true(game.get_state(destination).prevent_placement(disease::yellow), "Quarantine specialist should prevent yellow cubes in the city they occupy.");

  city::City destination_city = game.get_city(destination);
  for (std::vector<city::City>::iterator cursor = destination_city.neighbors.begin(); cursor != destination_city.neighbors.end(); cursor++) {
    assert_true(game.get_state(cursor->name).prevent_placement(disease::black), "Quarantine specialist should prevent black cubes in all neighboring cities.");
    assert_true(game.get_state(cursor->name).prevent_placement(disease::blue), "Quarantine specialist should prevent blue cubes in all neighboring cities.");
    assert_true(game.get_state(cursor->name).prevent_placement(disease::red), "Quarantine specialist should prevent red cubes in all neighboring cities.");
    assert_true(game.get_state(cursor->name).prevent_placement(disease::yellow), "Quarantine specialist should prevent yellow cubes in all neighboring cities.");
  }
}

TEST(direct_flight_requires_card) {
  Game game;
  game.setup();

  std::string destination = "";
  int player_index = 0, hand_index = 0;
  while (destination == "") {
    if (game.get_player(player_index).hand.contents[hand_index].type == card::city) {
      destination = game.get_player(player_index).hand.contents[hand_index].name;
    } else if (hand_index < game.get_player(player_index).hand.contents.size() - 1) {
      hand_index++;
    } else if (player_index < game.get_player_count() - 1) {
      hand_index = 0;
      player_index++;
    } else {
      throw AssertionFailure("Some player should have a city card.");
    }
  }

  bool exception_thrown = false;
  try {
    game.direct_flight(game.get_player(player_index + 1).role, destination);
  } catch (std::invalid_argument e) {
    assert_equal<std::string>(e.what(), "This player doesn't have this card.");
    exception_thrown = true;
  }
  assert_true(exception_thrown, "Attempting an illegal charter flight should throw an exception.");
}

TEST(charter_flight_quarantine_specialist) {
  Game game;
  game.setup();
  game.add_role(player::quarantine_specialist);
  game.add_card(player::quarantine_specialist, card::Card(CDC_LOCATION, card::player, card::city));
  std::string destination = "Istanbul";
  game.charter_flight(player::quarantine_specialist, destination);

  // The starting CDC location is unprotected
  assert_false(game.get_state(CDC_LOCATION).prevent_placement(disease::black), "Quarantine specialist no longer prevents black cubes in the city they leave.");
  assert_false(game.get_state(CDC_LOCATION).prevent_placement(disease::blue), "Quarantine specialist no longer prevents blue cubes in the city they leave.");
  assert_false(game.get_state(CDC_LOCATION).prevent_placement(disease::red), "Quarantine specialist no longer prevents red cubes in the city they leave.");
  assert_false(game.get_state(CDC_LOCATION).prevent_placement(disease::yellow), "Quarantine specialist no longer prevents yellow cubes in the city they leave.");

  city::City cdc_location = game.get_city(CDC_LOCATION);
  for (std::vector<city::City>::iterator cursor = cdc_location.neighbors.begin(); cursor != cdc_location.neighbors.end(); cursor++) {
    assert_false(game.get_state(cursor->name).prevent_placement(disease::black), "Quarantine specialist no longer prevents black cubes in the neighbors of the city they leave.");
    assert_false(game.get_state(cursor->name).prevent_placement(disease::blue), "Quarantine specialist no longer prevents prevent blue cubes in the neighbors of the city they leave.");
    assert_false(game.get_state(cursor->name).prevent_placement(disease::red), "Quarantine specialist no longer prevents prevent red cubes in the neighbors of the city they leave.");
    assert_false(game.get_state(cursor->name).prevent_placement(disease::yellow), "Quarantine specialist no longer prevents prevent yellow cubes in the neighbors of the city they leave.");
  }

  // The destination is protected
  assert_true(game.get_state(destination).prevent_placement(disease::black), "Quarantine specialist should prevent black cubes in the city they occupy.");
  assert_true(game.get_state(destination).prevent_placement(disease::blue), "Quarantine specialist should prevent blue cubes in the city they occupy.");
  assert_true(game.get_state(destination).prevent_placement(disease::red), "Quarantine specialist should prevent red cubes in the city they occupy.");
  assert_true(game.get_state(destination).prevent_placement(disease::yellow), "Quarantine specialist should prevent yellow cubes in the city they occupy.");

  city::City destination_city = game.get_city(destination);
  for (std::vector<city::City>::iterator cursor = destination_city.neighbors.begin(); cursor != destination_city.neighbors.end(); cursor++) {
    assert_true(game.get_state(cursor->name).prevent_placement(disease::black), "Quarantine specialist should prevent black cubes in all neighboring cities.");
    assert_true(game.get_state(cursor->name).prevent_placement(disease::blue), "Quarantine specialist should prevent blue cubes in all neighboring cities.");
    assert_true(game.get_state(cursor->name).prevent_placement(disease::red), "Quarantine specialist should prevent red cubes in all neighboring cities.");
    assert_true(game.get_state(cursor->name).prevent_placement(disease::yellow), "Quarantine specialist should prevent yellow cubes in all neighboring cities.");
  }
}

TEST(charter_flight_requires_card) {
  Game game;
  game.setup();

  card::Hand first_player_hand = game.get_player(0).hand;

  // This is the index of a player that doesn't have the starting location in their hand.
  int player_index = 0;
  for (std::vector<card::Card>::iterator cursor = first_player_hand.contents.begin(); cursor != first_player_hand.contents.end(); cursor++) {
    if (cursor->name == CDC_LOCATION) {
      player_index = 1;
      break;
    }
  }

  bool exception_thrown = false;
  try {
    game.charter_flight(game.get_player(player_index).role, "Istanbul");
  } catch (std::invalid_argument e) {
    assert_equal<std::string>(e.what(), "This player doesn't have this card.");
    exception_thrown = true;
  }
  assert_true(exception_thrown, "Attempting an illegal charter flight should throw an exception.");
}
