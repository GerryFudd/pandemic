#include <types/card.hpp>
#include <random>
#include <stdexcept>
#include <iostream>

namespace gerryfudd::types::card {
  std::string name_of(DeckType deck_type) {
    switch (deck_type)
    {
    case infect:
      return "Infect";
    case player:
      return "Player";
    default:
      throw std::invalid_argument("This card type doesn't have a name");
    }
  }
  std::string name_of(CardType card_type) {
    switch(card_type) {
      case one_quiet_night:
        return ONE_QUIET_NIGHT;
      case resilient_population:
        return RESILIENT_POPULATION;
      case government_grant:
        return GOVERNMENT_GRANT;
      case airlift:
        return AIRLIFT;
      case epidemic:
        return EPIDEMIC;
      case city:
        return "City";
      default:
        throw std::invalid_argument("This is not a card type.");
    }
  }
  Card::Card(std::string name, DeckType deck_type): Card::Card(name, deck_type, city) {}
  Card::Card(std::string name, DeckType deck_type, CardType type): name{name}, deck_type{deck_type}, type{type} {}


  Hand::Hand(DeckType deck_type): deck_type{deck_type} {}
  std::ostream& operator<<(std::ostream& out, const Hand &hand) {
    out << name_of(hand.deck_type);
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

  bool contains(Hand hand, std::string card_name) {
    for (std::vector<Card>::iterator cursor = hand.contents.begin(); cursor != hand.contents.end(); cursor++) {
      if (cursor->name == card_name) {
        return true;
      }
    }
    return false;
  }

  Deck::Deck(DeckType deck_type): deck_type{deck_type} {}
  void Deck::discard(Card card) {
    if (deck_type != card.deck_type) {
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
    return draw(0);
  }
  Card Deck::draw(int i) {
    std::vector<card::Card>::iterator cursor = contents.begin();
    int place = 0;
    while ((contents.size() - i - 1) % contents.size() > place) {
      cursor++;
      place++;
    }
    Card result = *cursor;
    contents.erase(cursor);
    return result;
  }
  Card Deck::reveal(int position) {
    return contents[(contents.size() - position - 1) % contents.size()];
  }
  int Deck::size() {
    return contents.size() + discard_contents.size();
  }
  int Deck::remaining() {
    return contents.size();
  }

  std::vector<Card> Deck::get_discard_contents() {
    return discard_contents;
  }
  Card Deck::remove_from_discard(std::string card_name) {
    std::vector<Card>::iterator cursor;
    for (cursor = discard_contents.begin(); cursor != discard_contents.end() && cursor->name != card_name; cursor++) {}
    if (cursor == discard_contents.end()) {
      throw std::invalid_argument("The card " + card_name + " is not in the discard pile.");
    }
    Card result = *cursor;
    discard_contents.erase(cursor);
    return result;
  }
  Card Deck::draw_and_discard() {
    return draw_and_discard(0);
  }
  Card Deck::draw_and_discard(int i) {
    Card result = draw(i);
    discard_contents.push_back(result);
    return result;
  }

  void Deck::clear() {
    contents.clear();
    discard_contents.clear();
  }
}
