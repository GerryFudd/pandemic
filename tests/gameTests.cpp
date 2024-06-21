#include <Framework.hpp>
#include <Assertions.inl>
#include <game.hpp>
#include <set>

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
  Game game;

  game.setup();

  GameState game_state = game.get_state();

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
  Game game;

  game.setup();

  GameState game_state = game.get_state();

  city::City current_city;
  city::CityState current_state;
  int with_one = 0, with_two = 0, with_three = 0, current_count;

  for (std::map<std::string, city::City>::iterator cursor = game_state.cities.begin(); cursor != game_state.cities.end(); cursor++) {
    current_city = cursor->second;
    current_state = game.get_state().board[cursor->first];
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
  Game game;
  game.setup();
  GameState game_state = game.get_state();

  int total_reserve = 0;
  total_reserve += game_state.diseases[disease::black].reserve;
  total_reserve += game_state.diseases[disease::blue].reserve;
  total_reserve += game_state.diseases[disease::red].reserve;
  total_reserve += game_state.diseases[disease::yellow].reserve;

  assert_equal(total_reserve, 4 * DISEASE_RESERVE - 18);
}

TEST(setup_and_players) {
  Game game;
  GameState game_state;

  game.setup(easy, 2);
  game_state = game.get_state();
  assert_equal<int>(game_state.players.size(), 2);
  assert_equal<std::string>(game_state.player_locations[game_state.players[0].role], CDC_LOCATION);
  assert_equal<std::string>(game_state.player_locations[game_state.players[1].role], CDC_LOCATION);

  game.setup(easy, 3);
  game_state = game.get_state();
  assert_equal<int>(game_state.players.size(), 3);
  assert_equal<std::string>(game_state.player_locations[game_state.players[0].role], CDC_LOCATION);
  assert_equal<std::string>(game_state.player_locations[game_state.players[1].role], CDC_LOCATION);
  assert_equal<std::string>(game_state.player_locations[game_state.players[2].role], CDC_LOCATION);

  game.setup(easy, 4);
  game_state = game.get_state();
  assert_equal<int>(game_state.players.size(), 4);
  assert_equal<std::string>(game_state.player_locations[game_state.players[0].role], CDC_LOCATION);
  assert_equal<std::string>(game_state.player_locations[game_state.players[1].role], CDC_LOCATION);
  assert_equal<std::string>(game_state.player_locations[game_state.players[2].role], CDC_LOCATION);
  assert_equal<std::string>(game_state.player_locations[game_state.players[3].role], CDC_LOCATION);
}

TEST(setup_and_get_research_facility) {
  Game game;
  game.setup();
  GameState game_state = game.get_state();

  assert_true(game_state.board[CDC_LOCATION].research_facility, "The CDC location starts with a research facility.");
  assert_equal(game_state.research_facility_reserve, 5);
}

TEST(setup_get_player_by_role) {
  Game game;
  game.setup(easy, 4);
  GameState game_state = game.get_state();

  assert_equal<int>(game_state.players.size(), 4);

  for (std::vector<player::Player>::iterator cursor = game_state.players.begin(); cursor != game_state.players.end(); cursor++) {
    player::Player player_by_role = game.get_player(cursor->role);
    assert_equal(player_by_role.role, cursor->role);
    assert_equal<int>(cursor->hand.contents.size(), 2);
    assert_equal<int>(player_by_role.hand.contents.size(), 2);
    for (int j = 0; j < 2; j++) {
      assert_equal(cursor->hand.contents[j].name, player_by_role.hand.contents[j].name); 
    }
  }
}

TEST(get_initial_infection_rate) {
  Game game;
  game.setup();
  assert_equal(game.get_state().infection_rate_level, 0);
  assert_equal(game.get_state().get_infection_rate(), 2);
}

TEST(draw_infection_card) {
  Game game;
  game.setup(easy, 4);
  GameState game_state = game.get_state();

  int reserve_black{game_state.diseases[disease::black].reserve},
  reserve_blue{game_state.diseases[disease::blue].reserve},
  reserve_red{game_state.diseases[disease::red].reserve},
  reserve_yellow{game_state.diseases[disease::yellow].reserve};

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
  Game game;
  game.setup(easy, 4);
  GameState game_state = game.get_state();
  
  player::Role first_role = game_state.players[0].role;
  city::City first_player_location = game_state.cities[game_state.player_locations[first_role]];

  assert_equal<std::string>(first_player_location.name, CDC_LOCATION);

  std::string destination = first_player_location.neighbors[0].name;
  game.drive(first_role, destination);

  game_state = game.get_state();
  assert_equal(game_state.player_locations[first_role], destination);
}

