#include <Framework.hpp>
#include <Assertions.inl>
#include <types/city.hpp>

using namespace gerryfudd::test;

TEST(get_name) {
  const char *city_name = "CITY_NAME";
  gerryfudd::types::city::City city(city_name, gerryfudd::types::disease::black, 1000000);

  assert_equal_strings(city.get_name(), city_name);
}

TEST(get_color) {
  gerryfudd::types::city::City city("CITY_NAME", gerryfudd::types::disease::DiseaseColor::blue, 1000000);

  assert_equal(city.get_color(), gerryfudd::types::disease::blue);
}

TEST(get_population) {
  const int population = 1000000;
  gerryfudd::types::city::City city("CITY_NAME", gerryfudd::types::disease::blue, population);

  assert_equal(city.get_population(), population);
}

TEST(attach) {
  gerryfudd::types::city::City cityA("A", gerryfudd::types::disease::black, 10), cityB("B", gerryfudd::types::disease::blue, 20);

  assert_equal(cityA.get_neighbor_count(), 0);
  assert_equal(cityB.get_neighbor_count(), 0);

  attach(&cityA, &cityB);
  assert_equal(cityA.get_neighbor_count(), 1);
  assert_equal(cityB.get_neighbor_count(), 1);
  assert_equal_strings(cityA.get_neighbor(0).get_name(), "B");
  assert_equal_strings(cityB.get_neighbor(0).get_name(), "A");
}

TEST(neighbor_logic) {
  gerryfudd::types::city::City cityA("A", gerryfudd::types::disease::black, 10), 
    cityB("B", gerryfudd::types::disease::blue, 20),
    cityC("C", gerryfudd::types::disease::blue, 15);

  attach(&cityA, &cityB);
  attach(&cityB, &cityC);
  assert_equal(cityA.get_neighbor_count(), 1);
  assert_equal_strings(cityA.get_neighbor(0).get_name(), "B");

  assert_equal(cityB.get_neighbor_count(), 2);
  assert_equal_strings(cityB.get_neighbor(0).get_name(), "A");
  assert_equal_strings(cityB.get_neighbor(1).get_name(), "C");

  assert_equal(cityC.get_neighbor_count(), 1);
  assert_equal_strings(cityC.get_neighbor(0).get_name(), "B");
}
