#include <types/card.hpp>
#include <random>
#include <stdexcept>

namespace gerryfudd::types::card {
  Card::Card(std::string name, CardType type): name{name}, type{type} {}

  Deck::Deck(CardType type): type{type} {}
  void Deck::discard(Card card) {
    if (type != card.type) {
      throw std::invalid_argument("This deck only accepts one type of card.");
    }
    discard_contents.push_back(card);
  }
  int random(int options) {
    std::random_device generator;
    std::uniform_int_distribution<int> distribution(0,options-1);
    return distribution(generator);
  }
  void Deck::shuffle() {
    int i;
    while (discard_contents.size() > 0) {
      i = random(discard_contents.size());
      contents.push_back(discard_contents[i]);
      discard_contents.erase(discard_contents.begin() + i);
    }
  }
  Card Deck::draw() {
    Card result = contents.back();
    contents.pop_back();
    return result;
  }
  Card Deck::reveal(int position) {
    return contents[contents.size() - position];
  }
  int Deck::size() {
    return contents.size() + discard_contents.size();
  }
  int Deck::remaining() {
    return contents.size();
  }
}
