prog = main
dll = test
default:
	g++ -static-libstdc++ -static-libgcc -I include -shared dllmain.cpp -o $(dll).dll
	g++ -static-libstdc++ -static-libgcc main.cpp -o $(prog).exe
	$(prog).exe
clean:
	rm $(prog).exe
	rm $(dll).dll
