#!/usr/bin/env bash

RED='\033[0;31m'
NC='\033[0m' # No Color

echo "It's a solved task N1.4 for the job application performed by Oshchepkov Artem (semitro_8@mail.ru)"
sleep 2
echo "That I was supposed to do:"
echo "--"
cat task.txt
echo "--"
sleep 3
cd src
g++ Sound2D.cpp AudioFile.cpp main.cpp -o sound2d

echo "Let's generate wav-file"
./sound2d 1. 6.14
cp ./sound2d ..
echo "And play it!"
sleep 1
echo -e "${RED}Be careful:${NC} It's going to ${RED}MAKE SOME NOISE${NC}"
sleep 3
echo -e "${RED}IT MAY BE LOUD TYPE ^C if you don't ready!${NC}"
sleep 5
echo 5
sleep 1
echo 4
sleep 1
echo 3
sleep 1
echo 2
sleep 1
echo 1..
sleep 1
aplay result.wav &
echo "If you want to play with parameters, run './sound2d distance speed' right here"
