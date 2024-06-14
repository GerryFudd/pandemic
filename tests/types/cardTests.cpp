#include <Framework.hpp>
#include <Assertions.inl>
#include <types/card.hpp>

using namespace gerryfudd::test;

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

TEST(deck_insert) {
  gerryfudd::types::card::Deck player_deck{gerryfudd::types::card::player};
  player_deck.discard(gerryfudd::types::card::Card("Seattle", gerryfudd::types::card::player));
  player_deck.discard(gerryfudd::types::card::Card("Tacoma", gerryfudd::types::card::player)); 
  player_deck.discard(gerryfudd::types::card::Card("Olympia", gerryfudd::types::card::player)); 
  player_deck.discard(gerryfudd::types::card::Card("Centralia", gerryfudd::types::card::player)); 
  player_deck.discard(gerryfudd::types::card::Card("Astoria", gerryfudd::types::card::player)); 
  player_deck.discard(gerryfudd::types::card::Card("Newberg", gerryfudd::types::card::player)); 
  player_deck.discard(gerryfudd::types::card::Card("Portland", gerryfudd::types::card::player)); 
  player_deck.discard(gerryfudd::types::card::Card("Bend", gerryfudd::types::card::player)); 
  player_deck.discard(gerryfudd::types::card::Card("Gresham", gerryfudd::types::card::player));

  player_deck.shuffle();

  player_deck.insert(gerryfudd::types::card::Card(EPIDEMIC, gerryfudd::types::card::player, gerryfudd::types::card::epidemic), 6);
  assert_equal<std::string>(player_deck.reveal(6).name, EPIDEMIC);

  player_deck.insert(gerryfudd::types::card::Card(EPIDEMIC, gerryfudd::types::card::player, gerryfudd::types::card::epidemic), 3);
  assert_equal<std::string>(player_deck.reveal(3).name, EPIDEMIC);

  player_deck.insert(gerryfudd::types::card::Card(EPIDEMIC, gerryfudd::types::card::player, gerryfudd::types::card::epidemic), 0);
  assert_equal<std::string>(player_deck.reveal(0).name, EPIDEMIC);

  assert_equal(player_deck.remaining(), 12);
}

TEST(deck_shuffle) {
  gerryfudd::types::card::Deck player_deck{gerryfudd::types::card::player};
  player_deck.insert(gerryfudd::types::card::Card("Seattle", gerryfudd::types::card::player), 0);
  player_deck.insert(gerryfudd::types::card::Card("Tacoma", gerryfudd::types::card::player), 0); 
  player_deck.insert(gerryfudd::types::card::Card("Olympia", gerryfudd::types::card::player), 0); 
  player_deck.insert(gerryfudd::types::card::Card(EPIDEMIC, gerryfudd::types::card::player, gerryfudd::types::card::epidemic), 0);
  player_deck.insert(gerryfudd::types::card::Card("Centralia", gerryfudd::types::card::player), 0); 
  player_deck.insert(gerryfudd::types::card::Card("Astoria", gerryfudd::types::card::player), 0); 
  player_deck.insert(gerryfudd::types::card::Card("Newberg", gerryfudd::types::card::player), 0); 
  player_deck.insert(gerryfudd::types::card::Card(EPIDEMIC, gerryfudd::types::card::player, gerryfudd::types::card::epidemic), 0);
  player_deck.insert(gerryfudd::types::card::Card("Portland", gerryfudd::types::card::player), 0); 
  player_deck.insert(gerryfudd::types::card::Card("Bend", gerryfudd::types::card::player), 0); 
  player_deck.insert(gerryfudd::types::card::Card("Gresham", gerryfudd::types::card::player), 0);
  player_deck.insert(gerryfudd::types::card::Card(EPIDEMIC, gerryfudd::types::card::player, gerryfudd::types::card::epidemic), 0);

  player_deck.shuffle(0, 4);
  player_deck.shuffle(4, 8);
  player_deck.shuffle(8, 12);
  bool has_match{false};
  for (int i = 0; i < 4; i++) {
    if (player_deck.reveal(i).name == EPIDEMIC) {
      has_match = true;
      break;
    }
  }
  assert_true(has_match, "One of the first four cards should be an Epidemic");
  has_match = false;
  for (int i = 4; i < 8; i++) {
    if (player_deck.reveal(i).name == EPIDEMIC) {
      has_match = true;
      break;
    }
  }
  assert_true(has_match, "One of the middle four cards should be an Epidemic");
  has_match = false;
  for (int i = 8; i < 12; i++) {
    if (player_deck.reveal(i).name == EPIDEMIC) {
      has_match = true;
      break;
    }
  }
  assert_true(has_match, "One of the last four cards should be an Epidemic");
}

TEST(hand_is_streamable) {
  gerryfudd::types::card::Hand player_hand(gerryfudd::types::card::player);

  player_hand.contents.push_back(gerryfudd::types::card::Card("foo", gerryfudd::types::card::player));
  player_hand.contents.push_back(gerryfudd::types::card::Card("bar", gerryfudd::types::card::player));
  player_hand.contents.push_back(gerryfudd::types::card::Card("baz", gerryfudd::types::card::player));

  std::stringstream captor;
  captor << player_hand;

  assert_equal<std::string>(captor.str(), "Player hand: baz, bar, foo");
}

TEST(clear_deck) {
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

  deck.clear();
  assert_equal(deck.size(), 0);
  assert_equal(deck.remaining(), 0);
}

TEST(get_discard_contents) {
  gerryfudd::types::card::Deck deck(gerryfudd::types::card::infect);

  deck.discard(gerryfudd::types::card::Card("foo", gerryfudd::types::card::infect));
  deck.discard(gerryfudd::types::card::Card("bar", gerryfudd::types::card::infect));
  deck.discard(gerryfudd::types::card::Card("baz", gerryfudd::types::card::infect));

  // This method returns a copy of the contents.
  std::vector<gerryfudd::types::card::Card> discard_copy = deck.get_discard_contents();
  assert_equal<std::string>(discard_copy[0].name, "foo");
  assert_equal<std::string>(discard_copy[1].name, "bar");
  assert_equal<std::string>(discard_copy[2].name, "baz");
  // Modifying this copy doesn't alter the original
  discard_copy.pop_back();
  assert_equal(discard_copy.size(), deck.get_discard_contents().size() - 1);
}
