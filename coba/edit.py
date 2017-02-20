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
TIDAK_ADA_JALAN = 99


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
			Dst[x["Location"]["Y"]][x["Location"]["X"]] = MUSUH
			if (x["Killed"] == False):
				setPemain(musuh,x)


def setPemain(pemain,masukan):
	pemain.update({"X" : masukan["Location"]["X"] , "Y" : masukan["Location"]["Y"] , "R" : masukan["BombRadius"], "BombBag" : masukan["BombBag"] })

def isDWall(pemain,matrik):
	if matrik[pemain["Y"] + 1][pemain["X"]] == DWALL or matrik[pemain["Y"] - 1][pemain["X"]] == DWALL  or matrik[pemain["Y"]][pemain["X"] + 1] == DWALL or matrik[pemain["Y"]][pemain["X"] - 1] == DWALL:
		return True

"""Menambahkan data bom yang ada pada Peta"""
def tambahBom(container, masukan):
	container.append({"X" : masukan["Location"]["X"] , "Y" : masukan["Location"]["Y"], "R" : masukan["BombRadius"] ,"T" : masukan["BombTimer"]})

def inRange(x,y):
	return x < colSize and x >= 0 and y < rowSize and y >= 0

def generateBombJangkauan(bomblist, bombs,mat):
	kanan = True
	kiri = True
	atas = True
	bawah = True
	if bombs: #mengecek list tidak kosong
		for x in bombs:
			var = x["R"] + 1
			for r in range(var):
				if r == 0:
					bomblist.append((x["X"],x["Y"]))
				else:
					if inRange(x["Y"],x["X"]+r) and mat[x["Y"]][x["X"]+r] in [JALAN,SUPER,POWERBAG,POWERBOMB,MUSUH] and kanan:
						bomblist.append((x["X"]+r,x["Y"]))
					else:
						kanan = False
					if inRange(x["Y"],x["X"]-r) and mat[x["Y"]][x["X"]-r] in [JALAN,SUPER,POWERBAG,POWERBOMB,MUSUH] and kiri:
						bomblist.append((x["X"]-r,x["Y"]))
					else:
						kiri = False
					if inRange(x["Y"]+r,x["X"]) and mat[x["Y"]+r][x["X"]] in [JALAN,SUPER,POWERBAG,POWERBOMB,MUSUH] and bawah:
						bomblist.append((x["X"],x["Y"]+r))
					else:
						bawah = False
					if inRange(x["Y"]-r,x["X"]) and mat[x["Y"]-r][x["X"]] in [JALAN,SUPER,POWERBAG,POWERBOMB,MUSUH] and atas:
						bomblist.append((x["X"],x["Y"]-r))
					else:
						atas = False

def jalan(x,y,mat):
	return mat[y][x] == JALAN

def cekplayer(x,y,mat):
	return mat[y][x] == PLAYER

def aman(x,y,mat,jangkauan):
	return mat[y][x] == JALAN and (x,y) not in jangkauan

def generateMatriksJarak(cur,mat):
	visited = []
	for i in range(rowSize+1):
		baris = []
		for j in range(colSize+1):
			baris.append(False)
		visited.append(baris)
	
	matrik = []
	for i in range(rowSize+1):
		baris = []
		for j in range(colSize+1):
			baris.append((INF,INF))
		matrik.append(baris)

	queue = [[cur,-1,0]]

	while queue:
		t = queue.pop(0)
		t_location = t[0]
		t_arah = t[1]
		t_jarak = t[2]
		
		if not(visited[t_location[0]][t_location[1]]):
			matrik[t_location[0]][t_location[1]] = (t_arah,t_jarak)
			visited[t_location[0]][t_location[1]] = True
			if visited[t_location[0]+1][t_location[1]] == False and mat[t_location[1]][t_location[0]+1] in [JALAN,SUPER,POWERBAG,POWERBOMB]:
				if t_arah == -1:
					arahnya = KANAN
				else:
				 	arahnya = t_arah
				queue.extend( [[(t_location[0]+1,t_location[1]),arahnya,t_jarak+1]] )
			if visited[t_location[0]-1][t_location[1]] == False and mat[t_location[1]][t_location[0]-1] in [JALAN,SUPER,POWERBAG,POWERBOMB]:
				if t_arah == -1:
					arahnya = KIRI
				else:
					arahnya = t_arah
				queue.extend( [[(t_location[0]-1,t_location[1]),arahnya,t_jarak+1]] )
			if visited[t_location[0]][t_location[1]+1] == False and mat[t_location[1]+1][t_location[0]] in [JALAN,SUPER,POWERBAG,POWERBOMB]:
				if t_arah == -1:
					arahnya = BAWAH
				else:
				 	arahnya = t_arah
				queue.extend( [[(t_location[0],t_location[1]+1),arahnya,t_jarak+1]] )
			if visited[t_location[0]][t_location[1]-1] == False and mat[t_location[1]-1][t_location[0]] in [JALAN,SUPER,POWERBAG,POWERBOMB]:
				if t_arah == -1:
					arahnya = ATAS
				else:
				 	arahnya = t_arah
				queue.extend( [[(t_location[0],t_location[1]-1),arahnya,t_jarak+1]] )
	return matrik		



