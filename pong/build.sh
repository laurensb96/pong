#!/usr/bin/bash

g++ -c main.cpp
g++ main.o -o pong-app -lsfml-graphics -lsfml-window -lsfml-system
