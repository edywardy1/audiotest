audio: audio.o
	g++ -o audio audio.o -std=c++1z -Wconversion -Wall -Werror -Wextra -pedantic
audio.o: audio.cpp
	g++ -c audio.cpp -std=c++1z -Wconversion -Wall -Werror -Wextra -pedantic
