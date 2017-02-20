# Bomberman BOT

## Mengcompile Bot
Untuk melakukan kompilasi, cukup jalankan ```make build``` pada terminal, atau command prompt.
Pastikan program g++ dan make sudah terinstall.

Untuk mengcompile tanpa menggunakan make dapat dilakukan dengan perintah berikut

```g++ src/algorithm.cpp src/bomb.cpp src/game_state.cpp src/main.cpp src/player.cpp src/powerup.cpp -Iinclude -std=c++11 -o bin/bot```
	
	
## Menjalankan bot
Untuk menjalankan demo bot, dapat dilakukan dengan menjalankan run.bat

Untuk menjalankan bot dengan bot lain dapat dilakukan dengan menjalankan game engine seperti berikut

```gameengine\Bomberman.exe -b [folder_bot] -c [jumlah_pemain_bukan_bot] --pretty```
