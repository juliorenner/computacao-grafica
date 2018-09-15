output: main.o mesh.o group.o face.o
	g++ main.o mesh.o group.o face.o -o out.exe -I/usr/local/include/ -I/usr/include/ -I/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/include/c++/v1/ -I/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/lib/clang/9.1.0/include/ -I/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/include/

main.o: main.cpp
	g++ -c main.cpp

mesh.o: Mesh.cpp Mesh.h
	g++ -c Mesh.cpp

group.o: Group.cpp Group.h
	g++ -c Group.cpp

face.o: Face.cpp Face.h
	g++ -c Face.cpp

clean:
	rm output main.o mesh.o group.o face.o