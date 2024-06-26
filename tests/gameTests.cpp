#include <Framework.hpp>
#include <Assertions.inl>
#include <game.hpp>
#include <set>
#include <data/city_data.hpp>

using namespace gerryfudd::test;
using namespace gerryfudd::core;

TEST(get_state) {
  Game game;

  GameState game_state = game.get_state();

  assert_equal(game_state.diseases[disease::black].reserve, DISEASE_RESERVE);
  assert_equal(game_state.diseases[disease::blue].reserve, DISEASE_RESERVE);
  assert_equal(game_state.diseases[disease::red].reserve, DISEASE_RESERVE);
  assert_equal(game_state.diseases[disease::yellow].reserve, DISEASE_RESERVE);
  assert_false(game_state.diseases[disease::black].cured, "Black is not cured at the start of the game.");
  assert_false(game_state.diseases[disease::blue].cured, "Blue is not cured at the start of the game.");
  assert_false(game_state.diseases[disease::red].cured, "Red is not cured at the start of the game.");
  assert_false(game_state.diseases[disease::yellow].cured, "Yellow is not cured at the start of the game.");
}

TEST(setup_initial_state) {
  GameState game_state = initialize_state();

  assert_equal<int>(game_state.cities.size(), 48);

  city::City istanbul = game_state.cities["Istanbul"], 
    montreal = game_state.cities["Montréal"],
    ho_chi_minh_city = game_state.cities["Ho Chi Minh City"],
    sao_paulo = game_state.cities["São Paulo"];
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

  assert_equal<int>(istanbul.neighbors.size(), 5);
  assert_equal<int>(montreal.neighbors.size(), 2);
  assert_equal<int>(ho_chi_minh_city.neighbors.size(), 4);
  assert_equal<int>(sao_paulo.neighbors.size(), 4);
}