def generatePathUtil(cur, target, mat):
	
	visited = []
	for i in range(rowSize+1):
		baris = []
		for j in range(colSize+1):
			baris.append(False)
		visited.append(baris)
	
	jarak = 0
	queue = [[cur,-1,0]]
	while queue:
		t = queue.pop(0)
		t_location = t[0]
		t_arah = t[1]
		t_jarak = t[2]
		print(t,target)

		if t_location[0] == target[0] and t_location[1] == target[1]:
			return t
		elif not(visited[t_location[0]][t_location[1]]):
			visited[t_location[0]][t_location[1]] = True
			if visited[t_location[0]+1][t_location[1]] == False and mat[t_location[1]][t_location[0]+1] in [JALAN,SUPER,POWERBAG,POWERBOMB]:
				if t_arah == -1:
					arahnya = KANAN
				else:
				 	arahnya = t_arah
				queue.extend( [[(t_location[0]+1,t_location[1]),arahnya,t_jarak+1]] )
			if visited[t_location[0]-1][t_location[1]] == False and mat[t_location[1]][t_location[0]-1] in [JALAN,SUPER,POWERBAG,POWERBOMB]:
				if t_arah == -1:
					arahnya = KIRI
				else:
					arahnya = t_arah
				queue.extend( [[(t_location[0]-1,t_location[1]),arahnya,t_jarak+1]] )
			if visited[t_location[0]][t_location[1]+1] == False and mat[t_location[1]+1][t_location[0]] in [JALAN,SUPER,POWERBAG,POWERBOMB]:
				if t_arah == -1:
					arahnya = BAWAH
				else:
				 	arahnya = t_arah
				queue.extend( [[(t_location[0],t_location[1]+1),arahnya,t_jarak+1]] )
			if visited[t_location[0]][t_location[1]-1] == False and mat[t_location[1]-1][t_location[0]] in [JALAN,SUPER,POWERBAG,POWERBOMB]:
				if t_arah == -1:
					arahnya = ATAS
				else:
				 	arahnya = t_arah
				queue.extend( [[(t_location[0],t_location[1]-1),arahnya,t_jarak+1]] )
	
def isMusuhDisekitar(pemain,lawan):
	return lawan["X"]  < pemain["X"] + 7 and lawan["X"] > pemain["X"] - 7 and lawan["Y"] < pemain["Y"] + 7 and lawan["Y"] > pemain["Y"] - 7   

def adaJalanKeMusuh(pemain,lawan,mat):
	return generatePathUtil((pemain["X"],pemain["Y"]),(lawan["X"],lawan["Y"]),mat ) != None

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

def isTembokSekitar(pos,mat):
	if mat[pos[1]][pos[0] + 1] == DWALL:
		return True
	elif mat[pos[1]][pos[0] -1] == DWALL:
		return True
	elif mat[pos[1] + 1][pos[0]] == DWALL:
		return True
	elif mat[pos[1] - 1][pos[0]] == DWALL:
		return True
	else:
		return False

def cariTempatAman(player,mat,jangkauan):
	listTempat = []
	cPos = (player["X"] , player["Y"])
	matJarak = generateMatriksJarak(cPos,mat)
	for x in range(colSize+1):
		for y in range(rowSize+1):
			if matJarak[x][y][0] != INF and (x,y) not in jangkauan:
				listTempat.append([matJarak[x][y][1],matJarak[x][y][0],x,y])
	listTempat.sort()
	for i in listTempat:
		if mat[i[3]][i[2]] == JALAN and (i[2],i[3]) not in jangkauan:
			return i[1]
	return random.choice([1,2,3,4])

def dekatiMusuh(pemain,mat,lawan,jangkauan):
	listTempat = []
	cPos = (pemain["X"],pemain["Y"])
	matJarak = generateMatriksJarak(cPos,mat)
	for x in range(colSize+1):
		for y in range(rowSize+1):
			if x == lawan["X"] and y == lawan["Y"]:
				listTempat.append([matJarak[x][y][1],matJarak[x][y][0],x,y])
	listTempat.sort()
	for x in listTempat:
		if mat[x[3]][x[2]] == JALAN and (x[2],x[3]) not in jangkauan:
			return x[1]
	return cariTembok(pemain,mat,jangkauan,titik)

def cariItem(pemain,target,mat,jangkauan):
	listTempat = []
	cPos = (pemain["X"],pemain["Y"])
	matJarak = generateMatriksJarak(cPos,mat)
	for x in range(colSize+1):
		for y in range(rowSize+1):
			if matJarak[x][y][0] != INF and x == target[0] and y == target[1]:
				listTempat.append([matJarak[x][y][1],matJarak[x][y][0],x,y])
	listTempat.sort()
	print("list cariItem")
	print(listTempat)
	for i in listTempat:
		if (mat[i[3]][i[2]] == JALAN and (i[2],i[3]) not in jangkauan) or mat[i[3]][i[2]] in [SUPER,POWERBAG,POWERBOMB]:
			return i[1]
	return cariTembok(pemain,mat,jangkauan)

