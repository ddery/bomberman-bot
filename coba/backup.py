import json,time
import random
import sys

DWALL = 11
UWALL = 12
JALAN = 13
BOMB = 14
PLAYER = 15
MUSUH = 16
SUPER = 17
POWERBAG = 18
POWERBOMB = 19
NAMA_PLAYER = "Hilmi"

ATAS = 1
BAWAH = 4
KANAN = 3
KIRI = 2

INF = 1000000

data_directory = ""
if len(sys.argv) >= 3:
	data_directory = sys.argv[2]
player_key = "A"
if len(sys.argv) >= 2:
	player_key = sys.argv[1]

"""Membaca file eksternal state.json"""
from pprint import pprint
with open(data_directory + '\\state.json') as data_file:    
    data = json.load(data_file)

"""Prosedur untuk mencetak Peta"""
def cetakMatriks(matrik):
	for x in matrik:
		print(x)

def cetakPeta(matrik):
	for i in range(rowSize):
		for j in range(colSize): 
			if matrik[i+1][j+1] == DWALL:
				print('+',end = '')
			elif matrik[i+1][j+1] == UWALL:
				print('#',end = '')
			elif matrik[i+1][j+1] == JALAN:
				print(' ',end = '')
			elif matrik[i+1][j+1] == PLAYER:
				print('P',end = '')
			elif matrik[i+1][j+1] == MUSUH:
				print('M',end = '')
			elif matrik[i+1][j+1] == BOMB:
				print('B',end = '')
			elif matrik[i+1][j+1] == SUPER:
				print('S',end = '')
			elif matrik[i+1][j+1] == POWERBAG:
				print('b',end = '')
			elif matrik[i+1][j+1] == POWERBOMB:
				print('p',end = '')
		print("\n", end = '')
		

"""Prosedur untuk mengambil data dari file state.json"""
def getData(Full,Src,Dst,powerup):
	for i in range(rowSize):
		for j in range(colSize): 
			if Src[i][j]["Entity"] == None:
				if Src[i][j]["PowerUp"] != None:
					if Src[i][j]["PowerUp"]["$type"] == "Domain.Entities.PowerUps.BombBagPowerUpEntity, Domain":
						Dst[j+1][i+1] = POWERBAG
						powerup.append(("PowerBag",Src[i][j]["PowerUp"]["Location"]["X"],Src[i][j]["PowerUp"]["Location"]["Y"]))
					elif Src[i][j]["PowerUp"]["$type"] == "Domain.Entities.PowerUps.SuperPowerUp, Domain":	
						Dst[j+1][i+1] = SUPER
						powerup.insert(0, ("Super",Src[i][j]["PowerUp"]["Location"]["X"],Src[i][j]["PowerUp"]["Location"]["Y"]))
					else:
						Dst[j+1][i+1] = POWERBOMB
						powerup.append(("PowerBomb",Src[i][j]["PowerUp"]["Location"]["X"],Src[i][j]["PowerUp"]["Location"]["Y"]))
				elif Src[i][j]["Bomb"] != None:
					Dst[j+1][i+1] = BOMB
					tambahBom(bombs,Src[i][j]["Bomb"])
				else:
					Dst[j+1][i+1] = JALAN	
			else: 
				if Src[i][j]["Entity"]["$type"] == "Domain.Entities.IndestructibleWallEntity, Domain":
					Dst[j+1][i+1] = UWALL
				elif Src[i][j]["Entity"]["$type"] == "Domain.Entities.DestructibleWallEntity, Domain":
					Dst[j+1][i+1] = DWALL
				elif Src[i][j]["Entity"]["$type"] == "Domain.Entities.PlayerEntity, Domain" and Src[i][j]["Bomb"] != None:
					Dst[j+1][i+1] = BOMB
					tambahBom(bombs,Src[i][j]["Bomb"])
	for x in Full["RegisteredPlayerEntities"]:
		if x["Key"] == player_key:
			setPemain(player,x)
			Dst[x["Location"]["Y"]][x["Location"]["X"]] = PLAYER
		else:
			setPemain(musuh,x)
			Dst[x["Location"]["Y"]][x["Location"]["X"]] = MUSUH

