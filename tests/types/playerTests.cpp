#include "Framework.hpp"
#include "Assertions.inl"

#include "types/player.hpp"

using namespace gerryfudd::test;
using namespace gerryfudd::types;

TEST(get_roles_by_player_count) {
  std::vector<player::Role> assigned_roles;
  for (int i = 2; i <= 4; i++) {
    assigned_roles = player::get_roles(i);
    assert_equal<int>(assigned_roles.size(), i);
    for (int j = 0; j < i - 1; j++) {
      for (int k = j + 1; k < i; k++) {
        assert_false(assigned_roles[j] == assigned_roles[k], "The assigned roles should be unique.");
      }
    }
  }
}

TEST(get_actions_for_role) {
  std::vector<player::ActionType> actions;
  player::Role begin = player::contingency_planner;
  for (int i = 0; i < 7; i++) {
    player::Role role = *(&begin + i);
    actions = player::get_actions(role);
    assert_equal(actions[0], player::drive);
    assert_equal(actions[1], player::direct_flight);
    assert_equal(actions[2], player::charter_flight);
    assert_equal(actions[3], player::shuttle);
    assert_equal(actions[4], player::build);
    assert_equal(actions[5], player::treat);
    assert_equal(actions[6], player::share);
    assert_equal(actions[7], player::cure);
    switch(role) {
      case player::contingency_planner:
        assert_equal(actions[8], player::reclaim);
        break;
      case player::dispatcher:
        assert_equal(actions[8], player::conference);
        break;
      case player::operations_expert:
        assert_equal(actions[8], player::company_plane);
        break;
      default:
        break;
    }
  }
}
