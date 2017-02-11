# Breath First Search Dalam Map

Untuk lebih gampangnya, contohnya dikasih map kaya gini.

                1         2
       123456789012345678901
     1 #####################
     2 #           7   +++ #
     3 # # # # # # #A# # # #
     4 #          1        #
     5 # # # # # # # # # # #
     6 #   ++    +      ++ #
     7 # # # #+# # # # # #+#
     8 #+ +   + + +     + +#
     9 #+# # # #+#+# # # #+#
    10 #   +  +++ +++  +   #
    11 #+# # #+#   #+# # #+#
    12 #       +       +   #
    13 #+# # # # # # # # #+#
    14 #+               + +#
    15 #+# # # # # # # # #+#
    16 #         +         #
    17 # # #+#+#+#+# # # #+#
    18 #+b  + +++ +++      #
    19 # # # #+# # #+# # # #
    20 # +++  ++   ++      #
    21 #####################

Kita mau tentuin dari A ke semua titik lain jaraknya berapa. Nah ini bisa diselesain pakai BFS. Jadi idenya gini, pertama kita kan di A tuh, posisinya di (3,14). Jadi kita bise tentuin, A kalau mau ke (3,14) tuh jaraknya 0. Nah dari (3,14) itu kita bisa tentuin nih, brarti kalau mau ke (3,15),(3,13),(2,14),(4,14) jaraknya satu (jarak (3,14) + 1). Nanti dari 4 posisi itu, kita bisa lanjut ke berikutnya.

Nah idenya sih gitu, implementasinya gimana? Ini bisa diimplementasiin pake queue dan matriks. Pertama bikin matriks jarak yang ukurannya seukuran peta. Pertama-tama set jarak nya jadi tak hingga semua dulu, atau mau di set -1 juga bisa sih.
    
    INF = 2000000000;
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            jarak[i][j] = INF;

Terus queue-nya buat apa? Queue-nya itu buat nampung posisi mana aja yang mau kita proses. Pastinya yang mau diproses pertama tu si-A kan, jadi pertama-tama kita push dulu si A ini kedalem queue. Pertama kita bikin dulu queue Q terus isi dengan A dengan jarak 0. jadi queue-nya isinya ```pair<location,int>```. Location buat nampung posisi mana yang mau diproses, int nampung jarak posisi itu.

    Q.push({{3,14},0});
    
Terus tinggal diproses satu-persatu. Kalau posisi yang mau diproses udah pernah diproses gausah diposes lagi. Kalau belum pernah tinggal set matriks jaraknya, terus tambah proses ke tetangga-tetangganya.

    while (!Q.empty()) {
        pair<location,int> current = Q.front(); Q.pop();
        if (jarak[current.first.Y][current.first.X] == INF) {
            jarak[current.first.Y][current.first.X] = current.second;
            if (jarak[current.first.Y][current.first.X+1] == INF && bisa_ditempati(current.first.Y,current.first.X+1))
                Q.push({current.first.X+1,current.first.Y},current.second+1);
            if (jarak[current.first.Y][current.first.X-1] == INF && bisa_ditempati(current.first.Y,current.first.X-1))
                Q.push({current.first.X-1,current.first.Y},current.second+1);
                ..... intinya gitulah ......
        }
    }
    
Baris pertama tu buat ngecek apakah masih ada yang harus diproses ```while (!Q.empty())```
Barus kedua tu buat dapetin lokasi yang harus diproses
Baris ketiga ngecek, apakah sebelumnya pernah diproses, kalau udah pernah mending gausah diproses lagi
Baris keempat ngeset jarak lokasi saat.
Baris sisanya tu masukin tetangga-tetangganya untuk diproses lebih lanjut.

Intinya sih gitu.

Contoh program jadinya gini

    #include <bits/stdc++.h>

    #define INF 2000000000

    using namespace std;

    typedef struct {
        int x,y;
    } location;
    
    string petak[21];
    int N,M,jarak[21][21];
    
    int main() {
    	N = 21;
    	M = 21;
    	petak[0] = "#####################";
        petak[1] = "#           7   +++ #";
        petak[2] = "# # # # # # #A# # # #";
        petak[3] = "#          1        #";
        petak[4] = "# # # # # # # # # # #";
        petak[5] = "#   ++    +      ++ #";
        petak[6] = "# # # #+# # # # # #+#";
        petak[7] = "#+ +   + + +     + +#";
        petak[8] = "#+# # # #+#+# # # #+#";
        petak[9] = "#   +  +++ +++  +   #";
        petak[10] = "#+# # #+#   #+# # #+#";
        petak[11] = "#       +       +   #";
        petak[12] = "#+# # # # # # # # #+#";
        petak[13] = "#+               + +#";
        petak[14] = "#+# # # # # # # # #+#";
        petak[15] = "#         +         #";
        petak[16] = "# # #+#+#+#+# # # #+#";
        petak[17] = "#+b  + +++ +++      #";
        petak[18] = "# # # #+# # #+# # # #";
        petak[19] = "# +++  ++   ++      #";
        petak[20] = "#####################";
    	
    	for (int i = 0; i < N; i++)
            for (int j = 0; j < M; j++)
                jarak[i][j] = INF;
    
        queue<pair<location,int> > Q;
        Q.push({{13,2},0});
        while (!Q.empty()) {
            pair<location,int> current = Q.front(); Q.pop();
            if (jarak[current.first.y][current.first.x] == INF) {
                jarak[current.first.y][current.first.x] = current.second;
                if (jarak[current.first.y][current.first.x+1] == INF && petak[current.first.y][current.first.x+1] == ' ')
                    Q.push({{current.first.x+1,current.first.y},current.second+1});
                if (jarak[current.first.y][current.first.x-1] == INF && petak[current.first.y][current.first.x-1] == ' ')
                    Q.push({{current.first.x-1,current.first.y},current.second+1});
                if (jarak[current.first.y+1][current.first.x] == INF && petak[current.first.y+1][current.first.x] == ' ')
                    Q.push({{current.first.x,current.first.y+1},current.second+1});
                if (jarak[current.first.y-1][current.first.x] == INF && petak[current.first.y-1][current.first.x] == ' ')
                    Q.push({{current.first.x,current.first.y-1},current.second+1});
            }
        }
    
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < M; j++)
                printf("%5d ", jarak[i][j] == INF ? 999 : jarak[i][j]);
            printf("\n");
        }
    	
    	return 0;
    }