"""Menambahkan data bom yang ada pada Peta"""
def tambahBom(container, masukan):
	container.append({"X" : masukan["Location"]["X"] , "Y" : masukan["Location"]["Y"], "R" : masukan["BombRadius"]})

def setPemain(pemain,masukan):
	pemain.update({"X" : masukan["Location"]["X"] , "Y" : masukan["Location"]["Y"] , "R" : masukan["BombRadius"], "BombBag" : masukan["BombBag"] })

def isDWall(pemain,matrik):
	if matrik[pemain["Y"] + 1][pemain["X"]] == DWALL or matrik[pemain["Y"] - 1][pemain["X"]] == DWALL  or matrik[pemain["Y"]][pemain["X"] + 1] == DWALL or matrik[pemain["Y"]][pemain["X"] - 1] == DWALL:
		return True

def generateBombJangkauan(bomblist, bombs):
	if bombs: #mengecek list tidak kosong
		for x in bombs:
			for r in range(x["R"]+1):
				if r == 0:
					bomblist.append((x["X"],x["Y"]))
				else:
					bomblist.append((x["X"]+1,x["Y"]))
					bomblist.append((x["X"]-1,x["Y"]))
					bomblist.append((x["X"],x["Y"]+1))
					bomblist.append((x["X"],x["Y"]-1))

def jalan(x,y,mat):
	return mat[y][x] == JALAN

def aman(x,y,mat,jangkauan):
	return mat[y][x] == JALAN and (x,y) not in jangkauan

