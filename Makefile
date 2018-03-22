ALL: Main

Main:
	g++ -g -std=c++11 get_entropies.cpp -o get_entropies
	g++ -g -std=c++11 top5anagram.cpp -o top5anagram -O3

Clean:
	rm get_entropies
	rm top5anagram