TEST(drive_requires_neighboring_city) {
  Game game;
  game.setup(easy, 4);
  GameState game_state = game.get_state();
  
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

TEST(drive_updates_protection) {
  Game game;
  game.setup(easy, 4);
  game.add_role(player::quarantine_specialist);
  GameState game_state = game.get_state();

  std::string destination = game.get_state().cities[CDC_LOCATION].neighbors[0].name;
  game.drive(player::quarantine_specialist, destination);

  // The destination is protected
  assert_true(game.get_state().board[destination].prevent_placement(disease::black), "Quarantine specialist should prevent black cubes in the city they occupy.");
  assert_true(game.get_state().board[destination].prevent_placement(disease::blue), "Quarantine specialist should prevent blue cubes in the city they occupy.");
  assert_true(game.get_state().board[destination].prevent_placement(disease::red), "Quarantine specialist should prevent red cubes in the city they occupy.");
  assert_true(game.get_state().board[destination].prevent_placement(disease::yellow), "Quarantine specialist should prevent yellow cubes in the city they occupy.");

  city::City destination_city = game.get_state().cities[destination];
  for (std::vector<city::City>::iterator cursor = destination_city.neighbors.begin(); cursor != destination_city.neighbors.end(); cursor++) {
    assert_true(game.get_state().board[cursor->name].prevent_placement(disease::black), "Quarantine specialist should prevent black cubes in all neighboring cities.");
    assert_true(game.get_state().board[cursor->name].prevent_placement(disease::blue), "Quarantine specialist should prevent blue cubes in all neighboring cities.");
    assert_true(game.get_state().board[cursor->name].prevent_placement(disease::red), "Quarantine specialist should prevent red cubes in all neighboring cities.");
    assert_true(game.get_state().board[cursor->name].prevent_placement(disease::yellow), "Quarantine specialist should prevent yellow cubes in all neighboring cities.");
  }
}

TEST(setup_with_quarantine_specialist) {
  Game game;
  game.setup();
  game.add_role(player::quarantine_specialist);

  city::City cdc_location = game.get_state().cities[CDC_LOCATION];
  assert_true(game.get_state().board[CDC_LOCATION].prevent_placement(disease::black), "Quarantine specialist should prevent black cubes in the city they occupy.");
  assert_true(game.get_state().board[CDC_LOCATION].prevent_placement(disease::blue), "Quarantine specialist should prevent blue cubes in the city they occupy.");
  assert_true(game.get_state().board[CDC_LOCATION].prevent_placement(disease::red), "Quarantine specialist should prevent red cubes in the city they occupy.");
  assert_true(game.get_state().board[CDC_LOCATION].prevent_placement(disease::yellow), "Quarantine specialist should prevent yellow cubes in the city they occupy.");

  for (std::vector<city::City>::iterator cursor = cdc_location.neighbors.begin(); cursor != cdc_location.neighbors.end(); cursor++) {
    assert_true(game.get_state().board[cursor->name].prevent_placement(disease::black), "Quarantine specialist should prevent black cubes in all neighboring cities.");
    assert_true(game.get_state().board[cursor->name].prevent_placement(disease::blue), "Quarantine specialist should prevent blue cubes in all neighboring cities.");
    assert_true(game.get_state().board[cursor->name].prevent_placement(disease::red), "Quarantine specialist should prevent red cubes in all neighboring cities.");
    assert_true(game.get_state().board[cursor->name].prevent_placement(disease::yellow), "Quarantine specialist should prevent yellow cubes in all neighboring cities.");
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
  assert_false(game.get_state().board[CDC_LOCATION].prevent_placement(disease::black), "Quarantine specialist no longer prevents black cubes in the city they leave.");
  assert_false(game.get_state().board[CDC_LOCATION].prevent_placement(disease::blue), "Quarantine specialist no longer prevents blue cubes in the city they leave.");
  assert_false(game.get_state().board[CDC_LOCATION].prevent_placement(disease::red), "Quarantine specialist no longer prevents red cubes in the city they leave.");
  assert_false(game.get_state().board[CDC_LOCATION].prevent_placement(disease::yellow), "Quarantine specialist no longer prevents yellow cubes in the city they leave.");

  city::City cdc_location = game.get_state().cities[CDC_LOCATION];
  for (std::vector<city::City>::iterator cursor = cdc_location.neighbors.begin(); cursor != cdc_location.neighbors.end(); cursor++) {
    assert_false(game.get_state().board[cursor->name].prevent_placement(disease::black), "Quarantine specialist no longer prevents black cubes in the neighbors of the city they leave.");
    assert_false(game.get_state().board[cursor->name].prevent_placement(disease::blue), "Quarantine specialist no longer prevents prevent blue cubes in the neighbors of the city they leave.");
    assert_false(game.get_state().board[cursor->name].prevent_placement(disease::red), "Quarantine specialist no longer prevents prevent red cubes in the neighbors of the city they leave.");
    assert_false(game.get_state().board[cursor->name].prevent_placement(disease::yellow), "Quarantine specialist no longer prevents prevent yellow cubes in the neighbors of the city they leave.");
  }

  // The destination is protected
  assert_true(game.get_state().board[destination].prevent_placement(disease::black), "Quarantine specialist should prevent black cubes in the city they occupy.");
  assert_true(game.get_state().board[destination].prevent_placement(disease::blue), "Quarantine specialist should prevent blue cubes in the city they occupy.");
  assert_true(game.get_state().board[destination].prevent_placement(disease::red), "Quarantine specialist should prevent red cubes in the city they occupy.");
  assert_true(game.get_state().board[destination].prevent_placement(disease::yellow), "Quarantine specialist should prevent yellow cubes in the city they occupy.");

  city::City destination_city = game.get_state().cities[destination];
  for (std::vector<city::City>::iterator cursor = destination_city.neighbors.begin(); cursor != destination_city.neighbors.end(); cursor++) {
    assert_true(game.get_state().board[cursor->name].prevent_placement(disease::black), "Quarantine specialist should prevent black cubes in all neighboring cities.");
    assert_true(game.get_state().board[cursor->name].prevent_placement(disease::blue), "Quarantine specialist should prevent blue cubes in all neighboring cities.");
    assert_true(game.get_state().board[cursor->name].prevent_placement(disease::red), "Quarantine specialist should prevent red cubes in all neighboring cities.");
    assert_true(game.get_state().board[cursor->name].prevent_placement(disease::yellow), "Quarantine specialist should prevent yellow cubes in all neighboring cities.");
  }
}

TEST(direct_flight_requires_card) {
  Game game;
  game.setup();
  GameState game_state = game.get_state();

  std::string destination = "";
  int player_index = 0, hand_index = 0;
  while (destination == "") {
    if (game_state.players[player_index].hand.contents[hand_index].type == card::city) {
      destination = game_state.players[player_index].hand.contents[hand_index].name;
    } else if (hand_index < game_state.players[player_index].hand.contents.size() - 1) {
      hand_index++;
    } else if (player_index < game_state.players.size() - 1) {
      hand_index = 0;
      player_index++;
    } else {
      throw AssertionFailure("Some player should have a city card.");
    }
  }

  bool exception_thrown = false;
  try {
    game.direct_flight(game_state.players[(player_index + 1) % game_state.players.size()].role, destination);
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
  assert_false(game.get_state().board[CDC_LOCATION].prevent_placement(disease::black), "Quarantine specialist no longer prevents black cubes in the city they leave.");
  assert_false(game.get_state().board[CDC_LOCATION].prevent_placement(disease::blue), "Quarantine specialist no longer prevents blue cubes in the city they leave.");
  assert_false(game.get_state().board[CDC_LOCATION].prevent_placement(disease::red), "Quarantine specialist no longer prevents red cubes in the city they leave.");
  assert_false(game.get_state().board[CDC_LOCATION].prevent_placement(disease::yellow), "Quarantine specialist no longer prevents yellow cubes in the city they leave.");

  city::City cdc_location = game.get_state().cities[CDC_LOCATION];
  for (std::vector<city::City>::iterator cursor = cdc_location.neighbors.begin(); cursor != cdc_location.neighbors.end(); cursor++) {
    assert_false(game.get_state().board[cursor->name].prevent_placement(disease::black), "Quarantine specialist no longer prevents black cubes in the neighbors of the city they leave.");
    assert_false(game.get_state().board[cursor->name].prevent_placement(disease::blue), "Quarantine specialist no longer prevents prevent blue cubes in the neighbors of the city they leave.");
    assert_false(game.get_state().board[cursor->name].prevent_placement(disease::red), "Quarantine specialist no longer prevents prevent red cubes in the neighbors of the city they leave.");
    assert_false(game.get_state().board[cursor->name].prevent_placement(disease::yellow), "Quarantine specialist no longer prevents prevent yellow cubes in the neighbors of the city they leave.");
  }

  // The destination is protected
  assert_true(game.get_state().board[destination].prevent_placement(disease::black), "Quarantine specialist should prevent black cubes in the city they occupy.");
  assert_true(game.get_state().board[destination].prevent_placement(disease::blue), "Quarantine specialist should prevent blue cubes in the city they occupy.");
  assert_true(game.get_state().board[destination].prevent_placement(disease::red), "Quarantine specialist should prevent red cubes in the city they occupy.");
  assert_true(game.get_state().board[destination].prevent_placement(disease::yellow), "Quarantine specialist should prevent yellow cubes in the city they occupy.");

  city::City destination_city = game.get_state().cities[destination];
  for (std::vector<city::City>::iterator cursor = destination_city.neighbors.begin(); cursor != destination_city.neighbors.end(); cursor++) {
    assert_true(game.get_state().board[cursor->name].prevent_placement(disease::black), "Quarantine specialist should prevent black cubes in all neighboring cities.");
    assert_true(game.get_state().board[cursor->name].prevent_placement(disease::blue), "Quarantine specialist should prevent blue cubes in all neighboring cities.");
    assert_true(game.get_state().board[cursor->name].prevent_placement(disease::red), "Quarantine specialist should prevent red cubes in all neighboring cities.");
    assert_true(game.get_state().board[cursor->name].prevent_placement(disease::yellow), "Quarantine specialist should prevent yellow cubes in all neighboring cities.");
  }
}

TEST(charter_flight_requires_card) {
  Game game;
  game.setup();
  GameState game_state = game.get_state();

  card::Hand first_player_hand = game_state.players[0].hand;

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
    game.charter_flight(game_state.players[player_index].role, "Istanbul");
  } catch (std::invalid_argument e) {
    assert_equal<std::string>(e.what(), "This player doesn't have this card.");
    exception_thrown = true;
  }
  assert_true(exception_thrown, "Attempting an illegal charter flight should throw an exception.");
}

TEST(shuttle) {
  Game game;
  game.setup();

  std::string destination = "Istanbul";
  game.place_research_facility(destination);

  game.shuttle(game.get_state().players[0].role, destination);

  GameState game_state = game.get_state();
  assert_equal(game_state.player_locations[game_state.players[0].role], destination);
}

TEST(shuttle_requires_research_facility) {
  Game game;
  game.setup();
  GameState game_state = game.get_state();

  bool exception_thrown = false;
  try {
    game.shuttle(game_state.players[0].role, "Istanbul");
  } catch(std::invalid_argument e) {
    exception_thrown = true;
  }
  assert_true(exception_thrown, "Players should not be able to shuttle without a research facility.");
}

TEST(dispatcher_direct_flight) {
  Game game;
  game.setup();

  game.add_role(player::dispatcher);
  game.add_card(player::dispatcher, card::Card("Istanbul", card::player));
  GameState game_state = game.get_state();

  player::Role other_role = game_state.players[0].role == player::dispatcher ? game_state.players[1].role : game_state.players[0].role;
  game.drive(other_role, game.get_state().cities[CDC_LOCATION].neighbors[0].name);

  game.dispatcher_direct_flight(other_role, "Istanbul");

  assert_equal<std::string>(game.get_state().player_locations[other_role], "Istanbul");
}

TEST(dispatcher_direct_flight_requires_card) {
  Game game;
  game.setup();

  game.add_role(player::dispatcher);
  try {
    game.remove_player_card(player::dispatcher, "Istanbul");
  } catch(std::invalid_argument) {
    // Ignore this failure. This test requires that the dispatcher doesn't have this card. This may have been the case before.
  }
  GameState game_state = game.get_state();

  player::Role other_role = game_state.players[0].role == player::dispatcher ? game_state.players[1].role : game_state.players[0].role;

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
  Game game;
  game.setup();
  game.add_role(player::dispatcher);
  game.add_card(player::dispatcher, card::Card(CDC_LOCATION, card::player));
  GameState game_state = game.get_state();

  player::Role other_role = game_state.players[0].role == player::dispatcher ? game_state.players[1].role : game_state.players[0].role;

  game.dispatcher_charter_flight(other_role, "Istanbul");

  assert_equal<std::string>(game.get_state().player_locations[other_role], "Istanbul");
}

TEST(dispatcher_charter_flight_requires_card) {
  Game game;
  game.setup();
  game.add_role(player::dispatcher);
  try {
    game.remove_player_card(player::dispatcher, CDC_LOCATION);
  } catch(std::invalid_argument) {
    // Ignore this failure. This test requires that the dispatcher doesn't have this card. This may have been the case before.
  }
  GameState game_state = game.get_state();

  player::Role other_role = game_state.players[0].role == player::dispatcher ? game_state.players[1].role : game_state.players[0].role;

  bool exception_thrown = false;
  try {
    game.dispatcher_charter_flight(other_role, "Istanbul");
  } catch (std::invalid_argument e) {
    assert_equal<std::string>(e.what(), "This player doesn't have this card.");
    exception_thrown = true;
  }
  assert_true(exception_thrown, "Attempting an illegal dispatcher charter flight should throw an exception.");
}

TEST(dispatcher_conference) {
  Game game;
  game.setup(easy, 3);
  game.add_role(player::dispatcher);
  GameState game_state = game.get_state();
  player::Role other_roles[2];
  int i, adjust = 0;
  for (i = 0; i < 3; i++) {
    if (game_state.players[i].role == player::dispatcher) {
      adjust = -1;
    } else {
      other_roles[i + adjust] = game_state.players[i].role;
    }
  }

  game.add_card(other_roles[1], card::Card("Istanbul", card::player));
  game.direct_flight(other_roles[1], "Istanbul");

  game.dispatcher_conference(other_roles[0], other_roles[1]);
  assert_equal<std::string>(game.get_state().player_locations[other_roles[0]], "Istanbul");
}

TEST(treat) {
  Game game;
  game.setup();
  GameState game_state = game.get_state();

  // Any game will have at least one role that isn't the medic. Given a scenario where this player has the CDC location card. This enables them to charter a flight anywhere.
  player::Role other_role = game_state.players[0].role == player::medic ? game_state.players[1].role : game_state.players[0].role;
  game.add_card(other_role, card::Card(CDC_LOCATION, card::player));

  game_state = game.get_state();
  std::string city_with_three;
  for (std::map<std::string, city::City>::iterator cursor = game_state.cities.begin(); cursor != game_state.cities.end(); cursor++) {
    if (game.get_state().board[cursor->first].disease_count[cursor->second.color] == 3) {
      // Given a city with three disease markers.
      city_with_three = cursor->first;
      break;
    }
  }

  // Have the non-medic fly to this city and treat the disease.
  game.charter_flight(other_role, city_with_three);
  game.treat(other_role, game.get_state().cities[city_with_three].color);

  // Then there should be two disease markers left.
  assert_equal(game.get_state().board[city_with_three].disease_count[game.get_state().cities[city_with_three].color], 2);
}

TEST(treat_as_medic) {
  Game game;
  game.setup();

  // Given a game with a medic where the medic has the CDC location card. This enables the medic to charter a flight anywhere.
  game.add_role(player::medic);
  game.add_card(player::medic, card::Card(CDC_LOCATION, card::player));

  GameState game_state = game.get_state();
  std::string city_with_three;
  for (std::map<std::string, city::City>::iterator cursor = game_state.cities.begin(); cursor != game_state.cities.end(); cursor++) {
    if (game.get_state().board[cursor->first].disease_count[cursor->second.color] == 3) {
      // Given a city with three disease markers.
      city_with_three = cursor->first;
      break;
    }
  }

  // Have the medic fly to this city and treat the disease.
  game.charter_flight(player::medic, city_with_three);
  game.treat(player::medic, game.get_state().cities[city_with_three].color);

  // Then there should be no disease markers left.
  assert_equal(game.get_state().board[city_with_three].disease_count[game.get_state().cities[city_with_three].color], 0);
}

TEST(share) {
  Game game;
  game.setup();
  GameState game_state = game.get_state();
  
  // There will be a player that isn't the researcher. Get their role and one other (the other player may or may not be the researcher).
  int non_researcher_index = game_state.players[0].role == player::researcher;
  player::Role non_researcher = game_state.players[non_researcher_index].role, other_role = game_state.players[!non_researcher_index].role;

  // Ensure that the non-researcher has the card of the starting location and that the other player doesn't have this card.
  game.add_card(non_researcher, card::Card(CDC_LOCATION, card::player));
  try {
    game.remove_player_card(other_role, CDC_LOCATION);
  } catch(std::invalid_argument) {} // Ignore the exception if the player doesn't have this card already.

  // Share the CDC_LOCATION card.
  game.share(non_researcher, other_role);

  card::Hand other_player_hand = game.get_player(other_role).hand,
    non_researcher_hand = game.get_player(non_researcher).hand;

  bool other_role_has_card = false;
  for (std::vector<card::Card>::iterator cursor = other_player_hand.contents.begin(); cursor != other_player_hand.contents.end(); cursor++) {
    if (cursor->name == CDC_LOCATION) {
      other_role_has_card = true;
      break;
    }
  }
  assert_true(other_role_has_card, "The card should be in the other player's hand.");

  bool non_researcher_has_card = false;
  for (std::vector<card::Card>::iterator cursor = non_researcher_hand.contents.begin(); cursor != non_researcher_hand.contents.end(); cursor++) {
    if (cursor->name == CDC_LOCATION) {
      non_researcher_has_card = true;
      break;
    }
  }
  assert_false(non_researcher_has_card, "The card should be removed from the non-researcher's hand.");
}

TEST(share_requires_card_of_city) {
  Game game;
  game.setup();
  GameState game_state = game.get_state();
  
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
  Game game;
  game.setup();
  // Ensure that the player at index 0 has the starting location card.
  game.add_card(game.get_state().players[0].role, card::Card(CDC_LOCATION, card::player));
  // Move the player at index 1 elsewhere.
  game.drive(game.get_state().players[1].role, game.get_state().cities[CDC_LOCATION].neighbors[0].name);

  // Attempt to share the CDC_LOCATION card.
  bool exception_thrown = false;
  try {
    game.share(game.get_state().players[0].role, game.get_state().players[1].role);
  } catch(std::invalid_argument e) {
    exception_thrown = true;
    assert_equal<std::string>(e.what(), "Players must be in the same city to share cards.");
  }

  // Then
  assert_true(exception_thrown, "Sharing a card should be illegal if the players aren't in the same city.");
}

TEST(share_as_researcher) {
  Game game;
  game.setup();
  game.add_role(player::researcher);
  std::string non_starting_location = "Istanbul";
  
  // There will be a player that isn't the researcher. Get their role and one other (the other player may or may not be the researcher).
  player::Role non_researcher = game.get_state().players[0].role == player::researcher ? game.get_state().players[1].role : game.get_state().players[0].role;

  // Ensure that the researcher has some card other than the starting location and that the non-researcher doesn't have this card.
  game.add_card(player::researcher, card::Card(non_starting_location, card::player));
  try {
    game.remove_player_card(non_researcher, non_starting_location);
  } catch(std::invalid_argument) {} // Ignore the exception if the player doesn't have this card already.

  // Share the non-starting location card.
  game.researcher_share(non_starting_location, non_researcher);

  // Confirm that the card has changed hands
  card::Hand researcher_hand = game.get_player(player::researcher).hand,
    non_researcher_hand = game.get_player(non_researcher).hand;

  bool researcher_has_card = false;
  for (std::vector<card::Card>::iterator cursor = researcher_hand.contents.begin(); cursor != researcher_hand.contents.end(); cursor++) {
    if (cursor->name == non_starting_location) {
      researcher_has_card = true;
      break;
    }
  }
  assert_false(researcher_has_card, "The card should be removed from the researcher's hand.");

  bool non_researcher_has_card = false;
  for (std::vector<card::Card>::iterator cursor = non_researcher_hand.contents.begin(); cursor != non_researcher_hand.contents.end(); cursor++) {
    if (cursor->name == non_starting_location) {
      non_researcher_has_card = true;
      break;
    }
  }
  assert_true(non_researcher_has_card, "The card should be added to the non-researcher's hand.");
}

TEST(share_as_researcher_requires_same_city) {
  Game game;
  game.setup();
  game.add_role(player::researcher);
  // Ensure that the researcher has the starting location card.
  game.add_card(player::researcher, card::Card(CDC_LOCATION, card::player));
  // Determine the other player's role. Move that player elsewhere.
  player::Role non_researcher = game.get_state().players[0].role == player::researcher ? game.get_state().players[1].role : game.get_state().players[0].role;
  game.drive(non_researcher, game.get_state().cities[CDC_LOCATION].neighbors[0].name);

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
  Game game;
  game.setup();
  
  // Suppose that player 0 has four of these cards.
  std::string to_discard[] {"Algeirs", "Baghdad", "Cairo", "Chennai", "Delhi"};
  for (int i = 0; i < 4; i++) {
    game.add_card(game.get_state().players[0].role, card::Card(to_discard[i], card::player));
  }
  try {
    game.remove_player_card(game.get_state().players[0].role, to_discard[4]);
  } catch(std::invalid_argument) {} // Ignore exception if card already missing.
  std::vector<card::Card> cards_in_hand = game.get_state().players[0].hand.contents;
  
  bool exception_thrown = false;
  try {
    game.cure(game.get_state().players[0].role, to_discard);
  } catch(std::invalid_argument e) {
    exception_thrown = true;
    assert_equal<std::string>(e.what(), "This player doesn't have this card.");
  }

  assert_true(exception_thrown, "Attempting to cure with cards that aren't in your hand should be illegal.");
  // The player's hand shouldn't be different after a failure.
  assert_equal<int>(game.get_state().players[0].hand.contents.size(), cards_in_hand.size());
  for (int i = 0; i < cards_in_hand.size(); i++) {
    assert_equal<std::string>(game.get_state().players[0].hand.contents[i].name, cards_in_hand[i].name);
  }
}

TEST(cure_requires_five_cards_of_the_same_color) {
  Game game;
  game.setup();
  
  // Suppose that player 0 has all of these cards.
  std::string to_discard[] {"Algeirs", "Baghdad", "Cairo", "Chennai", "Atlanta"};
  for (int i = 0; i < 5; i++) {
    game.add_card(game.get_state().players[0].role, card::Card(to_discard[i], card::player));
  }
  std::vector<card::Card> cards_in_hand = game.get_state().players[0].hand.contents;
  
  bool exception_thrown = false;
  try {
    game.cure(game.get_state().players[0].role, to_discard);
  } catch(std::invalid_argument e) {
    exception_thrown = true;
    assert_equal<std::string>(e.what(), "Curing a disease requires that all discarded cards share a color.");
  }

  assert_true(exception_thrown, "Attempting to cure with cards that aren't the same color should be illegal.");
  // The player's hand shouldn't be different after a failure.
  assert_equal<int>(game.get_state().players[0].hand.contents.size(), cards_in_hand.size());
  for (int i = 0; i < cards_in_hand.size(); i++) {
    assert_equal<std::string>(game.get_state().players[0].hand.contents[i].name, cards_in_hand[i].name);
  }
}

TEST(cure_requires_research_facility) {
  Game game;
  game.setup();
  // Move player 0 away from the starting location.
  game.drive(game.get_state().players[0].role, game.get_state().cities[CDC_LOCATION].neighbors[0].name);
  
  // Suppose that player 0 has all of these cards.
  std::string to_discard[] {"Algeirs", "Baghdad", "Cairo", "Chennai", "Delhi"};
  for (int i = 0; i < 5; i++) {
    game.add_card(game.get_state().players[0].role, card::Card(to_discard[i], card::player));
  }
  std::vector<card::Card> cards_in_hand = game.get_state().players[0].hand.contents;
  
  bool exception_thrown = false;
  try {
    game.cure(game.get_state().players[0].role, to_discard);
  } catch(std::invalid_argument e) {
    exception_thrown = true;
    assert_equal<std::string>(e.what(), "Curing a disease may only occur in a city with a research facility.");
  }

  assert_true(exception_thrown, "Attempting to cure without a research facility should be illegal.");
  // The player's hand shouldn't be different after a failure.
  assert_equal<int>(game.get_state().players[0].hand.contents.size(), cards_in_hand.size());
  for (int i = 0; i < cards_in_hand.size(); i++) {
    assert_equal<std::string>(game.get_state().players[0].hand.contents[i].name, cards_in_hand[i].name);
  }
}

TEST(cure) {
  Game game;
  game.setup();
  
  // Suppose that player 0 has all of these cards.
  std::string to_discard[] {"Algeirs", "Baghdad", "Cairo", "Chennai", "Delhi"};
  for (int i = 0; i < 5; i++) {
    game.add_card(game.get_state().players[0].role, card::Card(to_discard[i], card::player));
  }
  std::vector<card::Card> cards_in_hand = game.get_state().players[0].hand.contents;
  
  // Cure the disease
  game.cure(game.get_state().players[0].role, to_discard);

  // The player's hand should have discarded the cards.
  assert_equal<int>(game.get_state().players[0].hand.contents.size(), cards_in_hand.size() - 5);
  bool discarded;
  for (int i = 0; i < cards_in_hand.size(); i++) {
    discarded = false;
    for (int j = 0; j < 5; j++) {
      if (cards_in_hand[i].name == to_discard[j]) {
        discarded = true;
        std::string message = "The player's hand shouldn't contain " + to_discard[j];
        assert_false(card::contains(game.get_state().players[0].hand, to_discard[j]), message.c_str());
        break;
      }
    }
    if (!discarded) {
      std::string message = "The player's hand should still contain " + cards_in_hand[i].name;
      assert_true(card::contains(game.get_state().players[0].hand, cards_in_hand[i].name), message.c_str());
    }
  }
  assert_true(game.get_state().diseases[disease::black].cured, "The disease should be cured.");
}

TEST(cure_as_scientist_requires_four_cards_in_hand) {
  Game game;
  game.setup();
  game.add_role(player::scientist);
  
  // Suppose that the scientist has three of these cards.
  std::string to_discard[] {"Algeirs", "Baghdad", "Cairo", "Chennai"};
  for (int i = 0; i < 3; i++) {
    game.add_card(player::scientist, card::Card(to_discard[i], card::player));
  }
  try {
    game.remove_player_card(player::scientist, to_discard[3]);
  } catch(std::invalid_argument) {}
  std::vector<card::Card> cards_in_hand = game.get_player(player::scientist).hand.contents;
  
  bool exception_thrown = false;
  try {
    game.scientist_cure(to_discard);
  } catch(std::invalid_argument e) {
    exception_thrown = true;
    assert_equal<std::string>(e.what(), "This player doesn't have this card.");
  }

  assert_true(exception_thrown, "Attempting to cure with cards that aren't in your hand should be illegal.");
  // The scientist's hand shouldn't be different after a failure.
  card::Hand hand_after_failure = game.get_player(player::scientist).hand;
  assert_equal<int>(hand_after_failure.contents.size(), cards_in_hand.size());
  for (int i = 0; i < cards_in_hand.size(); i++) {
    assert_equal<std::string>(hand_after_failure.contents[i].name, cards_in_hand[i].name);
  }
}

TEST(cure_as_scientist_requires_four_cards_of_the_same_color) {
  Game game;
  game.setup();
  game.add_role(player::scientist);
  
  // Suppose that the scientists has all of these cards.
  std::string to_discard[] {"Algeirs", "Baghdad", "Cairo", "Atlanta"};
  for (int i = 0; i < 4; i++) {
    game.add_card(player::scientist, card::Card(to_discard[i], card::player));
  }
  std::vector<card::Card> cards_in_hand = game.get_player(player::scientist).hand.contents;
  
  bool exception_thrown = false;
  try {
    game.scientist_cure(to_discard);
  } catch(std::invalid_argument e) {
    exception_thrown = true;
    assert_equal<std::string>(e.what(), "Curing a disease requires that all discarded cards share a color.");
  }

  assert_true(exception_thrown, "Attempting to cure with cards that aren't the same color should be illegal.");
  // The scientist's hand shouldn't be different after a failure.
  card::Hand hand_after_failure = game.get_player(player::scientist).hand;
  assert_equal<int>(hand_after_failure.contents.size(), cards_in_hand.size());
  for (int i = 0; i < cards_in_hand.size(); i++) {
    assert_equal<std::string>(hand_after_failure.contents[i].name, cards_in_hand[i].name);
  }
}

TEST(cure_as_scientist_requires_research_facility) {
  Game game;
  game.setup();
  game.add_role(player::scientist);
  // Move the scientist away from the starting location.
  game.drive(player::scientist, game.get_state().cities[CDC_LOCATION].neighbors[0].name);
  
  // Suppose that the scientists has all of these cards.
  std::string to_discard[] {"Algeirs", "Baghdad", "Cairo", "Chennai"};
  for (int i = 0; i < 4; i++) {
    game.add_card(player::scientist, card::Card(to_discard[i], card::player));
  }
  std::vector<card::Card> cards_in_hand = game.get_player(player::scientist).hand.contents;
  
  bool exception_thrown = false;
  try {
    game.scientist_cure(to_discard);
  } catch(std::invalid_argument e) {
    exception_thrown = true;
    assert_equal<std::string>(e.what(), "Curing a disease may only occur in a city with a research facility.");
  }

  assert_true(exception_thrown, "Attempting to cure without a research facility should be illegal.");
  // The scientist's hand shouldn't be different after a failure.
  card::Hand hand_after_failure = game.get_player(player::scientist).hand;
  assert_equal<int>(hand_after_failure.contents.size(), cards_in_hand.size());
  for (int i = 0; i < cards_in_hand.size(); i++) {
    assert_equal<std::string>(hand_after_failure.contents[i].name, cards_in_hand[i].name);
  }
}

TEST(cure_as_scientist) {
  Game game;
  game.setup();
  game.add_role(player::scientist);
  
  // Suppose that the scientist has all of these cards.
  std::string to_discard[] {"Algeirs", "Baghdad", "Cairo", "Chennai"};
  for (int i = 0; i < 4; i++) {
    game.add_card(player::scientist, card::Card(to_discard[i], card::player));
  }
  std::vector<card::Card> cards_in_hand = game.get_player(player::scientist).hand.contents;
  
  // Cure the disease
  game.scientist_cure(to_discard);

  // The scientist's hand should have discarded the cards.
  assert_equal<int>(game.get_player(player::scientist).hand.contents.size(), cards_in_hand.size() - 4);
  bool discarded;
  for (int i = 0; i < cards_in_hand.size(); i++) {
    discarded = false;
    for (int j = 0; j < 4; j++) {
      if (cards_in_hand[i].name == to_discard[j]) {
        discarded = true;
        std::string message = "The scientist's hand shouldn't contain " + to_discard[j];
        assert_false(card::contains(game.get_player(player::scientist).hand, to_discard[j]), message.c_str());
        break;
      }
    }
    if (!discarded) {
      std::string message = "The scientist's hand should still contain " + cards_in_hand[i].name;
      assert_true(card::contains(game.get_player(player::scientist).hand, cards_in_hand[i].name), message.c_str());
    }
  }
  assert_true(game.get_state().diseases[disease::black].cured, "The disease should be cured.");
}
TEST(reclaim_requires_event_type) {
  Game game;
  game.setup();
  game.add_role(player::contingency_planner);
  game.discard(card::Card(CDC_LOCATION, card::player));
  
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
  Game game;
  game.setup();
  game.add_role(player::contingency_planner);
  
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
  Game game;
  game.setup();
  game.add_role(player::contingency_planner);
  game.discard(card::Card(ONE_QUIET_NIGHT, card::player, card::one_quiet_night));

  assert_false(card::contains(game.get_state().contingency_card, ONE_QUIET_NIGHT), "The event card should not be on the contingency planner card initially.");
  assert_equal<int>(game.get_state().player_deck.get_discard_contents().size(), 1);


  game.reclaim(ONE_QUIET_NIGHT);
  assert_true(card::contains(game.get_state().contingency_card, ONE_QUIET_NIGHT), "The event card should be on the contingency planner card after reclaiming it.");
  assert_equal<int>(game.get_state().player_deck.get_discard_contents().size(), 0);
}
TEST(reclaim_limited_to_one) {
  Game game;
  game.setup();
  game.add_role(player::contingency_planner);
  game.discard(card::Card(ONE_QUIET_NIGHT, card::player, card::one_quiet_night));
  game.discard(card::Card(RESILIENT_POPULATION, card::player, card::resilient_population));

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
  Game game;
  game.setup();
  game.remove_role(player::contingency_planner);
  game.discard(card::Card(ONE_QUIET_NIGHT, card::player, card::one_quiet_night));
  
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
  Game game;
  game.setup();
  game.remove_role(player::operations_expert);
  
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
  Game game;
  game.setup();
  game.add_role(player::operations_expert);
  std::string to_discard = "Madrid";
  try {
  game.remove_player_card(player::operations_expert, to_discard);
  } catch(std::invalid_argument) {} // Ignore exception if card is missing.
  
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
  Game game;
  game.setup();
  game.add_role(player::operations_expert);
  std::string to_discard = "Madrid";
  game.add_card(player::operations_expert, card::Card(to_discard, card::player));

  game.drive(player::operations_expert, game.get_state().cities[CDC_LOCATION].neighbors[0].name);
  
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
  Game game;
  game.setup();
  game.add_role(player::operations_expert);
  std::string destination = "Istanbul", to_discard = "Madrid";
  game.add_card(player::operations_expert, card::Card(to_discard, card::player));
  
  // Take the company plane
  game.company_plane(destination, to_discard);

  assert_false(card::contains(game.get_player(player::operations_expert).hand, to_discard), "The discarded card should be removed from the Operations Expert's hand.");
  assert_equal(game.get_state().player_deck.get_discard_contents().back().name, to_discard);
  assert_equal(game.get_state().player_locations[player::operations_expert], destination);
}

TEST(epidemic) {
  Game game;
  game.setup();
  game.remove_role(player::quarantine_specialist); // This role may prevent disease placement. Remove it for testing.

  GameState game_state_before = game.get_state();

  card::Card bottom_card = game_state_before.infection_deck.reveal(-1);
  std::vector<card::Card> infection_discard_before = game_state_before.infection_deck.get_discard_contents();
  disease::DiseaseColor color = game_state_before.cities[bottom_card.name].color;

  assert_equal(game_state_before.board[bottom_card.name].disease_count[color], 0);
  assert_equal(game_state_before.infection_rate_level, 0);


  assert_equal<int>(infection_discard_before.size(), 9);
  assert_equal<int>(game_state_before.infection_deck.remaining(), game_state_before.cities.size() - 9);

  assert_false(game.epidemic(), "The first epidemic of the game shouldn't result in a loss.");

  GameState game_state_after = game.get_state();
  assert_equal(game_state_after.board[bottom_card.name].disease_count[color], 3);
  assert_equal(game_state_after.diseases[color].reserve, game_state_before.diseases[color].reserve - 3);
  assert_equal(game_state_after.infection_rate_level, 1);

  assert_equal<int>(game_state_after.infection_deck.get_discard_contents().size(), 0);

  struct CardCompare {
    bool operator() (const card::Card& lhs, const card::Card& rhs) const {
      return lhs.name < rhs.name;
    }
  };
  std::set<card::Card, CardCompare> expected_top_ten(infection_discard_before.begin(), infection_discard_before.end());
  expected_top_ten.insert(bottom_card);
  assert_equal<int>(game_state_after.infection_deck.remaining(), game_state_after.cities.size());
  std::set<card::Card>::iterator cursor;
  for (int i = 0; i < 10; i++) {
    cursor = expected_top_ten.find(game_state_after.infection_deck.reveal(i));
    assert_false(cursor == expected_top_ten.end(), "All of the top ten infection cards should come from the discard.");
    expected_top_ten.erase(cursor);
  }
}