def cariAman(pos, jangkauan,mat):
	pilihan = []
	if mat[pos[1]+1][pos[0] + 1] == UWALL and mat[pos[1]-1][pos[0] + 1] == UWALL and mat[pos[1]-1][pos[0] - 1] == UWALL and mat[pos[1]+1][pos[0] - 1] == UWALL:	
		print("4 arah")
		if aman(pos[0]+1, pos[1],mat,jangkauan):
			pilihan.append(KANAN)
		if aman(pos[0]-1, pos[1],mat,jangkauan):
			pilihan.append(KIRI)
		if aman(pos[0],pos[1]+1,mat,jangkauan):
			pilihan.append(BAWAH)
		if aman(pos[0],pos[1]-1,mat,jangkauan):
			pilihan.append(ATAS)
		if not pilihan: #list empty
			print("not pilihan 1")
			if jalan(pos[0]+1,pos[1],mat) and aman(pos[0]+2, pos[1],mat,jangkauan):
				pilihan.append(KANAN)
			if jalan(pos[0]-1,pos[1],mat) and aman(pos[0]-2, pos[1],mat,jangkauan):
				pilihan.append(KIRI)
			if jalan(pos[0],pos[1]+1,mat) and aman(pos[0],pos[1]+2,mat,jangkauan):
				pilihan.append(BAWAH)
			if jalan(pos[0],pos[1]-1,mat) and aman(pos[0],pos[1]-2,mat,jangkauan):
				pilihan.append(ATAS)
			if not pilihan:
				print("not pilihan 2")
				if jalan(pos[0]+1,pos[1],mat) and jalan(pos[0]+2, pos[1],mat) and (aman(pos[0]+2, pos[1]+1,mat,jangkauan) or aman(pos[0]+2, pos[1]-1,mat,jangkauan) or aman(pos[0]+3, pos[1],mat,jangkauan)):
					pilihan.append(KANAN)
				if jalan(pos[0]-1,pos[1],mat) and jalan(pos[0]-2, pos[1],mat) and (aman(pos[0]-2, pos[1]+1,mat,jangkauan) or aman(pos[0]-2, pos[1]-1,mat,jangkauan) or aman(pos[0]-3, pos[1],mat,jangkauan)):
					pilihan.append(KIRI)
				if jalan(pos[0],pos[1]+1,mat) and jalan(pos[0],pos[1]+2,mat) and (aman(pos[0]-1, pos[1]+2,mat,jangkauan) or aman(pos[0]+1, pos[1]+2,mat,jangkauan) or aman(pos[0], pos[1]+3,mat,jangkauan)):
					pilihan.append(BAWAH)
				if jalan(pos[0],pos[1]-1,mat) and jalan(pos[0],pos[1]-2,mat) and (aman(pos[0]-1, pos[1]-2,mat,jangkauan) or aman(pos[0]+1, pos[1]-2,mat,jangkauan) or aman(pos[0], pos[1]-3,mat,jangkauan)):
					pilihan.append(ATAS)
				if not pilihan:
					print("not pilihan 3")
					if jalan(pos[0]+1,pos[1],mat) and jalan(pos[0]+2, pos[1],mat,jangkauan):
						pilihan.append(KANAN)
					if jalan(pos[0]-1,pos[1],mat) and jalan(pos[0]-2, pos[1],mat,jangkauan):
						pilihan.append(KIRI)
					if jalan(pos[0],pos[1]+1,mat) and jalan(pos[0],pos[1]+2,mat,jangkauan):
						pilihan.append(BAWAH)
					if jalan(pos[0],pos[1]-1,mat) and jalan(pos[0],pos[1]-2,mat,jangkauan):
						pilihan.append(ATAS)
					if not pilihan:
						print("not pilihan 4")
						if jalan(pos[0]+1,pos[1],mat):
							pilihan.append(KANAN)
						if jalan(pos[0]-1,pos[1],mat):
							pilihan.append(KIRI)
						if jalan(pos[0],pos[1]+1,mat):
							pilihan.append(BAWAH)
						if jalan(pos[0],pos[1]-1,mat):
							pilihan.append(ATAS)
						if not pilihan:
							print("not pilihan 5")
							pilihan = [1,2,3,4]
	else:
		if (mat[pos[1]+1][pos[0]] == UWALL):
			print("kesamping")
			if aman(pos[0]+1,pos[1],mat,jangkauan) and (aman(pos[0]+1,pos[1]+1,mat,jangkauan) or aman(pos[0]+1,pos[1]-1,mat,jangkauan)):
				pilihan.append(KANAN)
			if aman(pos[0]-1,pos[1],mat,jangkauan) and (aman(pos[0]-1,pos[1]+1,mat,jangkauan) or aman(pos[0]-1,pos[1]-1,mat,jangkauan)):
				pilihan.append(KIRI)
			if not pilihan:
				print("not pilihan 1")
				if jalan(pos[0]+1,pos[1],mat) and (aman(pos[0]+1,pos[1]+1,mat,jangkauan) or aman(pos[0]+1,pos[1]-1,mat,jangkauan)):
					pilihan.append(KANAN)
				if jalan(pos[0]-1,pos[1],mat) and (aman(pos[0]-1,pos[1]+1,mat,jangkauan) or aman(pos[0]-1,pos[1]-1,mat,jangkauan)):
					pilihan.append(KIRI)
				if not pilihan:
					print("not pilihan 2")
					if jalan(pos[0]+1,pos[1],mat) and (jalan(pos[0]+1,pos[1]+1,mat) or jalan(pos[0]+1,pos[1]-1,mat)):
						pilihan.append(KANAN)
					if jalan(pos[0]-1,pos[1],mat) and (jalan(pos[0]-1,pos[1]+1,mat) or jalan(pos[0]-1,pos[1]-1,mat)):
						pilihan.append(KIRI)
					if not pilihan:
						print("not pilihan 3")
						if jalan(pos[0]+1,pos[1],mat):
							pilihan.append(KANAN)
						if jalan(pos[0]-1,pos[1],mat):
							pilihan.append(KIRI)
		else:
			print("atas bawah")
			if aman(pos[0],pos[1]+1,mat,jangkauan) and (aman(pos[0]-1,pos[1]+1,mat,jangkauan) or aman(pos[0]+1,pos[1]+1,mat,jangkauan)):
				pilihan.append(BAWAH)
			if aman(pos[0],pos[1]-1,mat,jangkauan) and (aman(pos[0]-1,pos[1]-1,mat,jangkauan) or aman(pos[0]+1,pos[1]-1,mat,jangkauan)):
				pilihan.append(ATAS)
			if not pilihan:
				print("not pilihan 1")
				if jalan(pos[0],pos[1]+1,mat) and (aman(pos[0]-1,pos[1]+1,mat,jangkauan) or aman(pos[0]+1,pos[1]+1,mat,jangkauan)):
					pilihan.append(BAWAH)
				if jalan(pos[0],pos[1]-1,mat) and (aman(pos[0]-1,pos[1]-1,mat,jangkauan) or aman(pos[0]+1,pos[1]-1,mat,jangkauan)):
					pilihan.append(ATAS)
				if not pilihan:
					print("not pilihan 2")
					if jalan(pos[0],pos[1]+1,mat) and (jalan(pos[0]-1,pos[1]+1,mat) or jalan(pos[0]+1,pos[1]+1,mat)):
						pilihan.append(BAWAH)
					if jalan(pos[0],pos[1]-1,mat) and (jalan(pos[0]-1,pos[1]-1,mat) or jalan(pos[0]+1,pos[1]-1,mat)):
						pilihan.append(ATAS)
					if not pilihan:
						print("not pilihan 3")
						if jalan(pos[0],pos[1]+1,mat):
							pilihan.append(BAWAH)
						if jalan(pos[0],pos[1]-1,mat):
							pilihan.append(ATAS)
	print("pilihan = ", pilihan)
	return random.choice(pilihan)

