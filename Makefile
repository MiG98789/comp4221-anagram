ALL: Main

Main:
	g++ -g -std=c++11 get_entropies.cpp -o get_entropies
	g++ -g -std=c++11 dijkstra.cpp -o dijkstra -O3

Clean:
	rm get_entropies
	rm dijkstra
