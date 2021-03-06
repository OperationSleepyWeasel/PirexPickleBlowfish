#include <gmock/gmock.h>
using ::testing::Eq;
#include <gtest/gtest.h>
using ::testing::Test;

#include <cstdlib>
#include <vector>
#include "logic/ModuleToken.hpp"
#include "logic/UnitToken.hpp"
#include "logic/Upgrader.hpp"
#include "logic/AddAttributeUpgrader.hpp"
#include "logic/ChangeArmyUpgrader.hpp"
#include "logic/ChangeAttributeUpgrader.hpp"

class ModuleTokenTest : public Test
{
protected:
  ModuleTokenTest() {
    initiative = new Attribute("initiative", 2);
    mainUnitAttributes = new Attributes;
    mainUnitAttributes -> addAttribute(INITIATIVE, initiative);
    melee = new Attribute("melee", 1);
    northSideAttributes = new Attributes;
    northSideAttributes  ->  addAttribute(MELEE, melee);
    unit = new UnitToken(HEGEMONY, "UniversalSoldier", mainUnitAttributes);
    unit -> setEdgeAttributes(NORTH, northSideAttributes);
    toughness = new Attribute("toughness", 1);
    mainModuleAttributes = new Attributes;
    mainModuleAttributes -> addAttribute(TOUGHNESS, toughness);
    activeEdges.push_back(NORTH);
    activeEdges.push_back(SOUTH);
  }
  ~ModuleTokenTest() {
    delete unit;
  }
  void SetUp() {}
  void TearDown() {}

  Attribute* initiative;
  Attributes* mainUnitAttributes;
  Attribute* melee;
  Attributes* northSideAttributes;
  UnitToken* unit;
  Attribute* toughness;
  Attributes* mainModuleAttributes;
  std::vector<Side> activeEdges;
};

TEST_F(ModuleTokenTest, shouldNotAffectEnemiesOnDefault) {
  Upgrader* upgrader = new ChangeAttributeUpgrader(new ModuleToken(HEGEMONY, "Officer", mainModuleAttributes, activeEdges), MELEE, 1);
  ASSERT_FALSE(upgrader  ->  isAffectingEnemies());
}

TEST_F(ModuleTokenTest, shouldAffectEnemies) {
  Upgrader* upgrader = new ChangeAttributeUpgrader(new ModuleToken(HEGEMONY, "Officer", mainModuleAttributes, activeEdges), MELEE, 1, true);
  ASSERT_TRUE(upgrader  ->  isAffectingEnemies());
}

TEST_F(ModuleTokenTest, shouldAddBoardToken) {
  ModuleToken* module = new ModuleToken(HEGEMONY, "Officer", mainModuleAttributes, activeEdges);
  EXPECT_TRUE(module -> boardTokens.empty());
  module -> addBoardToken(unit);
  EXPECT_EQ(1, module -> boardTokens.size());
  EXPECT_EQ("UniversalSoldier", module -> boardTokens[0] -> getName());
  delete module;
}

TEST_F(ModuleTokenTest, shouldRemoveBoardToken) {
  ModuleToken* module = new ModuleToken(HEGEMONY, "Officer", mainModuleAttributes, activeEdges);
  UnitToken* soldier = new UnitToken(HEGEMONY, "Soldier", mainUnitAttributes);
  UnitToken* anotherSoldier = new UnitToken(HEGEMONY, "UniversalSoldier", mainUnitAttributes);
  ASSERT_TRUE(module -> boardTokens.empty());
  module -> addBoardToken(unit);
  module -> addBoardToken(soldier);
  module -> addBoardToken(anotherSoldier);
  EXPECT_EQ(3, module -> boardTokens.size());
  EXPECT_EQ("Soldier", module -> boardTokens[1] -> getName());
  module -> removeBoardToken(soldier);
  EXPECT_EQ(2, module -> boardTokens.size());
  EXPECT_EQ("UniversalSoldier", module -> boardTokens[1] -> getName());
  module -> removeBoardToken(anotherSoldier);
  EXPECT_EQ(1, module -> boardTokens.size());
  EXPECT_EQ("UniversalSoldier", module -> boardTokens[0] -> getName());
  EXPECT_EQ(unit, module -> boardTokens[0]);
  delete module;
}

TEST_F(ModuleTokenTest, shouldUpgradeBaseAttribute) {
  int oldInitiativeValue = unit -> getAttribute(INITIATIVE) -> getValue();
  ASSERT_EQ(2, oldInitiativeValue);
  Module* ranger = new ChangeAttributeUpgrader(new ModuleToken(HEGEMONY, "Ranger", mainModuleAttributes, activeEdges), INITIATIVE, 1);
  ranger -> addBoardToken(unit);
  int newInitiativeValue = unit -> getAttribute(INITIATIVE) -> getValue();
  ASSERT_EQ(3, newInitiativeValue);
}

