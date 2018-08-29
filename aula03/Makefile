output: main.o mesh.o group.o face.o
	g++ main.o mesh.o group.o face.o -o output

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