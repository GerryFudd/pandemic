#include <types/card.hpp>
#include <random>
#include <stdexcept>
#include <iostream>

namespace gerryfudd::types::card {
  std::string name_of(CardType card_type) {
    switch (card_type)
    {
    case infect:
      return "Infect";
    case player:
      return "Player";
    default:
      throw std::invalid_argument("This card type doesn't have a name");
    }
  }
  Card::Card(std::string name, CardType type): Card::Card(name, type, true) {}
  Card::Card(std::string name, CardType type, bool city): name{name}, type{type}, city{city} {}


  Hand::Hand(CardType type): type{type} {}
  std::ostream& operator<<(std::ostream& out, const Hand &hand) {
    out << name_of(hand.type);
    out << " hand: ";
    bool middle = false;
    for (auto cursor = hand.contents.rbegin(); cursor != hand.contents.rend(); cursor++) {
      if (middle) {
        out << ", ";
      } else {
        middle = true;
      }
      out << cursor->name;
    }
    return out;
  }

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

  void Deck::clear() {
    contents.clear();
    discard_contents.clear();
  }
}
