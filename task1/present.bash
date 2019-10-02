#!/usr/bin/env bash

echo "It's a solved task for the job application performed by Oshchepkov Artem (semitro_8@mail.ru)"
sleep 5
echo "That I was supposed to do:"
echo "--"
cat task.txt
echo "--"
sleep 10

echo "Let's build and run the program not using SSE instructions"
sleep 5
cd src
make clean && make && ./fir

sleep 1
echo ""
echo "If all the tests are marked \"PASSED\", it works"
sleep 5
echo "Now let's use SSE!"
sleep 7
echo ""
make clean && make sse && ./fir

sleep 1
echo "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"
echo "I invite you to take a look at report.pdf :)"
