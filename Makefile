audio: audio.o
	g++ -o audio audio.o -std=c++1z -Wconversion -Wall -Werror -Wextra -pedantic  -lfftw3 -lm
audio.o: audio.cpp
	g++ -c audio.cpp -std=c++1z -Wconversion -Wall -Werror -Wextra -pedantic -lfftw3 -lm
