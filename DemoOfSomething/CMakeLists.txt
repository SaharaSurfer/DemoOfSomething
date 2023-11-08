cmake_minimum_required(VERSION 3.1)

project(DemoOfSomething LANGUAGES CXX)

set(SOURCE main.cpp GamePlayHandler.cpp EventHandler.cpp BattleHandler.cpp 
	   Character.cpp CharacterCharacteristics.cpp Inventory.cpp Ability.cpp
	   Interface.cpp Item.cpp Lock.cpp Lockpick.cpp Key.cpp)
set(HEADER GamePlayHandler.h EventHandler.h BattleHandler.h Race.h GameClass.h
	   Character.h CharacterCharacteristics.h Inventory.h Ability.h
	   Interface.h Item.h Lock.h Lockpick.h Key.h json.hpp)

add_executable(DemoOfSomething ${SOURCE} ${HEADER})