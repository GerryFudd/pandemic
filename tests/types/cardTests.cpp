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
