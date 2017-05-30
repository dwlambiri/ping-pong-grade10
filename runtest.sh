#!/bin/bash
cat testconfig1.txt > testresults.txt; ./d-allegro -v -c d -l 2  testconfig1.txt >> testresults.txt
cat testconfig1.txt >> testresults.txt; ./d-allegro -v -c -d -l 2 testconfig2.txt >> testresults.txt
cat testconfig1.txt >> testresults.txt; ./d-allegro -v -c d -l 2  testconfig3.txt >> testresults.txt