def generatePathUtil(cur, target, mat):
	# visited[cur[0]][cur[1]] = True
	
	visited = []
	for i in range(rowSize+1):
		baris = []
		for j in range(colSize+1):
			baris.append(False)
		visited.append(baris)
	
	queue = [[cur,-1]]
	while queue:
		t = queue.pop(0)
		t_location = t[0]
		t_arah = t[1]
		print(t,target)

		if t_location[0] == target[0] and t_location[1] == target[1]:
			return t_arah
		elif not(visited[t_location[0]][t_location[1]]):
			visited[t_location[0]][t_location[1]] = True
			if visited[t_location[0]+1][t_location[1]] == False and mat[t_location[1]][t_location[0]+1] in [JALAN,SUPER,POWERBAG,POWERBOMB]:
				if t_arah == -1:
					arahnya = KANAN
				else:
				 	arahnya = t_arah
				queue.extend( [[(t_location[0]+1,t_location[1]),arahnya]] )
			if visited[t_location[0]-1][t_location[1]] == False and mat[t_location[1]][t_location[0]-1] in [JALAN,SUPER,POWERBAG,POWERBOMB]:
				if t_arah == -1:
					arahnya = KIRI
				else:
				 	arahnya = t_arah
				# arahnya = t_location == -1 ? KIRI : t_location
				queue.extend( [[(t_location[0]-1,t_location[1]),arahnya]] )
			if visited[t_location[0]][t_location[1]+1] == False and mat[t_location[1]+1][t_location[0]] in [JALAN,SUPER,POWERBAG,POWERBOMB]:
				if t_arah == -1:
					arahnya = BAWAH
				else:
				 	arahnya = t_arah
				# arahnya = t_location == -1 ? BAWAH : t_location
				queue.extend( [[(t_location[0],t_location[1]+1),arahnya]] )
			if visited[t_location[0]][t_location[1]-1] == False and mat[t_location[1]-1][t_location[0]] in [JALAN,SUPER,POWERBAG,POWERBOMB]:
				if t_arah == -1:
					arahnya = ATAS
				else:
				 	arahnya = t_arah
				# arahnya = t_location == -1 ? ATAS : t_location
				queue.extend( [[(t_location[0],t_location[1]-1),arahnya]] )



			


#cek apakah ada path ke item
def cariItem(pemain,mat,barang):
	listPath = []
	for x in barang:
		initial = (pemain["X"],pemain["Y"]) #get posisi current player
		final = (x[1],x[2]) #mengambil posisi item yang akan diambil
		cariPath(initial,final,mat,listPath)
		print(listPath)
		if listPath:
			if listPath[0][0] == listPath[1][0] + 1:
				return 2
			elif listPath[0][0] == listPath[1][0] - 1:
				return 3
			elif listPath[0][1] == listPath[1][1] + 1:
				return 1
			else:
				return 4
	if not listPath:
		return 0;

