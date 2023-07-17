prog = main
dll = test
default:
	g++ -I include -shared dllmain.cpp -o $(dll).dll
	g++ main.cpp -o $(prog).exe
	$(prog).exe
clean:
	rm $(prog).exe
	rm $(dll).dll
