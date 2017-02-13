CC=g++
CFLAGS=-Iinclude -std=c++11
DEPS =  include/bomb.h \
		include/entity.h \
		include/game_state.h \
		include/json.hpp \
		include/location.h \
		include/player.h \

run: bin/bot.exe
	gameengine/Bomberman.exe -b . bot-reference --pretty

bin/%.o: src/%.cpp $(DEPS)
	g++ -Iinclude -c -o bin/$@ $< -std=c++11

build: bin/bomb.o bin/game_state.o bin/player.o bin/main.o
	g++ -Iinclude bin/bomb.o bin/game_state.o bin/player.o bin/main.o -o bin/bot -std=c++11