def isMusuhDisekitar(pemain,lawan):
	return lawan["X"]  < pemain["X"] + 7 and lawan["X"] > pemain["X"] - 7 and lawan["Y"] < pemain["Y"] + 7 and lawan["Y"] > pemain["Y"] - 7   

def adaJalan(pemain,lawan,mat):
	listPath = []
	initial = (pemain["X"],pemain["Y"]) #get posisi current player
	final = (lawan["X"],lawan["Y"]) #mengambil posisi lawan yang akan didekati
	cariPath(initial,final,mat,listPath)
	# print("path ",listPath)
	if listPath:
		if listPath[0][0] == listPath[1][0] + 1:
			return 2
		elif listPath[0][0] == listPath[1][0] - 1:
			return 3
		elif listPath[0][1] == listPath[1][1] + 1:
			return 1
		else:
			return 4
	if not listPath:
		return 0

def isMusuhDisamping(pemain,lawan):
	if lawan["X"]  == pemain["X"] + 1 and lawan["Y"] == pemain["Y"]:
		return True
	elif lawan["X"]  == pemain["X"] - 1 and lawan["Y"] == pemain["Y"]:
		return True
	elif lawan["X"]  == pemain["X"] and lawan["Y"] == pemain["Y"] + 1:
		return True
	elif lawan["X"]  == pemain["X"] and lawan["Y"] == pemain["Y"] -1:
		return True
	else:
		return False

def isTembokSekitar(pemain,mat):
	if mat[pemain["Y"]][pemain["X"] + 1] == DWALL:
		return True
	elif mat[pemain["Y"]][pemain["X"] -1] == DWALL:
		return True
	elif mat[pemain["Y"] + 1][pemain["X"]] == DWALL:
		return True
	elif mat[pemain["Y"] - 1][pemain["X"]] == DWALL:
		return True
	else:
		return False

def cariTembok(pemain,mat,jangkauan):
	pindah = -1
	pilihan = [1,2,3,4]
	while pindah == -1:
		pindah = random.choice(pilihan)
		if pindah == ATAS:
			if mat[pemain["Y"] - 1][pemain["X"]] != JALAN or (pemain["X"], pemain["Y"] - 1) in jangkauan:
				pindah = -1
				pilihan.remove(ATAS)
		if pindah == BAWAH:
			if mat[pemain["Y"] + 1][pemain["X"]] != JALAN or (pemain["X"], pemain["Y"] + 1) in jangkauan:
				pindah = -1
				pilihan.remove(BAWAH)
		if pindah == KANAN:
			if mat[pemain["Y"]][pemain["X"] + 1] != JALAN or (pemain["X"]+1, pemain["Y"]) in jangkauan:
				pindah = -1
				pilihan.remove(KANAN)
		if pindah == KIRI:
			if mat[pemain["Y"]][pemain["X"] - 1] != JALAN or (pemain["X"]-1, pemain["Y"]) in jangkauan:
				pindah = -1
				pilihan.remove(KIRI)
	return pindah

def isSekitarAman(pemain,bomblist,mat):
	if mat[pemain["X"] + 1][pemain["Y"]] == JALAN and (pemain["X"] + 1 , pemain["Y"]) not in bomblist:
		return True
	elif mat[pemain["X"] -1][pemain["Y"]] == JALAN and (pemain["X"] - 1 , pemain["Y"]) not in bomblist :
		return True
	elif mat[pemain["X"]][pemain["Y"] + 1] == JALAN and (pemain["X"], pemain["Y"] + 1) not in bomblist:
		return True
	elif mat[pemain["X"]][pemain["Y"] - 1] == JALAN and (pemain["X"], pemain["Y"] - 1) not in bomblist:
		return True
	else:
		return False
	
def sebelah(pemain,barang):
	for x in barang:
		if x[1]  == pemain["X"] + 1 and x[2] == pemain["Y"]:
			return KANAN
		elif x[1]  == pemain["X"] - 1 and x[2] == pemain["Y"]:
			return KIRI
		elif x[1]  == pemain["X"] and x[2] == pemain["Y"] + 1:
			return BAWAH
		elif x[1] == pemain["X"] and x[2] == pemain["Y"] - 1:
			return ATAS
	return 0


