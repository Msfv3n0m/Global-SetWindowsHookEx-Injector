default:
	g++ -I include -shared dllmain.cpp -o test.dll
	g++ main.cpp -o main.exe
	main.exe
clean:
	del main.exe
	del test.dll
