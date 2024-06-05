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
  void Deck::shuffle(int start, int end) {
    std::vector<Card> tempSection;
    auto content_addr = contents.end() - end;
    int temp_i;
    while (tempSection.size() < end - start) {
      tempSection.push_back(*content_addr);
      contents.erase(content_addr);
    }
    while (tempSection.size() > 0) {
      temp_i = random(tempSection.size());
      contents.insert(content_addr, tempSection[temp_i]);
      tempSection.erase(tempSection.begin() + temp_i);
    }
  }
  void Deck::insert(Card card, int position) {
    contents.insert(contents.end() - position, card);
  }

  Card Deck::draw() {
    Card result = contents.back();
    contents.pop_back();
    return result;
  }
  Card Deck::reveal(int position) {
    return contents[contents.size() - position - 1];
  }
  int Deck::size() {
    return contents.size() + discard_contents.size();
  }
  int Deck::remaining() {
    return contents.size();
  }
}
