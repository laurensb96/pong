#!/usr/bin/bash

g++ -c main.cpp pong.cpp
g++ main.o pong.o -o pong-app -lsfml-graphics -lsfml-window -lsfml-system
