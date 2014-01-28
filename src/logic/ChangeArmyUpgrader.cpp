#include "ChangeArmyUpgrader.hpp"

ChangeArmyUpgrader::ChangeArmyUpgrader(Module* module, bool affectsEnemies)
  : BoardToken( module-> getArmy(), module -> getName(), module -> getAttributes()), Upgrader(module, affectsEnemies) {
}

void ChangeArmyUpgrader::upgrade(BoardToken* token) {
  token -> setArmy(module -> getArmy());
}

void ChangeArmyUpgrader::downgrade(BoardToken* token) {
  token -> resetArmy();
}
