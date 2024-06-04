#include <Framework.hpp>
#include <Assertions.inl>
#include <types/city.hpp>

using namespace gerryfudd::test;

TEST(get_name) {
  std::string city_name = "CITY_NAME";
  gerryfudd::types::city::City city(city_name, gerryfudd::types::disease::black, 1000000);

  assert_equal(city.name, city_name);
}

TEST(get_color) {
  gerryfudd::types::city::City city("CITY_NAME", gerryfudd::types::disease::DiseaseColor::blue, 1000000);

  assert_equal(city.color, gerryfudd::types::disease::blue);
}

TEST(get_population) {
  const int population = 1000000;
  gerryfudd::types::city::City city("CITY_NAME", gerryfudd::types::disease::blue, population);

  assert_equal(city.population, population);
}

TEST(attach) {
  gerryfudd::types::city::City cityA("A", gerryfudd::types::disease::black, 10), cityB("B", gerryfudd::types::disease::blue, 20);

  assert_equal<int>(cityA.neighbors.size(), 0);
  assert_equal<int>(cityB.neighbors.size(), 0);

  attach(&cityA, &cityB);
  assert_equal<int>(cityA.neighbors.size(), 1);
  assert_equal<int>(cityB.neighbors.size(), 1);
  assert_equal<std::string>(cityA.neighbors[0].name, "B");
  assert_equal<std::string>(cityB.neighbors[0].name, "A");
}

TEST(neighbor_logic) {
  gerryfudd::types::city::City cityA("A", gerryfudd::types::disease::black, 10), 
    cityB("B", gerryfudd::types::disease::blue, 20),
    cityC("C", gerryfudd::types::disease::blue, 15);

  attach(&cityA, &cityB);
  attach(&cityB, &cityC);
  assert_equal<int>(cityA.neighbors.size(), 1);
  assert_equal<std::string>(cityA.neighbors[0].name, "B");

  assert_equal<int>(cityB.neighbors.size(), 2);
  assert_equal<std::string>(cityB.neighbors[0].name, "A");
  assert_equal<std::string>(cityB.neighbors[1].name, "C");

  assert_equal<int>(cityC.neighbors.size(), 1);
  assert_equal<std::string>(cityC.neighbors[0].name, "B");
}
