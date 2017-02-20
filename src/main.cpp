#include <bits/stdc++.h>
#include "game_state.h"

using namespace std;

void debug_map(GameState& gamestate, int** kena_bomb, pair<int,int>** jarak_arah, int** luas) {
    int width = gamestate.get_map_width();
    int height = gamestate.get_map_height();

    printf("   ");
    for (int i = 1; i <= width; i++)
        printf("%14d", i); printf("\n");
    for (int i = 1; i <= height; i++) {
        printf("%2d ", i);
        for (int j = 1; j <= width; j++) {
            if (gamestate[i][j].type & GameState::FREE)
                cout<<" ";
            else if (gamestate[i][j].type & GameState::BOMB)
                cout<<gamestate[i][j].bomb->radius;
            else if (gamestate[i][j].type & GameState::PLAYER)
                cout<<gamestate[i][j].player->key;
            else if (gamestate[i][j].type & GameState::BRICK)
                cout<<"+";
            else if (gamestate[i][j].type & GameState::WALL)
                cout<<"#";
            else if (gamestate[i][j].type & GameState::SUPER_POWER_UP)
                cout<<"$";
            else if (gamestate[i][j].type & GameState::RADIUS_POWER_UP)
                cout<<"!";
            else if (gamestate[i][j].type & GameState::BAG_POWER_UP)
                cout<<"&";
            else
                cout<<gamestate[i][j].type;
            printf("(%3d,%3d,%3d) ", kena_bomb[i][j], jarak_arah[i][j].first,jarak_arah[i][j].second);
        }
        cout<<endl;
    }

    printf("Luas :\n  ");
    for (int i = 1; i <= width; i++)
        printf("%4d", i); printf("\n");
    for (int i = 1; i <= height; i++) {
        printf("%2d ", i);
        for (int j = 1; j <= width; j++)
            printf("%3d ", luas[i][j]);
        printf("\n");
    }
}

/*
 * generate_kena_bomb
 * author : Dery Rahman
 * tanggal : 13 Februari 2017
 * digunakan untuk menciptakan matriks berukuran sama seperti map yang berisi
 * integer. Integer tersebut menyatakan banyaknya detik yang tersisa sehingga
 * petak yang bersangkutan akan meledak. matriks[y][x] menyatakan detik yang
 * tersisa sehingga petak (x,y) meledak
 * gamestate : berisi game state
 * return int** yang merepresentasikan matriks
 */
int** generate_kena_bomb(GameState& gamestate);

/*
 * cari_jarak_arah
 * author : Dery Rahman
 * tanggal : 14 Februari 2017
 * digunakan untuk mendapatkan jarak dari player "me" ke semua petak di peta
 * dalam representasi matriks berukuran seperti peta yang bertipe pair<int,int>.
 * nilai first menunjukkan jarak dari posisi player sekarang, second menunjukkan
 * langkah yang harus dituju untuk menuju ke posisi yang bersangkutan
 * gamestate : berisi game state
 * return : pair<int,int>* yang merepresentasikan matriks
 */
pair<int,int>** cari_jarak_arah(GameState& gamestate);

/*
 * generate_luas
 * author : Jauhar Arifin
 * tanggal : 17 Februari 2017
 * digunakan untuk menentukan luas petak. Luas petak didefinisikan sebagai
 * banyaknya petak kosong yang dapat ditempati oleh player tanpa melalui brick,
 * wall, atau petak-petak yang akan terkena bomb. Proses ini menggunakan
 * pendekatan DFS dengan algoritma floodfill.
 * gamestate : berisi game state
 * return : int** yang menyatakan matriks berisi luas petak. matriks[i][j] me-
 * nandakan luas petak pada posisi (j,i).
 */
int** generate_luas(GameState& gamestate);

/*
 * generate_point
 * author : Irfan Ariq
 * tanggal : 16 Feruari 2017
 * digunakan untuk menilai point setiap petak, setiap petak memiliki point yang
 * ditentukan dari banyaknya brick di sekitarnya dan keberadaan musuh di seki-
 * tarnya. Petak yang memiliki nilai tinggi dan jarak tempuh yang pendek akan
 * menjadi prioritas untuk dikunjungi.
 * gamestate : berisi game state
 * return : int** yang menyatakan matriks berisi point setiap petak.
 */
int** generate_point(GameState& gamestate);

