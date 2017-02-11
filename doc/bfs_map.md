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