#Matriks berisi semua matrik GameBlocks dari state.json
Matriks = data["GameBlocks"]

#Baris dan Kolom pada Peta
rowSize = data["MapHeight"]
colSize = data["MapWidth"]

#Deklarasi variabel Peta
Peta = []

#Inisialisasi nilai tiap matrik pada peta dengan 0
for i in range(rowSize+1):
	baris = []
	for j in range(colSize+1):
		baris.append(0)
	Peta.append(baris)

#List berisi seluruh bom yang ada di peta
bombs = []

#Inisialisasi player dan musuh
player = {"X": 0,"Y" : 0,"R" : 0 ,"BombBag" : 0}
musuh = {"X": 0,"Y" : 0,"R" : 0 ,"BombBag" : 0}

#list berisi item power up
item = []


def ambilItemBagus(barang,mat,output):
	for T in barang:
		if jalan(T[1]+1,T[2],mat) or jalan(T[1]-1,T[2],mat) or jalan(T[1],T[2]+1,mat) or jalan(T[1],T[2]-1,mat):
			output.append(T)

#Untuk Generate Peta, p
getData(data,Matriks,Peta,item)

itemBagus = []
ambilItemBagus(item,Peta,itemBagus)

#list berisi (x,y) posisi jangkaun bomb
bombJangkauan = []
generateBombJangkauan(bombJangkauan,bombs)




#Mengecek isi peta
print("\n")
print("Peta yang diperoleh:")
cetakPeta(Peta)
print("\n")


#print Item
print("Item")
for x in item:
	print("Type = ",x[0])
	print("X = ",x[1])
	print("Y = ",x[2])
print("\n")

#coba melihat bomb
print("Bombs:")
for x in bombs:
	print("X = ", x["X"])
	print("Y = ", x["Y"])
	print("R = ", x["R"])
print("\n")

#Coba melihat atribut player
print("Player:")
print("X = ", player["X"])
print("Y = ", player["Y"])
print("R = ", player["R"])
print("B = ", player["BombBag"])
print("\n")

#Coba melihat atribut musuh
print("Musuh:")
print("X = ", musuh["X"])
print("Y = ", musuh["Y"])
print("R = ", musuh["R"])
print("B = ", musuh["BombBag"])
print("\n")

"""
langkah = []

inisialisasiLangkah(langkah)
langkah.remove(-1)

for x in langkah:
	print(x)

if isDWall(player,Peta):
	print("pasang bom")

#print jangkauan Bomb
for x in bombJangkauan:
	print(x)

#cek keanggotaan
if (not (15,15) in bombJangkauan):
	print("tidak  ada") 

#cari Path
output = []
awal = (6,2)
tujuan = (6,6)
cariPath(awal,tujuan,Peta,output)
print(output)
"""

#ALGORITMA
move = -1
currentPos = (player["X"] , player["Y"])
print(currentPos)
if currentPos in bombJangkauan:
	print("ada bom")
	move = cariAman(currentPos,bombJangkauan,Peta)
else:
	if itemBagus:
		move = generatePathUtil(currentPos,(itemBagus[0][1],itemBagus[0][2]), Peta)
	else: #tidak ada item yang bisa diambil
		if isMusuhDisekitar(player,musuh):
			 print("Musuh Disekitar")
			 move = generatePathUtil(currentPos,(musuh["X"],musuh["Y"]), Peta)
			 if isMusuhDisamping(player,musuh):
			 	move = 5
		elif isTembokSekitar(player,Peta):
		 	move = 5
		 	# if isSekitarAman(player,bombJangkauan,Peta):
		 	# 	move = 5
		 	# else:
		 	# 	move = generatePathUtil(currentPos,(musuh["X"],musuh["Y"]), Peta)
		else:
			print("cari tembok")
			move = cariTembok(player,Peta,bombJangkauan)

print(bombs)
for x in bombJangkauan:
	print(x)
print(move)

with open(data_directory + '\\move.txt',"w+") as f:
		f.write('{}\n'.format(move))
		
# print(Peta[3][2])
# print(random.choice([1,2,3,4]))