int main(int argc, char** argv) {
    // Menciptakan gamestate dari input
    GameState gamestate(argc,argv);

    // Insialisasi varibel yang sering digunakan
    Player* me=gamestate.get_me();
    vector<Player> players = gamestate.get_player_vector();
    vector<PowerUp> powerups = gamestate.get_powerup_vector();
    vector<Bomb> bombs = gamestate.get_bomb_vector();

    // Menghitung matriks yang akan digunakan dalam greedy
    int** batubata = generate_point(gamestate);
    int** kena_bomb = generate_kena_bomb(gamestate);
    int** luas = generate_luas(gamestate);
	pair<int,int> **jarak_arah = cari_jarak_arah(gamestate);
    debug_map(gamestate, kena_bomb, jarak_arah, luas);

    for(int i = 1; i <= gamestate.get_map_height(); i++){
        for(int j = 1; j <= gamestate.get_map_width(); j++)
            cout << batubata[i][j] << " ";
        cout<<"\n";
    }

    // memilih satu lawan yang akan ditinjau
    Player* op = NULL;
    for (Player player : players)
        if (player.key != me->key) {
            op = &player;
            break;
        }

    // menentukan tujuan player saat ini
    Location tujuan = op->location;
    if(powerups.size() > 0){
        int jarakpowerup = jarak_arah[powerups[0].location.y][powerups[0].location.x].first;
        tujuan = powerups[0].location;
        for (PowerUp powerup : powerups)
            if(jarakpowerup > jarak_arah[powerup.location.y][powerup.location.x].first){
                jarakpowerup = jarak_arah[powerup.location.y][powerup.location.x].first;
                tujuan.x = powerup.location.x;
                tujuan.y = powerup.location.y;
            }
    }else{
        double maximum = 0.0001;
        for(int i = 1; i <= gamestate.get_map_height(); i++)
            for(int j = 1; j <= gamestate.get_map_width(); j++)
                if(maximum < (batubata[i][j]/(jarak_arah[i][j].first+1))){
                    maximum = batubata[i][j]/(jarak_arah[i][j].first+1);
                    tujuan.x = j;
                    tujuan.y = i;
                }
    }

    printf("Tujuan : (%d,%d)\n", tujuan.x, tujuan.y);

    int arah = jarak_arah[tujuan.y][tujuan.x].second;

    printf("Arah : %d\n", arah);

    // menentukan langkah yang akan diambil oleh player berdasarkan tujuan dan
    // keadaan saat ini.
    if (kena_bomb[me->location.y][me->location.x] > 0) {
        int jarakhindar = 999;
        int luashindar = -999;
        for (int i = 1; i <= gamestate.get_map_height(); i++)
            for (int j = 1; j <= gamestate.get_map_width(); j++)
                if ((jarakhindar > jarak_arah[i][j].first || (jarakhindar == jarak_arah[i][j].first && luashindar < luas[i][j])) && kena_bomb[i][j] == 0) {
                    jarakhindar = jarak_arah[i][j].first;
                    luashindar = luas[i][j];
                    tujuan.x = j;
                    tujuan.y = i;
                }
        printf("Tujuan menghindar (%d,%d) -> %d\n", tujuan.x, tujuan.y, jarak_arah[tujuan.y][tujuan.x].second);
        gamestate.decide_move(jarak_arah[tujuan.y][tujuan.x].second);
    } else {
        for (Bomb bomb : bombs)
            if (bomb.owner->key == me->key)
                arah = GameState::TRIGGER_BOMB;

        if (arah ==  GameState::GO_RIGHT && (gamestate[me->location.y][me->location.x+1].type & (GameState::BRICK | GameState::PLAYER)))
            arah = GameState::PUT_BOMB;
        else if (arah ==  GameState::GO_LEFT && (gamestate[me->location.y][me->location.x-1].type & (GameState::BRICK | GameState::PLAYER)))
            arah = GameState::PUT_BOMB;
        else if (arah ==  GameState::GO_DOWN && (gamestate[me->location.y+1][me->location.x].type & (GameState::BRICK | GameState::PLAYER)))
            arah = GameState::PUT_BOMB;
        else if (arah ==  GameState::GO_UP && (gamestate[me->location.y-1][me->location.x].type & (GameState::BRICK | GameState::PLAYER)))
            arah = GameState::PUT_BOMB;
        else if (tujuan == me->location)
            arah = GameState::PUT_BOMB;
        gamestate.decide_move(arah);
    }

    return 0;
}