TEST_F(ModuleTokenTest, shouldDowngradeAttributeOnRemove) {
  Module* ranger = new ChangeAttributeUpgrader(new ModuleToken(HEGEMONY, "Ranger", mainModuleAttributes, activeEdges), INITIATIVE, 1);
  ranger -> addBoardToken(unit);
  int attributeValue = unit -> getAttribute(INITIATIVE) -> getValue();
  EXPECT_EQ(3, attributeValue);
  ranger -> removeBoardToken(unit);
  attributeValue = unit -> getAttribute(INITIATIVE) -> getValue();
  EXPECT_EQ(2, attributeValue);
}

TEST_F(ModuleTokenTest, shouldUpgradeEdgeAttribute) {
  Module* officer = new ChangeAttributeUpgrader(new ModuleToken(HEGEMONY, "Officer", mainModuleAttributes, activeEdges), MELEE, 1);
  Attribute* melee = unit -> getEdgeAttributes(NORTH) -> getAttribute(MELEE);
  int oldMeleeValue = melee -> getValue();
  ASSERT_EQ(1, oldMeleeValue);
  officer -> addBoardToken(unit);
  int newMeleeValue = melee -> getValue();
  ASSERT_EQ(2, newMeleeValue);
}

TEST_F(ModuleTokenTest, shouldAddAttribute) {
  Module* transport = new AddAttributeUpgrader(new ModuleToken(HEGEMONY, "Transport", mainModuleAttributes, activeEdges), MOBILITY, "mobility");
  Attribute* unitAttribute = unit -> getAttribute(MOBILITY);
  EXPECT_EQ(0, unitAttribute);
  transport -> addBoardToken(unit);
  unitAttribute = unit -> getAttribute(MOBILITY);
  ASSERT_NE((Attribute*) 0, unitAttribute);
  int mobilityValue = unit -> getAttribute(MOBILITY) -> getValue();
  ASSERT_EQ(1, mobilityValue);
}

TEST_F(ModuleTokenTest, shouldDowngradeEnemyAttribute) {
  Attribute* initiative = unit -> getAttribute(INITIATIVE);
  ASSERT_NE((Attribute*) 0, initiative);
  int oldInitiativeValue = initiative -> getValue();
  ASSERT_EQ(2, oldInitiativeValue);
  Module* saboteur = new ChangeAttributeUpgrader(new ModuleToken(OUTPOST, "Saboteur", mainModuleAttributes, activeEdges), INITIATIVE, -1);
  saboteur -> addBoardToken(unit);
  int newInitiativeValue = unit -> getAttribute(INITIATIVE) -> getValue();
  ASSERT_EQ(1, newInitiativeValue);
}

TEST_F(ModuleTokenTest, shouldCaptureEnemyModule) {
  Module* scoper = new ChangeArmyUpgrader(new ModuleToken(OUTPOST, "Scoper", mainModuleAttributes, activeEdges));
  EXPECT_EQ(HEGEMONY, unit -> getArmy());
  scoper -> addBoardToken(unit);
  ASSERT_EQ(OUTPOST, unit -> getArmy());
  scoper -> removeBoardToken(unit);
  EXPECT_EQ(HEGEMONY, unit -> getArmy());
  UnitToken* anotherUnit = new UnitToken(MOLOCH, "Gauss cannon", mainUnitAttributes);
  anotherUnit -> setEdgeAttributes(NORTH, northSideAttributes);
  EXPECT_EQ(MOLOCH, anotherUnit -> getArmy());
  scoper -> addBoardToken(anotherUnit);
  ASSERT_EQ(OUTPOST, anotherUnit -> getArmy());
}

TEST_F(ModuleTokenTest, shouldApplyTwoDifferentUpgradesFromOneModule) {
  Module* boss = new ChangeAttributeUpgrader(new ChangeAttributeUpgrader(new ModuleToken(HEGEMONY, "Boss", mainModuleAttributes, activeEdges), MELEE, 1), INITIATIVE, 2);
  ASSERT_EQ(1, unit -> getEdgeAttributes(NORTH) -> getAttribute(MELEE) -> getValue());
  ASSERT_EQ(2, unit -> getAttribute(INITIATIVE) -> getValue());

  boss -> addBoardToken(unit);

  ASSERT_EQ(2, unit -> getEdgeAttributes(NORTH) -> getAttribute(MELEE) -> getValue());
  ASSERT_EQ(4, unit -> getAttribute(INITIATIVE) -> getValue());
}
