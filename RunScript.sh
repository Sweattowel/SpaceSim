#! /bin/bash

echo -e "Running compilation of and executing of project"

ProjectTarget="main.cpp"
CompileName="SpaceSim"

ROOT="./Helpers"

SRCS=$(find "$ROOT" -type f -name "*.cpp" -print)

echo -e $SRCS

g++ -o $CompileName $ProjectTarget $SRCS -lX11 -lGL -lpthread -lpng -lstdc++fs -std=c++17

echo -e "Running..."

./$CompileName