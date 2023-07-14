default:
	g++ -I include -shared dllmain.cpp -o test.dll
	g++ main.cpp -o main.exe
#	del \windows\system32\test.dll
#	xcopy test.dll \windows\system32
	main.exe