CC=g++
CFLAGS=-Iinclude -std=c++11 -g
DEPS =  include/bomb.h \
		include/entity.h \
		include/game_state.h \
		include/json.hpp \
		include/location.h \
		include/player.h \
		include/powerup.h \

run: bin/bot.exe
	gameengine/Bomberman.exe -b . 18plusplus --pretty

rusuh: bin/bot.exe
	gameengine/Bomberman.exe -b . 18plusplus . 18plusplus --pretty
	
demo: bin/bot.exe
	gameengine/Bomberman.exe -b . coba 18plusplus bot-reference --pretty

duel: bin/bot.exe
	gameengine/Bomberman.exe -b . coba --pretty
	
bin/%.o: src/%.cpp $(DEPS)
	g++ $(CFLAGS) -c -o $@ $<

build: bin/algorithm.o bin/bomb.o bin/game_state.o bin/player.o bin/main.o bin/powerup.o
	g++ $(CFLAGS) bin/algorithm.o bin/bomb.o bin/game_state.o bin/player.o bin/main.o bin/powerup.o -o bin/bot