def jarak(A,B):
	dx = A[0] - B[0]
	dy = A[1] - B[1]
	return dx*dx + dy*dy

def cariTembok(pemain,mat,jangkauan):
	pos = (pemain["X"],pemain["Y"])
	listTempat = []
	matJarak = generateMatriksJarak(pos,mat)
	for x in range(colSize+1):
		for y in range(rowSize+1):
			print(x," ",y," " , matJarak[x][y][0], " ",matJarak[x][y][1])
			if matJarak[x][y][0] != INF and (x,y) not in jangkauan and isTembokSekitar((x,y),mat):
				listTempat.append([matJarak[x][y][1],matJarak[x][y][0],x,y])
	# tengah 
	# tengah = int(colSize/2) + 1
	# print("tengah = ", tengah)
	# if (mat[tengah][tengah] == SUPER):
	# 	for i in listTempat:
	# 		i[0] = jarak((i[2],i[3]),(tengah,tengah)) 
	listTempat.sort()
	# print(listTempat)
	for i in listTempat:
		# print(x)
		if i[1] == KANAN:
			cek = (pos[0]+1,pos[1])
		elif i[1] == KIRI:
			cek = (pos[0]-1,pos[1])
		elif i[1] == ATAS:
			cek = (pos[0],pos[1]-1)
		elif i[1] == BAWAH:
			cek = (pos[0],pos[1]+1)
		else:
			cek = pos
		# print(i[1], " ",cek)
		if (i[2],i[3]) not in jangkauan and cek not in jangkauan and mat[cek[1]][cek[0]] == JALAN: # and isTembokSekitar((i[2],i[3]),mat)
			return i[1]
	return cariTempatAman(pemain,mat,jangkauan)

# def titikAman(titik,bomblist):
# 	return (titik[0],titik[1],1) not in bomblist and (titik[0],titik[1],2) not in bomblist and  


def isSekitarAman(pemain,bomblist,mat):
	if mat[pemain["X"] + 1][pemain["Y"]] == JALAN and (pemain["X"] + 1 , pemain["Y"]) not in bomblist:
		return True
	elif mat[pemain["X"] -1][pemain["Y"]] == JALAN and (pemain["X"] - 1 , pemain["Y"]) not in bomblist:
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

def ambilItemBagus(barang,mat,output):
	for T in barang:
		if jalan(T[1]+1,T[2],mat) or jalan(T[1]-1,T[2],mat) or jalan(T[1],T[2]+1,mat) or jalan(T[1],T[2]-1,mat) or cekplayer(T[1]+1,T[2],mat) or cekplayer(T[1]-1,T[2],mat) or cekplayer(T[1],T[2]+1,mat) or cekplayer(T[1],T[2]-1,mat):
			output.append(T)

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



#Untuk Generate Peta, p
getData(data,Matriks,Peta,item)

itemBagus = []
ambilItemBagus(item,Peta,itemBagus)

#list berisi (x,y) posisi jangkaun bomb
bombJangkauan = []
generateBombJangkauan(bombJangkauan,bombs,Peta)


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

"""

print("Bombs Jangkauan")
if not bombJangkauan:
	print("tidak ada bomb")
for x in bombJangkauan:
	print(x)
print("selesai bomb jangkaun")

def isSebelah(A,B):
	return (A[0] == B[0]+1 and A[1] == B[1]) or (A[0] == B[0]-1 and A[1] == B[1]) or (A[0] == B[0] and A[1] == B[1] -1) or (A[0] == B[0]+1 and A[1] == B[1]+1) 

#ALGORITMA
move = -1
currentPos = (player["X"] , player["Y"])
print(currentPos)
if currentPos in bombJangkauan:
	print("ada bom")
	move = cariTempatAman(player,Peta,bombJangkauan)  #cariAman(currentPos,bombJangkauan,Peta)
else:
	if itemBagus:
		print("ambil item")
		move  = cariItem(player,(itemBagus[0][1],itemBagus[0][2]),Peta,bombJangkauan)
	else: #tidak ada item yang bisa diambil
		if adaJalanKeMusuh(player,musuh,Peta):#isMusuhDisekitar(player,musuh):
			 print("Mendekati Musuh")
			 move = dekatiMusuh(player,musuh,Peta,bombJangkauan)
			 if isMusuhDisamping(player,musuh):
			 	move = 5
		else:
			print("cari tembok")
			move = cariTembok(player,Peta,bombJangkauan)
			if isTembokSekitar(currentPos,Peta):
				move = 5
		 		# print("pasang bomb")
print("move = ",move)

# cariTempatAman(player,Peta,bombJangkauan)
# matJarak = generateMatriksJarak(currentPos,Peta)
# for x in range(colSize+1):
# 	for y in range(rowSize+1):
# 		print("(",x,",",y,") = ", matJarak[x][y])

with open(data_directory + '\\move.txt',"w+") as f:
		f.write('{}\n'.format(move))
