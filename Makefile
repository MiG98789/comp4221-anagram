ALL: Main

Main:
	g++ -std=gnu++11 main.cpp -o top5anagram -O3 -march=native -flto -fno-omit-frame-pointer
