target:
	g++ -c MT23064_Q1.cpp
	g++ -g -o debug1 MT23064_Q1.o
	g++ -O3 -o optimize1 MT23064_Q1.o
	./debug1
	

	g++ -c MT23064_Q2.cpp
	g++ -g -o debug2 MT23064_Q2.o
	g++ -O3 -o optimize2 MT23064_Q2.o
	./debug2


	g++ -c MT23064_Q3.cpp
	g++ -g -o debug3 MT23064_Q3.o
	g++ -O3 -o optimize3 MT23064_Q3.o
	./debug3
	