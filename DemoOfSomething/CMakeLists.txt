cmake_minimum_required(VERSION 3.1)

project(DemoOfSomething LANGUAGES CXX)

file(GLOB SOURCES
    header/*.h
    header/*.hpp
    src/*.cpp)

add_executable(DemoOfSomething ${SOURCES})