TEST(setup_and_get_disease_counts) {
  GameState game_state = initialize_state();

  city::City current_city;
  city::CityState current_state;
  int with_one = 0, with_two = 0, with_three = 0, current_count;

  for (std::map<std::string, city::City>::iterator cursor = game_state.cities.begin(); cursor != game_state.cities.end(); cursor++) {
    current_city = cursor->second;
    current_state = game_state.board[cursor->first];
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

TEST(setup_and_disease_reserve) {
  GameState game_state = initialize_state();

  int total_reserve = 0;
  total_reserve += game_state.diseases[disease::black].reserve;
  total_reserve += game_state.diseases[disease::blue].reserve;
  total_reserve += game_state.diseases[disease::red].reserve;
  total_reserve += game_state.diseases[disease::yellow].reserve;

  assert_equal(total_reserve, 4 * DISEASE_RESERVE - 18);
}

TEST(setup_and_players) {
  GameState game_state;
  game_state = initialize_state(easy, 2);
  assert_equal<int>(game_state.players.size(), 2);
  assert_equal<std::string>(game_state.player_locations[game_state.players[0].role], CDC_LOCATION);
  assert_equal<std::string>(game_state.player_locations[game_state.players[1].role], CDC_LOCATION);

  game_state = initialize_state(easy, 3);
  assert_equal<int>(game_state.players.size(), 3);
  assert_equal<std::string>(game_state.player_locations[game_state.players[0].role], CDC_LOCATION);
  assert_equal<std::string>(game_state.player_locations[game_state.players[1].role], CDC_LOCATION);
  assert_equal<std::string>(game_state.player_locations[game_state.players[2].role], CDC_LOCATION);

  game_state = initialize_state(easy, 4);
  assert_equal<int>(game_state.players.size(), 4);
  assert_equal<std::string>(game_state.player_locations[game_state.players[0].role], CDC_LOCATION);
  assert_equal<std::string>(game_state.player_locations[game_state.players[1].role], CDC_LOCATION);
  assert_equal<std::string>(game_state.player_locations[game_state.players[2].role], CDC_LOCATION);
  assert_equal<std::string>(game_state.player_locations[game_state.players[3].role], CDC_LOCATION);
}

TEST(setup_and_get_research_facility) {
  GameState game_state = initialize_state();

  assert_true(game_state.board[CDC_LOCATION].research_facility, "The CDC location starts with a research facility.");
  assert_equal(game_state.research_facility_reserve, 5);
}

TEST(setup_get_player_by_role) {
  GameState game_state = initialize_state(easy, 4);

  assert_equal<int>(game_state.players.size(), 4);

  for (std::vector<player::Player>::iterator cursor = game_state.players.begin(); cursor != game_state.players.end(); cursor++) {
    player::Player player_by_role = game_state.get_player(cursor->role);
    assert_equal(player_by_role.role, cursor->role);
    assert_equal<int>(cursor->hand.contents.size(), 2);
    assert_equal<int>(player_by_role.hand.contents.size(), 2);
    for (int j = 0; j < 2; j++) {
      assert_equal(cursor->hand.contents[j].name, player_by_role.hand.contents[j].name); 
    }
  }
}

TEST(get_initial_infection_rate) {
  GameState game_state = initialize_state();
  assert_equal(game_state.infection_rate_level, 0);
  assert_equal(game_state.get_infection_rate(), 2);
}

TEST(draw_infection_card) {
  GameState game_state = initialize_state(easy, 4);

  int reserve_black{game_state.diseases[disease::black].reserve},
  reserve_blue{game_state.diseases[disease::blue].reserve},
  reserve_red{game_state.diseases[disease::red].reserve},
  reserve_yellow{game_state.diseases[disease::yellow].reserve};

  Game game{game_state};
  assert_false(game.draw_infection_card(), "The game should not end in a loss after one infection card.");
  assert_false(game.draw_infection_card(), "The game should not end in a loss after two infection cards.");

  assert_true(
    game.get_state().diseases[disease::black].reserve + game.get_state().diseases[disease::blue].reserve + game.get_state().diseases[disease::red].reserve + game.get_state().diseases[disease::yellow].reserve >=
    reserve_black + reserve_blue + reserve_red + reserve_yellow - 2,
    "There should be at most 2 disease cubes placed."
  );

  city::City current_city;
  city::CityState current_state;
  int with_one = 0, with_two = 0, with_three = 0, current_count;

  for (std::map<std::string, city::City>::iterator cursor = game_state.cities.begin(); cursor != game_state.cities.end(); cursor++) {
    current_city = cursor->second;
    current_state = game_state.board[cursor->first];
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

  assert_true(with_one <= 5 && with_one >= 3, "There should be between 3 and 5 cities with one disease cube.");
  assert_equal(with_two, 3);
  assert_equal(with_three, 3);
}

TEST(drive) {
  GameState game_state = initialize_state(easy, 4);
  Game game{game_state};
  
  player::Role first_role = game_state.players[0].role;
  city::City first_player_location = game_state.cities[game_state.player_locations[first_role]];

  assert_equal<std::string>(first_player_location.name, CDC_LOCATION);

  std::string destination = first_player_location.neighbors[0].name;
  game.drive(first_role, destination);

  game_state = game.get_state();
  assert_equal(game_state.player_locations[first_role], destination);
}

TEST(drive_requires_neighboring_city) {
  GameState game_state = initialize_state(easy, 4);
  Game game{game_state};
  
  player::Role first_role = game_state.players[0].role;

  bool exception_thrown = false;

  try {
    game.drive(first_role, "Istanbul");
  } catch (std::invalid_argument e) {
    exception_thrown = true;
    assert_equal<std::string>(e.what(), "This player can't drive to Istanbul");
  }
  assert_true(exception_thrown, "An exception should be thrown if you attempt an illegal drive.");
}

TEST(drive_updates_protection_quarantine_specialist) {
  GameState game_state{};
  gerryfudd::data::city::load_cities(&game_state.cities);

  game_state.players.push_back(player::Player(player::quarantine_specialist));
  game_state.player_locations[player::quarantine_specialist] = CDC_LOCATION;
  Game game{game_state};

  std::string destination = game_state.cities[CDC_LOCATION].neighbors[0].name;
  game.drive(player::quarantine_specialist, destination);

  // The destination is protected
  assert_true(game.get_state().prevent_placement(destination, disease::black), "Quarantine specialist should prevent black cubes in the city they occupy.");
  assert_true(game.get_state().prevent_placement(destination, disease::blue), "Quarantine specialist should prevent blue cubes in the city they occupy.");
  assert_true(game.get_state().prevent_placement(destination, disease::red), "Quarantine specialist should prevent red cubes in the city they occupy.");
  assert_true(game.get_state().prevent_placement(destination, disease::yellow), "Quarantine specialist should prevent yellow cubes in the city they occupy.");

  city::City destination_city = game.get_state().cities[destination];
  for (std::vector<city::City>::iterator cursor = destination_city.neighbors.begin(); cursor != destination_city.neighbors.end(); cursor++) {
    assert_true(game.get_state().prevent_placement(cursor->name, disease::black), "Quarantine specialist should prevent black cubes in all neighboring cities.");
    assert_true(game.get_state().prevent_placement(cursor->name, disease::blue), "Quarantine specialist should prevent blue cubes in all neighboring cities.");
    assert_true(game.get_state().prevent_placement(cursor->name, disease::red), "Quarantine specialist should prevent red cubes in all neighboring cities.");
    assert_true(game.get_state().prevent_placement(cursor->name, disease::yellow), "Quarantine specialist should prevent yellow cubes in all neighboring cities.");
  }
}

TEST(drive_updates_protection_medic) {
  GameState game_state{};
  gerryfudd::data::city::load_cities(&game_state.cities);

  game_state.players.push_back(player::Player(player::medic));
  game_state.player_locations[player::medic] = CDC_LOCATION;
  game_state.diseases[disease::blue].cured = true;
  Game game{game_state};

  std::string destination = game_state.cities[CDC_LOCATION].neighbors[0].name;
  game.drive(player::medic, destination);

  // The destination is protected from blue disease only
  assert_false(game.get_state().prevent_placement(destination, disease::black), "Medic should not prevent black cubes in the city they occupy.");
  assert_true(game.get_state().prevent_placement(destination, disease::blue), "Medic should prevent blue cubes in the city they occupy.");
  assert_false(game.get_state().prevent_placement(destination, disease::red), "Medic should not prevent red cubes in the city they occupy.");
  assert_false(game.get_state().prevent_placement(destination, disease::yellow), "Medic should not prevent yellow cubes in the city they occupy.");

  city::City destination_city = game.get_state().cities[destination];
  for (std::vector<city::City>::iterator cursor = destination_city.neighbors.begin(); cursor != destination_city.neighbors.end(); cursor++) {
    assert_false(game.get_state().prevent_placement(cursor->name, disease::black), "Medic should not prevent black cubes in any neighboring cities.");
    assert_false(game.get_state().prevent_placement(cursor->name, disease::blue), "Medic should not prevent blue cubes in any neighboring cities.");
    assert_false(game.get_state().prevent_placement(cursor->name, disease::red), "Medic should not prevent red cubes in any neighboring cities.");
    assert_false(game.get_state().prevent_placement(cursor->name, disease::yellow), "Medic should not prevent yellow cubes in any neighboring cities.");
  }
}

TEST(setup_with_quarantine_specialist) {
  GameState game_state{};
  gerryfudd::data::city::load_cities(&game_state.cities);
  game_state.players.push_back(player::Player(player::quarantine_specialist));
  game_state.player_locations[player::quarantine_specialist] = CDC_LOCATION;
  Game game{game_state};

  city::City cdc_location = game.get_state().cities[CDC_LOCATION];
  assert_true(game.get_state().prevent_placement(CDC_LOCATION, disease::black), "Quarantine specialist should prevent black cubes in the city they occupy.");
  assert_true(game.get_state().prevent_placement(CDC_LOCATION, disease::blue), "Quarantine specialist should prevent blue cubes in the city they occupy.");
  assert_true(game.get_state().prevent_placement(CDC_LOCATION, disease::red), "Quarantine specialist should prevent red cubes in the city they occupy.");
  assert_true(game.get_state().prevent_placement(CDC_LOCATION, disease::yellow), "Quarantine specialist should prevent yellow cubes in the city they occupy.");

  for (std::vector<city::City>::iterator cursor = cdc_location.neighbors.begin(); cursor != cdc_location.neighbors.end(); cursor++) {
    assert_true(game.get_state().prevent_placement(cursor->name, disease::black), "Quarantine specialist should prevent black cubes in all neighboring cities.");
    assert_true(game.get_state().prevent_placement(cursor->name, disease::blue), "Quarantine specialist should prevent blue cubes in all neighboring cities.");
    assert_true(game.get_state().prevent_placement(cursor->name, disease::red), "Quarantine specialist should prevent red cubes in all neighboring cities.");
    assert_true(game.get_state().prevent_placement(cursor->name, disease::yellow), "Quarantine specialist should prevent yellow cubes in all neighboring cities.");
  }
}

TEST(direct_flight_quarantine_specialist) {
  GameState game_state{};
  gerryfudd::data::city::load_cities(&game_state.cities);

  game_state.players.push_back(player::Player(player::quarantine_specialist));
  game_state.player_locations[player::quarantine_specialist] = CDC_LOCATION;
  std::string destination = "Istanbul";
  game_state.add_card(player::quarantine_specialist, card::Card(destination, card::player));
  Game game{game_state};

  game.direct_flight(player::quarantine_specialist, destination);

  // The starting CDC location is unprotected
  assert_false(game.get_state().prevent_placement(CDC_LOCATION, disease::black), "Quarantine specialist no longer prevents black cubes in the city they leave.");
  assert_false(game.get_state().prevent_placement(CDC_LOCATION, disease::blue), "Quarantine specialist no longer prevents blue cubes in the city they leave.");
  assert_false(game.get_state().prevent_placement(CDC_LOCATION, disease::red), "Quarantine specialist no longer prevents red cubes in the city they leave.");
  assert_false(game.get_state().prevent_placement(CDC_LOCATION, disease::yellow), "Quarantine specialist no longer prevents yellow cubes in the city they leave.");

  city::City cdc_location = game.get_state().cities[CDC_LOCATION];
  for (std::vector<city::City>::iterator cursor = cdc_location.neighbors.begin(); cursor != cdc_location.neighbors.end(); cursor++) {
    assert_false(game.get_state().prevent_placement(cursor->name, disease::black), "Quarantine specialist no longer prevents black cubes in the neighbors of the city they leave.");
    assert_false(game.get_state().prevent_placement(cursor->name, disease::blue), "Quarantine specialist no longer prevents prevent blue cubes in the neighbors of the city they leave.");
    assert_false(game.get_state().prevent_placement(cursor->name, disease::red), "Quarantine specialist no longer prevents prevent red cubes in the neighbors of the city they leave.");
    assert_false(game.get_state().prevent_placement(cursor->name, disease::yellow), "Quarantine specialist no longer prevents prevent yellow cubes in the neighbors of the city they leave.");
  }

  // The destination is protected
  assert_true(game.get_state().prevent_placement(destination, disease::black), "Quarantine specialist should prevent black cubes in the city they occupy.");
  assert_true(game.get_state().prevent_placement(destination, disease::blue), "Quarantine specialist should prevent blue cubes in the city they occupy.");
  assert_true(game.get_state().prevent_placement(destination, disease::red), "Quarantine specialist should prevent red cubes in the city they occupy.");
  assert_true(game.get_state().prevent_placement(destination, disease::yellow), "Quarantine specialist should prevent yellow cubes in the city they occupy.");

  city::City destination_city = game.get_state().cities[destination];
  for (std::vector<city::City>::iterator cursor = destination_city.neighbors.begin(); cursor != destination_city.neighbors.end(); cursor++) {
    assert_true(game.get_state().prevent_placement(cursor->name, disease::black), "Quarantine specialist should prevent black cubes in all neighboring cities.");
    assert_true(game.get_state().prevent_placement(cursor->name, disease::blue), "Quarantine specialist should prevent blue cubes in all neighboring cities.");
    assert_true(game.get_state().prevent_placement(cursor->name, disease::red), "Quarantine specialist should prevent red cubes in all neighboring cities.");
    assert_true(game.get_state().prevent_placement(cursor->name, disease::yellow), "Quarantine specialist should prevent yellow cubes in all neighboring cities.");
  }
}

TEST(direct_flight_requires_card) {
  GameState game_state{};
  gerryfudd::data::city::load_cities(&game_state.cities);

  game_state.players.push_back(player::Player(player::contingency_planner));
  game_state.player_locations[player::contingency_planner] = CDC_LOCATION;
  std::string destination = "Istanbul";
  Game game{game_state};

  bool exception_thrown = false;
  try {
    game.direct_flight(player::contingency_planner, destination);
  } catch (std::invalid_argument e) {
    assert_equal<std::string>(e.what(), "This player doesn't have this card.");
    exception_thrown = true;
  }
  assert_true(exception_thrown, "Attempting an illegal charter flight should throw an exception.");
}

TEST(charter_flight_quarantine_specialist) {
  GameState game_state{};
  gerryfudd::data::city::load_cities(&game_state.cities);

  game_state.players.push_back(player::Player(player::quarantine_specialist));
  game_state.player_locations[player::quarantine_specialist] = CDC_LOCATION;
  game_state.add_card(player::quarantine_specialist, card::Card(CDC_LOCATION, card::player));
  Game game{game_state};
  std::string destination = "Istanbul";
  game.charter_flight(player::quarantine_specialist, destination);

  // The starting CDC location is unprotected
  assert_false(game.get_state().prevent_placement(CDC_LOCATION, disease::black), "Quarantine specialist no longer prevents black cubes in the city they leave.");
  assert_false(game.get_state().prevent_placement(CDC_LOCATION, disease::blue), "Quarantine specialist no longer prevents blue cubes in the city they leave.");
  assert_false(game.get_state().prevent_placement(CDC_LOCATION, disease::red), "Quarantine specialist no longer prevents red cubes in the city they leave.");
  assert_false(game.get_state().prevent_placement(CDC_LOCATION, disease::yellow), "Quarantine specialist no longer prevents yellow cubes in the city they leave.");

  city::City cdc_location = game.get_state().cities[CDC_LOCATION];
  for (std::vector<city::City>::iterator cursor = cdc_location.neighbors.begin(); cursor != cdc_location.neighbors.end(); cursor++) {
    assert_false(game.get_state().prevent_placement(cursor->name, disease::black), "Quarantine specialist no longer prevents black cubes in the neighbors of the city they leave.");
    assert_false(game.get_state().prevent_placement(cursor->name, disease::blue), "Quarantine specialist no longer prevents prevent blue cubes in the neighbors of the city they leave.");
    assert_false(game.get_state().prevent_placement(cursor->name, disease::red), "Quarantine specialist no longer prevents prevent red cubes in the neighbors of the city they leave.");
    assert_false(game.get_state().prevent_placement(cursor->name, disease::yellow), "Quarantine specialist no longer prevents prevent yellow cubes in the neighbors of the city they leave.");
  }

  // The destination is protected
  assert_true(game.get_state().prevent_placement(destination, disease::black), "Quarantine specialist should prevent black cubes in the city they occupy.");
  assert_true(game.get_state().prevent_placement(destination, disease::blue), "Quarantine specialist should prevent blue cubes in the city they occupy.");
  assert_true(game.get_state().prevent_placement(destination, disease::red), "Quarantine specialist should prevent red cubes in the city they occupy.");
  assert_true(game.get_state().prevent_placement(destination, disease::yellow), "Quarantine specialist should prevent yellow cubes in the city they occupy.");

  city::City destination_city = game.get_state().cities[destination];
  for (std::vector<city::City>::iterator cursor = destination_city.neighbors.begin(); cursor != destination_city.neighbors.end(); cursor++) {
    assert_true(game.get_state().prevent_placement(cursor->name, disease::black), "Quarantine specialist should prevent black cubes in all neighboring cities.");
    assert_true(game.get_state().prevent_placement(cursor->name, disease::blue), "Quarantine specialist should prevent blue cubes in all neighboring cities.");
    assert_true(game.get_state().prevent_placement(cursor->name, disease::red), "Quarantine specialist should prevent red cubes in all neighboring cities.");
    assert_true(game.get_state().prevent_placement(cursor->name, disease::yellow), "Quarantine specialist should prevent yellow cubes in all neighboring cities.");
  }
}

TEST(charter_flight_requires_card) {
  GameState game_state{};
  gerryfudd::data::city::load_cities(&game_state.cities);
  game_state.players.push_back(player::Player(player::contingency_planner));

  Game game{game_state};

  bool exception_thrown = false;
  try {
    game.charter_flight(player::contingency_planner, "Istanbul");
  } catch (std::invalid_argument e) {
    assert_equal<std::string>(e.what(), "This player doesn't have this card.");
    exception_thrown = true;
  }
  assert_true(exception_thrown, "Attempting an illegal charter flight should throw an exception.");
}

TEST(shuttle) {
  Game game{initialize_state()};

  std::string destination = "Istanbul";
  game.place_research_facility(destination);

  game.shuttle(game.get_state().players[0].role, destination);

  GameState game_state = game.get_state();
  assert_equal(game_state.player_locations[game_state.players[0].role], destination);
}

TEST(shuttle_requires_research_facility) {
  Game game{initialize_state()};

  bool exception_thrown = false;
  try {
    game.shuttle(game.get_state().players[0].role, "Istanbul");
  } catch(std::invalid_argument e) {
    exception_thrown = true;
  }
  assert_true(exception_thrown, "Players should not be able to shuttle without a research facility.");
}

TEST(dispatcher_direct_flight) {
  GameState game_state{};
  gerryfudd::data::city::load_cities(&game_state.cities);

  game_state.players.push_back(player::Player(player::dispatcher));
  game_state.player_locations[player::dispatcher] = CDC_LOCATION;

  std::string destination = "Istanbul";
  player::Role other_role = player::contingency_planner;

  game_state.add_card(player::dispatcher, card::Card(destination, card::player));

  game_state.players.push_back(player::Player(other_role));
  game_state.player_locations[other_role] = game_state.cities[CDC_LOCATION].neighbors[0].name;
  Game game{game_state};

  game.dispatcher_direct_flight(other_role, destination);

  assert_equal<std::string>(game.get_state().player_locations[other_role], destination);
}

TEST(dispatcher_direct_flight_requires_card) {
  GameState game_state{};
  gerryfudd::data::city::load_cities(&game_state.cities);

  game_state.players.push_back(player::Player(player::dispatcher));
  game_state.player_locations[player::dispatcher] = CDC_LOCATION;

  std::string destination = "Istanbul";
  player::Role other_role = player::contingency_planner;
  
  game_state.players.push_back(player::Player(other_role));
  game_state.player_locations[other_role] = game_state.cities[CDC_LOCATION].neighbors[0].name;
  Game game{game_state};

  bool exception_thrown = false;
  try {
    game.dispatcher_direct_flight(other_role, "Istanbul");
  } catch (std::invalid_argument e) {
    assert_equal<std::string>(e.what(), "This player doesn't have this card.");
    exception_thrown = true;
  }
  assert_true(exception_thrown, "Attempting an illegal dispatcher direct flight should throw an exception.");
}

TEST(dispatcher_charter_flight) {
  GameState game_state{};
  gerryfudd::data::city::load_cities(&game_state.cities);

  game_state.players.push_back(player::Player(player::dispatcher));
  game_state.player_locations[player::dispatcher] = CDC_LOCATION;

  std::string destination = "Istanbul";
  player::Role other_role = player::contingency_planner;

  
  game_state.players.push_back(player::Player(other_role));
  game_state.player_locations[other_role] = game_state.cities[CDC_LOCATION].neighbors[0].name;
  game_state.add_card(player::dispatcher, card::Card(game_state.player_locations[other_role], card::player));
  Game game{game_state};

  game.dispatcher_charter_flight(other_role, destination);

  assert_equal<std::string>(game.get_state().player_locations[other_role], destination);
}

TEST(dispatcher_charter_flight_requires_card) {
  GameState game_state{};
  gerryfudd::data::city::load_cities(&game_state.cities);

  game_state.players.push_back(player::Player(player::dispatcher));
  game_state.player_locations[player::dispatcher] = CDC_LOCATION;

  std::string destination = "Istanbul";
  player::Role other_role = player::contingency_planner;

  
  game_state.players.push_back(player::Player(other_role));
  game_state.player_locations[other_role] = game_state.cities[CDC_LOCATION].neighbors[0].name;
  Game game{game_state};

  bool exception_thrown = false;
  try {
    game.dispatcher_charter_flight(other_role, destination);
  } catch (std::invalid_argument e) {
    assert_equal<std::string>(e.what(), "This player doesn't have this card.");
    exception_thrown = true;
  }
  assert_true(exception_thrown, "Attempting an illegal dispatcher charter flight should throw an exception.");
}

TEST(dispatcher_conference) {
  GameState game_state{};
  gerryfudd::data::city::load_cities(&game_state.cities);

  game_state.players.push_back(player::Player(player::dispatcher));
  game_state.player_locations[player::dispatcher] = CDC_LOCATION;

  std::string destination = "Istanbul";
  player::Role other_roles[] = {player::contingency_planner, player::medic };

  
  game_state.players.push_back(player::Player(other_roles[0]));
  game_state.player_locations[other_roles[0]] = CDC_LOCATION;
  game_state.players.push_back(player::Player(other_roles[1]));
  game_state.player_locations[other_roles[1]] = "Istanbul";
  Game game{game_state};

  game.dispatcher_conference(other_roles[0], other_roles[1]);
  assert_equal<std::string>(game.get_state().player_locations[other_roles[0]], "Istanbul");
}

TEST(treat) {
  GameState game_state{};
  gerryfudd::data::city::load_cities(&game_state.cities);
  player::Role non_medic = player::contingency_planner;

  game_state.players.push_back(player::Player(non_medic));
  game_state.player_locations[non_medic] = CDC_LOCATION;
  game_state.board[CDC_LOCATION].disease_count[disease::blue] = 3;

  Game game{game_state};

  // Have the non-medic treat the disease.
  game.treat(non_medic, disease::blue);

  // Then there should be two disease markers left.
  assert_equal(game.get_state().board[CDC_LOCATION].disease_count[disease::blue], 2);
}

TEST(treat_as_medic) {
  GameState game_state{};
  gerryfudd::data::city::load_cities(&game_state.cities);

  game_state.players.push_back(player::Player(player::medic));
  game_state.player_locations[player::medic] = CDC_LOCATION;
  game_state.board[CDC_LOCATION].disease_count[disease::blue] = 3;

  Game game{game_state};

  // Have the medic treat the disease.
  game.treat(player::medic, disease::blue);

  // Then there should be no disease markers left.
  assert_equal(game.get_state().board[CDC_LOCATION].disease_count[disease::blue], 0);
}

TEST(share) {
  GameState game_state{};
  gerryfudd::data::city::load_cities(&game_state.cities);

  game_state.players.push_back(player::Player(player::contingency_planner));
  game_state.player_locations[player::contingency_planner] = CDC_LOCATION;
  game_state.add_card(player::contingency_planner, card::Card(CDC_LOCATION, card::player));

  game_state.players.push_back(player::Player(player::dispatcher));
  game_state.player_locations[player::dispatcher] = CDC_LOCATION;

  Game game{game_state};

  // Share the CDC_LOCATION card.
  game.share(player::contingency_planner, player::dispatcher);

  assert_equal<int>(game.get_state().get_player(player::contingency_planner).hand.contents.size(), 0);
  assert_equal<int>(game.get_state().get_player(player::dispatcher).hand.contents.size(), 1);
  assert_equal<std::string>(game.get_state().get_player(player::dispatcher).hand.contents[0].name, CDC_LOCATION);
}

TEST(share_requires_card_of_city) {
  GameState game_state = initialize_state();
  Game game{game_state};
  
  // There will be a player that isn't the researcher. Get their role and one other (the other player may or may not be the researcher).
  int non_researcher_index = game_state.players[0].role == player::researcher;
  player::Role non_researcher = game_state.players[non_researcher_index].role, other_role = game_state.players[!non_researcher_index].role;

  // Ensure that the non-researcher doesn't have the starting location card.
  try {
    game.remove_player_card(non_researcher, CDC_LOCATION);
  } catch(std::invalid_argument) {} // Ignore the exception if the player doesn't have this card already.

  // Attempt to share the CDC_LOCATION card.
  bool exception_thrown = false;
  try {
    game.share(non_researcher, other_role);
  } catch(std::invalid_argument e) {
    exception_thrown = true;
    assert_equal<std::string>(e.what(), "This player doesn't have this card.");
  }

  // Then
  assert_true(exception_thrown, "Sharing a card should be illegal if it isn't present.");
}

TEST(share_requires_same_city) {
  GameState game_state{};
  // Ensure that the player at index 0 has the starting location card.
  game_state.players.push_back(player::Player(player::contingency_planner));
  game_state.player_locations[player::contingency_planner] = CDC_LOCATION;
  game_state.add_card(player::contingency_planner, card::Card(CDC_LOCATION, card::player));

  game_state.players.push_back(player::Player(player::dispatcher));
  game_state.player_locations[player::dispatcher] = "Istanbul";

  Game game{game_state};

  // Attempt to share the CDC_LOCATION card.
  bool exception_thrown = false;
  try {
    game.share(player::contingency_planner, player::dispatcher);
  } catch(std::invalid_argument e) {
    exception_thrown = true;
    assert_equal<std::string>(e.what(), "Players must be in the same city to share cards.");
  }

  // Then
  assert_true(exception_thrown, "Sharing a card should be illegal if the players aren't in the same city.");
}

TEST(share_as_researcher) {
  GameState game_state{};
  std::string non_starting_location = "Istanbul";

  game_state.players.push_back(player::Player(player::researcher));
  game_state.player_locations[player::researcher] = CDC_LOCATION;
  // Ensure that the researcher has some card other than the starting location and that the non-researcher doesn't have this card.
  game_state.add_card(player::researcher, card::Card(non_starting_location, card::player));

  player::Role non_researcher = player::contingency_planner;
  game_state.players.push_back(player::Player(non_researcher));
  game_state.player_locations[non_researcher] = CDC_LOCATION;

  Game game{game_state};

  // Share the non-starting location card.
  game.researcher_share(non_starting_location, non_researcher);

  assert_equal<int>(game.get_state().get_player(player::researcher).hand.contents.size(), 0);
  assert_equal<int>(game.get_state().get_player(non_researcher).hand.contents.size(), 1);
  assert_equal<std::string>(game.get_state().get_player(non_researcher).hand.contents[0].name, non_starting_location);
}

TEST(share_as_researcher_requires_same_city) {
  GameState game_state{};
  std::string non_starting_location = "Istanbul";

  game_state.players.push_back(player::Player(player::researcher));
  game_state.player_locations[player::researcher] = CDC_LOCATION;
  // Ensure that the researcher has some card other than the starting location and that the non-researcher doesn't have this card.
  game_state.add_card(player::researcher, card::Card(CDC_LOCATION, card::player));

  player::Role non_researcher = player::contingency_planner;
  game_state.players.push_back(player::Player(non_researcher));
  game_state.player_locations[non_researcher] = non_starting_location;

  Game game{game_state};

  // Attempt to share the CDC_LOCATION card.
  bool exception_thrown = false;
  try {
    game.researcher_share(CDC_LOCATION, non_researcher);
  } catch(std::invalid_argument e) {
    exception_thrown = true;
    assert_equal<std::string>(e.what(), "Players must be in the same city to share cards.");
  }

  // Then
  assert_true(exception_thrown, "Sharing a card should be illegal if the players aren't in the same city.");
}

TEST(cure_requires_five_cards_in_hand) {
  GameState game_state{};
  gerryfudd::data::city::load_cities(&game_state.cities);
  game_state.board[CDC_LOCATION].research_facility = true;

  // Suppose that the contingency planner has four of these cards.
  game_state.players.push_back(player::Player(player::contingency_planner));
  game_state.player_locations[player::contingency_planner] = CDC_LOCATION;
  std::string to_discard[] {"Algeirs", "Baghdad", "Cairo", "Chennai", "Delhi"};
  for (int i = 0; i < 4; i++) {
    game_state.add_card(player::contingency_planner, card::Card(to_discard[i], card::player));
  }
  Game game{game_state};
  
  bool exception_thrown = false;
  try {
    game.cure(player::contingency_planner, to_discard);
  } catch(std::invalid_argument e) {
    exception_thrown = true;
    assert_equal<std::string>(e.what(), "This player doesn't have this card.");
  }

  assert_true(exception_thrown, "Attempting to cure with cards that aren't in your hand should be illegal.");
  // The player's hand shouldn't be different after a failure.
  assert_equal<int>(game.get_state().get_player(player::contingency_planner).hand.contents.size(), 4);
  for (int i = 0; i < 4; i++) {
    assert_equal<std::string>(game.get_state().get_player(player::contingency_planner).hand.contents[i].name, to_discard[i]);
  }
}

TEST(cure_requires_five_cards_of_the_same_color) {
  GameState game_state{};
  gerryfudd::data::city::load_cities(&game_state.cities);
  game_state.board[CDC_LOCATION].research_facility = true;

  // Suppose that the contingency planner has four of these cards.
  game_state.players.push_back(player::Player(player::contingency_planner));
  game_state.player_locations[player::contingency_planner] = CDC_LOCATION;
  std::string to_discard[] {"Algeirs", "Baghdad", "Cairo", "Chennai", "Atlanta"};
  for (int i = 0; i < 5; i++) {
    game_state.add_card(player::contingency_planner, card::Card(to_discard[i], card::player));
  }
  Game game{game_state};
  
  bool exception_thrown = false;
  try {
    game.cure(game.get_state().players[0].role, to_discard);
  } catch(std::invalid_argument e) {
    exception_thrown = true;
    assert_equal<std::string>(e.what(), "Curing a disease requires that all discarded cards share a color.");
  }

  assert_true(exception_thrown, "Attempting to cure with cards that aren't the same color should be illegal.");
  // The player's hand shouldn't be different after a failure.
  assert_equal<int>(game.get_state().get_player(player::contingency_planner).hand.contents.size(), 5);
  for (int i = 0; i < 5; i++) {
    assert_equal<std::string>(game.get_state().get_player(player::contingency_planner).hand.contents[i].name, to_discard[i]);
  }
}

TEST(cure_requires_research_facility) {
  GameState game_state{};
  gerryfudd::data::city::load_cities(&game_state.cities);

  // Suppose that the contingency planner has four of these cards.
  game_state.players.push_back(player::Player(player::contingency_planner));
  game_state.player_locations[player::contingency_planner] = CDC_LOCATION;
  std::string to_discard[] {"Algeirs", "Baghdad", "Cairo", "Chennai", "Delhi"};
  for (int i = 0; i < 5; i++) {
    game_state.add_card(player::contingency_planner, card::Card(to_discard[i], card::player));
  }
  Game game{game_state};
  
  bool exception_thrown = false;
  try {
    game.cure(game.get_state().players[0].role, to_discard);
  } catch(std::invalid_argument e) {
    exception_thrown = true;
    assert_equal<std::string>(e.what(), "Curing a disease may only occur in a city with a research facility.");
  }

  assert_true(exception_thrown, "Attempting to cure without a research facility should be illegal.");
  // The player's hand shouldn't be different after a failure.
  assert_equal<int>(game.get_state().get_player(player::contingency_planner).hand.contents.size(), 5);
  for (int i = 0; i < 5; i++) {
    assert_equal<std::string>(game.get_state().get_player(player::contingency_planner).hand.contents[i].name, to_discard[i]);
  }
}

TEST(cure) {
  GameState game_state{};
  gerryfudd::data::city::load_cities(&game_state.cities);
  game_state.board[CDC_LOCATION].research_facility = true;

  // Suppose that the contingency planner has four of these cards.
  game_state.players.push_back(player::Player(player::contingency_planner));
  game_state.player_locations[player::contingency_planner] = CDC_LOCATION;
  std::string to_discard[] {"Algeirs", "Baghdad", "Cairo", "Chennai", "Delhi"};
  for (int i = 0; i < 5; i++) {
    game_state.add_card(player::contingency_planner, card::Card(to_discard[i], card::player));
  }
  Game game{game_state};
  
  // Cure the disease
  game.cure(player::contingency_planner, to_discard);

  // The player's hand should have discarded the cards.
  assert_equal<int>(game.get_state().get_player(player::contingency_planner).hand.contents.size(), 0);
  assert_true(game.get_state().diseases[disease::black].cured, "The disease should be cured.");
}

TEST(cure_as_scientist_requires_four_cards_in_hand) {
  GameState game_state{};
  gerryfudd::data::city::load_cities(&game_state.cities);
  game_state.board[CDC_LOCATION].research_facility = true;

  // Suppose that the contingency planner has four of these cards.
  game_state.players.push_back(player::Player(player::scientist));
  game_state.player_locations[player::scientist] = CDC_LOCATION;
  std::string to_discard[] {"Algeirs", "Baghdad", "Cairo", "Chennai"};
  for (int i = 0; i < 3; i++) {
    game_state.add_card(player::scientist, card::Card(to_discard[i], card::player));
  }
  Game game{game_state};
  
  bool exception_thrown = false;
  try {
    game.scientist_cure(to_discard);
  } catch(std::invalid_argument e) {
    exception_thrown = true;
    assert_equal<std::string>(e.what(), "This player doesn't have this card.");
  }

  assert_true(exception_thrown, "Attempting to cure with cards that aren't in your hand should be illegal.");
  // The player's hand shouldn't be different after a failure.
  assert_equal<int>(game.get_state().get_player(player::scientist).hand.contents.size(), 3);
  for (int i = 0; i < 3; i++) {
    assert_equal<std::string>(game.get_state().get_player(player::scientist).hand.contents[i].name, to_discard[i]);
  }
}

TEST(cure_as_scientist_requires_four_cards_of_the_same_color) {
  GameState game_state{};
  gerryfudd::data::city::load_cities(&game_state.cities);
  game_state.board[CDC_LOCATION].research_facility = true;

  // Suppose that the contingency planner has four of these cards.
  game_state.players.push_back(player::Player(player::scientist));
  game_state.player_locations[player::scientist] = CDC_LOCATION;
  std::string to_discard[] {"Algeirs", "Baghdad", "Cairo", "Atlanta"};
  for (int i = 0; i < 4; i++) {
    game_state.add_card(player::scientist, card::Card(to_discard[i], card::player));
  }
  Game game{game_state};
  
  bool exception_thrown = false;
  try {
    game.scientist_cure(to_discard);
  } catch(std::invalid_argument e) {
    exception_thrown = true;
    assert_equal<std::string>(e.what(), "Curing a disease requires that all discarded cards share a color.");
  }

  assert_true(exception_thrown, "Attempting to cure with cards that aren't the same color should be illegal.");
  // The player's hand shouldn't be different after a failure.
  assert_equal<int>(game.get_state().get_player(player::scientist).hand.contents.size(), 4);
  for (int i = 0; i < 4; i++) {
    assert_equal<std::string>(game.get_state().get_player(player::scientist).hand.contents[i].name, to_discard[i]);
  }
}

TEST(cure_as_scientist_requires_research_facility) {
  GameState game_state{};
  gerryfudd::data::city::load_cities(&game_state.cities);

  // Suppose that the contingency planner has four of these cards.
  game_state.players.push_back(player::Player(player::scientist));
  game_state.player_locations[player::scientist] = CDC_LOCATION;
  std::string to_discard[] {"Algeirs", "Baghdad", "Cairo", "Chennai"};
  for (int i = 0; i < 4; i++) {
    game_state.add_card(player::scientist, card::Card(to_discard[i], card::player));
  }
  Game game{game_state};
  
  bool exception_thrown = false;
  try {
    game.scientist_cure(to_discard);
  } catch(std::invalid_argument e) {
    exception_thrown = true;
    assert_equal<std::string>(e.what(), "Curing a disease may only occur in a city with a research facility.");
  }

  assert_true(exception_thrown, "Attempting to cure without a research facility should be illegal.");
  // The player's hand shouldn't be different after a failure.
  assert_equal<int>(game.get_state().get_player(player::scientist).hand.contents.size(), 4);
  for (int i = 0; i < 4; i++) {
    assert_equal<std::string>(game.get_state().get_player(player::scientist).hand.contents[i].name, to_discard[i]);
  }
}

TEST(cure_as_scientist) {
  GameState game_state{};
  gerryfudd::data::city::load_cities(&game_state.cities);
  game_state.board[CDC_LOCATION].research_facility = true;

  // Suppose that the contingency planner has four of these cards.
  game_state.players.push_back(player::Player(player::scientist));
  game_state.player_locations[player::scientist] = CDC_LOCATION;
  std::string to_discard[] {"Algeirs", "Baghdad", "Cairo", "Chennai"};
  for (int i = 0; i < 4; i++) {
    game_state.add_card(player::scientist, card::Card(to_discard[i], card::player));
  }
  Game game{game_state};
  
  // Cure the disease
  game.scientist_cure(to_discard);

  // The scientist's hand should have discarded the cards.
  assert_equal<int>(game.get_state().get_player(player::scientist).hand.contents.size(), 0);
  assert_true(game.get_state().diseases[disease::black].cured, "The disease should be cured.");
}
TEST(reclaim_requires_event_type) {
  GameState game_state{};
  game_state.players.push_back(player::Player(player::contingency_planner));
  game_state.player_locations[player::contingency_planner] = CDC_LOCATION;
  game_state.player_deck.discard(card::Card(CDC_LOCATION, card::player));
  Game game{game_state};
  
  bool exception_thrown = false;
  try {
    game.reclaim(CDC_LOCATION);
  } catch(std::invalid_argument e) {
    exception_thrown = true;
    assert_equal<std::string>(e.what(), "City cards may not be reclaimed.");
  }
  assert_true(exception_thrown, "City cards should not be re-claimable.");
}
TEST(reclaim_requires_card_in_discard) {
  GameState game_state{};
  game_state.players.push_back(player::Player(player::contingency_planner));
  game_state.player_locations[player::contingency_planner] = CDC_LOCATION;
  Game game{game_state};
  
  bool exception_thrown = false;
  try {
    game.reclaim(ONE_QUIET_NIGHT);
  } catch(std::invalid_argument e) {
    exception_thrown = true;
    assert_equal<std::string>(e.what(), "The card One Quiet Night is not in the discard pile.");
  }
  assert_true(exception_thrown, "Only discarded cards may be re-claimed.");
}
TEST(reclaim) {
  GameState game_state{};
  game_state.players.push_back(player::Player(player::contingency_planner));
  game_state.player_locations[player::contingency_planner] = CDC_LOCATION;
  game_state.player_deck.discard(card::Card(ONE_QUIET_NIGHT, card::player, card::one_quiet_night));
  Game game{game_state};

  assert_false(card::contains(game.get_state().contingency_card, ONE_QUIET_NIGHT), "The event card should not be on the contingency planner card initially.");
  assert_equal<int>(game.get_state().player_deck.get_discard_contents().size(), 1);


  game.reclaim(ONE_QUIET_NIGHT);
  assert_true(card::contains(game.get_state().contingency_card, ONE_QUIET_NIGHT), "The event card should be on the contingency planner card after reclaiming it.");
  assert_equal<int>(game.get_state().player_deck.get_discard_contents().size(), 0);
}
TEST(reclaim_limited_to_one) {
  GameState game_state{};
  game_state.players.push_back(player::Player(player::contingency_planner));
  game_state.player_locations[player::contingency_planner] = CDC_LOCATION;
  game_state.player_deck.discard(card::Card(ONE_QUIET_NIGHT, card::player, card::one_quiet_night));
  game_state.player_deck.discard(card::Card(RESILIENT_POPULATION, card::player, card::resilient_population));
  Game game{game_state};

  assert_equal<int>(game.get_state().player_deck.get_discard_contents().size(), 2);

  game.reclaim(ONE_QUIET_NIGHT);
  assert_true(card::contains(game.get_state().contingency_card, ONE_QUIET_NIGHT), "The reclaimed event card should be on the contingency planner card after reclaiming it.");
  assert_false(card::contains(game.get_state().contingency_card, RESILIENT_POPULATION), "The other event card should not be on the contingency planner card.");

  game.reclaim(RESILIENT_POPULATION);
  assert_false(card::contains(game.get_state().contingency_card, ONE_QUIET_NIGHT), "The old event card should be evicted from the contingency planner card after reclaiming it.");
  assert_true(card::contains(game.get_state().contingency_card, RESILIENT_POPULATION), "The newly reclaimed event card should be on the contingency planner card.");

  assert_equal<int>(game.get_state().player_deck.get_discard_contents().size(), 0);
}
TEST(reclaim_requires_contingency_planner) {
  GameState game_state{};
  game_state.player_deck.discard(card::Card(ONE_QUIET_NIGHT, card::player, card::one_quiet_night));
  Game game{game_state};
  
  bool exception_thrown = false;
  try {
    game.reclaim(ONE_QUIET_NIGHT);
  } catch(std::invalid_argument e) {
    exception_thrown = true;
    assert_equal<std::string>(e.what(), "Reclaim is not usable without a Contingency Planner.");
  }
  assert_true(exception_thrown, "Reclaim should require a Contingency Planner.");
}

TEST(company_plane_requires_operations_expert) {
  GameState game_state{};
  gerryfudd::data::city::load_cities(&game_state.cities);

  game_state.board[CDC_LOCATION].research_facility = true;

  Game game{game_state};
  
  bool exception_thrown = false;
  try {
    game.company_plane("Istanbul", "Madrid");
  } catch(std::invalid_argument e) {
    exception_thrown = true;
    assert_equal<std::string>(e.what(), "Company plane is not usable without an Operations Expert.");
  }
  assert_true(exception_thrown, "Company plane should require an Operations Expert.");
}

TEST(company_plane_requires_discarded_card) {
  GameState game_state{};
  gerryfudd::data::city::load_cities(&game_state.cities);

  game_state.board[CDC_LOCATION].research_facility = true;

  game_state.players.push_back(player::Player(player::operations_expert));
  game_state.player_locations[player::operations_expert] = CDC_LOCATION;
  Game game{game_state};

  std::string to_discard = "Madrid";
  
  bool exception_thrown = false;
  try {
    game.company_plane("Istanbul", to_discard);
  } catch(std::invalid_argument e) {
    exception_thrown = true;
    assert_equal<std::string>(e.what(), "This player does not have this card.");
  }
  assert_true(exception_thrown, "Company plane should require the discarded card.");
}

TEST(company_plane_requires_research_facility) {
  GameState game_state{};
  gerryfudd::data::city::load_cities(&game_state.cities);

  game_state.players.push_back(player::Player(player::operations_expert));
  game_state.player_locations[player::operations_expert] = CDC_LOCATION;
  std::string to_discard = "Madrid";
  game_state.add_card(player::operations_expert, card::Card(to_discard, card::player));
  Game game{game_state};
  
  bool exception_thrown = false;
  try {
    game.company_plane("Istanbul", to_discard);
  } catch(std::invalid_argument e) {
    exception_thrown = true;
    assert_equal<std::string>(e.what(), "Company plane requires the Operations Expert to be in a city with a research facility.");
  }
  assert_true(exception_thrown, "Company plane should require a research facility.");
}

TEST(company_plane) {
  GameState game_state{};
  gerryfudd::data::city::load_cities(&game_state.cities);

  game_state.board[CDC_LOCATION].research_facility = true;

  game_state.players.push_back(player::Player(player::operations_expert));
  game_state.player_locations[player::operations_expert] = CDC_LOCATION;
  std::string destination = "Istanbul", to_discard = "Madrid";
  game_state.add_card(player::operations_expert, card::Card(to_discard, card::player));
  Game game{game_state};
  
  // Take the company plane
  game.company_plane(destination, to_discard);

  assert_false(card::contains(game.get_state().get_player(player::operations_expert).hand, to_discard), "The discarded card should be removed from the Operations Expert's hand.");
  assert_equal(game.get_state().player_deck.get_discard_contents().back().name, to_discard);
  assert_equal(game.get_state().player_locations[player::operations_expert], destination);
}

TEST(epidemic) {
  GameState game_state{};
  gerryfudd::data::city::load_cities(&game_state.cities);

  card::Card bottom_card = card::Card(CDC_LOCATION, card::infect);
  disease::DiseaseColor color = disease::blue;
  game_state.infection_deck.insert(bottom_card, 0);
  game_state.infection_deck.insert(card::Card("Istanbul", card::infect), 0);

  game_state.infection_deck.discard(card::Card("Algeirs", card::infect));
  game_state.infection_deck.discard(card::Card("Baghdad", card::infect));
  game_state.infection_deck.discard(card::Card("Cairo", card::infect));
  game_state.infection_deck.discard(card::Card("Chennai", card::infect));
  std::vector<card::Card> infection_discard_before = game_state.infection_deck.get_discard_contents();
  for (int i = 0; i < 4; i++) {
    game_state.board[infection_discard_before[i].name].disease_count[disease::black] = 1;
  }
  game_state.diseases[disease::black].reserve = DISEASE_RESERVE - 4;
  game_state.diseases[disease::blue].reserve = DISEASE_RESERVE;

  Game game{game_state};

  GameState game_state_before = game.get_state();


  assert_equal(game_state_before.board[CDC_LOCATION].disease_count[color], 0);
  assert_equal(game_state_before.infection_rate_level, 0);


  assert_equal<int>(infection_discard_before.size(), 4);
  assert_equal<int>(game_state_before.infection_deck.remaining(), 2);

  assert_false(game.epidemic(), "The first epidemic of the game shouldn't result in a loss.");

  GameState game_state_after = game.get_state();
  assert_equal(game_state_after.board[CDC_LOCATION].disease_count[color], 3);
  assert_equal(game_state_after.diseases[color].reserve, DISEASE_RESERVE - 3);
  assert_equal(game_state_after.infection_rate_level, 1);

  assert_equal<int>(game_state_after.infection_deck.get_discard_contents().size(), 0);

  struct CardCompare {
    bool operator() (const card::Card& lhs, const card::Card& rhs) const {
      return lhs.name < rhs.name;
    }
  };
  std::set<card::Card, CardCompare> expected_top_five(infection_discard_before.begin(), infection_discard_before.end());
  expected_top_five.insert(bottom_card);
  assert_equal<int>(game_state_after.infection_deck.remaining(), 6);
  std::set<card::Card>::iterator cursor;
  for (int i = 0; i < 5; i++) {
    cursor = expected_top_five.find(game_state_after.infection_deck.reveal(i));
    assert_false(cursor == expected_top_five.end(), "All of the top five infection cards should come from the discard.");
    expected_top_five.erase(cursor);
  }
}

TEST(get_player_choice_one_quiet_night) {
  GameState game_state{};

  gerryfudd::data::city::load_cities(&game_state.cities);

  game_state.players.push_back(player::Player(player::contingency_planner));
  game_state.player_locations[player::contingency_planner] = CDC_LOCATION;
  game_state.players.back().hand.contents.push_back(card::Card(CDC_LOCATION, card::player));

  game_state.players.push_back(player::Player(player::dispatcher));
  game_state.player_locations[player::dispatcher] = CDC_LOCATION;
  game_state.players.back().hand.contents.push_back(card::Card(ONE_QUIET_NIGHT, card::player, card::one_quiet_night));

  TurnState turn_state{player::contingency_planner, game_state.get_infection_rate()};

  std::vector<PlayerChoice> cp_choices = get_player_choices(player::contingency_planner, game_state, turn_state);
  assert_equal<int>(cp_choices.size(), 0);

  std::vector<PlayerChoice> disp_choices = get_player_choices(player::dispatcher, game_state, turn_state);
  assert_equal<int>(disp_choices.size(), 1);
  std::string expected_propmpt = "Play ";
  expected_propmpt += ONE_QUIET_NIGHT;
  expected_propmpt += " to skip the next infect step.";
  assert_equal<std::string>(disp_choices[0].prompt, expected_propmpt);
}

TEST(get_player_choice_resilient_population) {
  GameState game_state{};

  gerryfudd::data::city::load_cities(&game_state.cities);

  game_state.infection_deck.discard(card::Card(CDC_LOCATION, card::infect));
  game_state.infection_deck.discard(card::Card("Istanbul", card::infect));

// quarantine_specialist, scientist, researcher
  game_state.players.push_back(player::Player(player::medic));
  game_state.player_locations[player::medic] = CDC_LOCATION;
  game_state.players.push_back(player::Player(player::operations_expert));
  game_state.player_locations[player::operations_expert] = CDC_LOCATION;
  game_state.players.back().hand.contents.push_back(card::Card(RESILIENT_POPULATION, card::player, card::resilient_population));

  TurnState turn_state{player::medic, game_state.get_infection_rate()};

  std::vector<PlayerChoice> cp_choices = get_player_choices(player::medic, game_state, turn_state);
  assert_equal<int>(cp_choices.size(), 0);

  std::vector<PlayerChoice> disp_choices = get_player_choices(player::operations_expert, game_state, turn_state);
  assert_equal<int>(disp_choices.size(), 2);
  std::string expected_propmpt = "Play ";
  expected_propmpt += RESILIENT_POPULATION;
  expected_propmpt += " to remove ";
  expected_propmpt += CDC_LOCATION;
  expected_propmpt += " from the infection discard.";
  assert_equal<std::string>(disp_choices[0].prompt, expected_propmpt);
  expected_propmpt = "Play ";
  expected_propmpt += RESILIENT_POPULATION;
  expected_propmpt += " to remove Istanbul from the infection discard.";
  assert_equal<std::string>(disp_choices[1].prompt, expected_propmpt);
}
