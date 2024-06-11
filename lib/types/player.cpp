#include <stdexcept>

#include "types/player.hpp"

namespace gerryfudd::types::player {
  std::string name_of(Role role) {
    switch (role)
    {
    case contingency_planner:
      return CONTINGENCY_PLANNER;
    case dispatcher:
      return DISPATCHER;
    case medic:
      return MEDIC;
    case operations_expert:
      return OPERATIONS_EXPERT;
    case quarantine_specialist:
      return QUARANTINE_SPECIALIST;
    case scientist:
      return SCIENTIST;
    case researcher:
      return RESEARCHER;
    default:
      throw std::invalid_argument("This role doesn't have a name.");
    }
  }
  std::string description_of(Role role) {
    switch (role)
    {
    case contingency_planner:
      return CONTINGENCY_PLANNER_DESCRIPTION;
    case dispatcher:
      return DISPATCHER_DESCRIPTION;
    case medic:
      return MEDIC_DESCRIPTION;
    case operations_expert:
      return OPERATIONS_EXPERT_DESCRIPTION;
    case quarantine_specialist:
      return QUARANTINE_SPECIALIST_DESCRIPTION;
    case scientist:
      return SCIENTIST_DESCRIPTION;
    case researcher:
      return RESEARCHER_DESCRIPTION;
    default:
      throw std::invalid_argument("This role doesn't have a description.");
    }
  }
  std::string name_of(ActionType action) {
    switch (action)
    {
    case drive:
      return DRIVE;
    case direct_flight:
      return DIRECT_FLIGHT;
    case charter_flight:
      return CHARTER_FLIGHT;
    case shuttle:
      return SHUTTLE;
    case build:
      return BUILD;
    case treat:
      return TREAT;
    case share:
      return SHARE;
    case cure:
      return CURE;
    case reclaim:
      return RECLAIM;
    case conference:
      return CONFERENCE;
    case company_plane:
      return COMPANY_PLANE;
    default:
      throw std::invalid_argument("This action doesn't have a name.");
    }
  }
  std::string description_of(ActionType action, Role role) {
    switch (action)
    {
    case drive:
      if (role == dispatcher) {
        return DRIVE_DISPATCHER_DESCRIPTION;
      }
      return DRIVE_DESCRIPTION;
    case direct_flight:
      if (role == dispatcher) {
        return DRIVE_DISPATCHER_DESCRIPTION;
      }
      return DIRECT_FLIGHT_DESCRIPTION;
    case charter_flight:
      if (role == dispatcher) {
        return CHARTER_FLIGHT_DISPATCHER_DESCRIPTION;
      }
      return CHARTER_FLIGHT_DESCRIPTION;
    case shuttle:
      if (role == dispatcher) {
        return SHUTTLE_DISPATCHER_DESCRIPTION;
      }
      return SHUTTLE_DESCRIPTION;
    case build:
      if (role == operations_expert) {
        return BUILD_OPERATIONS_EXPERT_DESCRIPTION;
      }
      return BUILD_DESCRIPTION;
    case treat:
      if (role == medic) {
        return TREAT_MEDIC_DESCRIPTION;
      }
      return TREAT_DESCRIPTION;
    case share:
      if (role == researcher) {
        return SHARE_RESEARCHER_DESCRIPTION;
      }
      return SHARE_DESCRIPTION;
    case cure:
      if (role == scientist) {
        return CURE_SCIENTIST_DESCRIPTION;
      }
      return CURE_DESCRIPTION;
    case reclaim:
      if (role == contingency_planner) {
        return RECLAIM_CONTINGENCY_PLANNER_DESCRIPTION;
      }
    case conference:
      if (role == dispatcher) {
        return CONFERENCE_DISPATCHER_DESCRIPTION;
      }
    case company_plane:
      if (role == operations_expert) {
        return COMPANY_PLANE_OPERATIONS_EXPERT_DESCRIPTION;
      }
    default:
      throw std::invalid_argument("This is an invalid combination of role and action.");
    }
  }
  std::vector<ActionType> get_actions(Role role) {
    std::vector<ActionType> result;
    result.push_back(drive);
    result.push_back(direct_flight);
    result.push_back(charter_flight);
    result.push_back(shuttle);
    result.push_back(build);
    result.push_back(treat);
    result.push_back(share);
    result.push_back(cure);
    switch(role) {
      case contingency_planner:
        result.push_back(reclaim);
        break;
      case dispatcher:
        result.push_back(conference);
        break;
      case operations_expert:
        result.push_back(company_plane);
        break;
      default:
        break;
    }
    return result;
  }
  std::vector<Role> get_roles(int player_count) {
    std::vector<Role> result;
    player::Role first = player::contingency_planner;
    std::vector<Role> remaining;
    remaining.push_back(contingency_planner);
    remaining.push_back(dispatcher);
    remaining.push_back(medic);
    remaining.push_back(operations_expert);
    remaining.push_back(quarantine_specialist);
    remaining.push_back(scientist);
    remaining.push_back(researcher);
    while (remaining.size() < 7) {
      remaining.push_back(*(&first + remaining.size()));
    }
    int i;
    while (result.size() < player_count) {
      i = card::random(remaining.size());
      result.push_back(remaining[i]);
      remaining.erase(remaining.begin() + i);
    }
    return result;
  }

  Player::Player(Role role): role{role}, hand{card::Hand(card::player)} {}
}