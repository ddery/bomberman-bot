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
				elif Src[i][j]["Exploding"] == True:
					explode.append((Src[i][j]["Location"]["X"],Src[i][j]["Location"]["Y"]))
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

def isPernahPasangBom(Src):
	pernah = False
	for i in range(rowSize):
		for j in range(colSize): 
			if Src[i][j]["Bomb"]["Owner"]["Key"] != None and Src[i][j]["Bomb"]["Owner"]["Key"] == player_key:
				return True

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

def generateBombJangkauan(bomblist,bombs,mat):
	if bombs: #mengecek list tidak kosong
		for x in bombs:
			# print(x["R"])
			var = x["R"] + 1
			kanan = True
			kiri = True
			atas = True
			bawah = True
			for r in range(var):
				if r == 0:
					bomblist.append((x["X"],x["Y"]))
				else:
					if inRange(x["Y"],x["X"]+r) and mat[x["Y"]][x["X"]+r] not in [UWALL,DWALL] and kanan:
						bomblist.append((x["X"]+r,x["Y"]))
					else:
						kanan = False
					if inRange(x["Y"],x["X"]-r) and mat[x["Y"]][x["X"]-r] not in [UWALL,DWALL] and kiri:
						bomblist.append((x["X"]-r,x["Y"]))
					else:
						kiri = False
					if inRange(x["Y"]+r,x["X"]) and mat[x["Y"]+r][x["X"]] not in [UWALL,DWALL] and bawah:
						bomblist.append((x["X"],x["Y"]+r))
					else:
						bawah = False
					if inRange(x["Y"]-r,x["X"]) and mat[x["Y"]-r][x["X"]] not in [UWALL,DWALL] and atas:
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
			if visited[t_location[0]+1][t_location[1]] == False and mat[t_location[1]][t_location[0]+1] in [JALAN,SUPER,POWERBAG,POWERBOMB,MUSUH]:
				if t_arah == -1:
					arahnya = KANAN
				else:
				 	arahnya = t_arah
				queue.extend( [[(t_location[0]+1,t_location[1]),arahnya,t_jarak+1]] )
			if visited[t_location[0]-1][t_location[1]] == False and mat[t_location[1]][t_location[0]-1] in [JALAN,SUPER,POWERBAG,POWERBOMB,MUSUH]:
				if t_arah == -1:
					arahnya = KIRI
				else:
					arahnya = t_arah
				queue.extend( [[(t_location[0]-1,t_location[1]),arahnya,t_jarak+1]] )
			if visited[t_location[0]][t_location[1]+1] == False and mat[t_location[1]+1][t_location[0]] in [JALAN,SUPER,POWERBAG,POWERBOMB,MUSUH]:
				if t_arah == -1:
					arahnya = BAWAH
				else:
				 	arahnya = t_arah
				queue.extend( [[(t_location[0],t_location[1]+1),arahnya,t_jarak+1]] )
			if visited[t_location[0]][t_location[1]-1] == False and mat[t_location[1]-1][t_location[0]] in [JALAN,SUPER,POWERBAG,POWERBOMB,MUSUH]:
				if t_arah == -1:
					arahnya = ATAS
				else:
				 	arahnya = t_arah
				queue.extend( [[(t_location[0],t_location[1]-1),arahnya,t_jarak+1]] )
	return matrik		

	
def isMusuhDisekitar(pemain,lawan):
	return lawan["X"]  < pemain["X"] + 7 and lawan["X"] > pemain["X"] - 7 and lawan["Y"] < pemain["Y"] + 7 and lawan["Y"] > pemain["Y"] - 7   


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


def cariTempatAman(player,mat,jangkauan,ledakan):
	listTempat = []
	cPos = (player["X"] , player["Y"])
	matJarak = generateMatriksJarak(cPos,mat)
	for x in range(colSize+1):
		for y in range(rowSize+1):
			if matJarak[x][y][0] != INF and (x,y) not in jangkauan and (x,y) not in ledakan:
				listTempat.append([matJarak[x][y][1],matJarak[x][y][0],x,y])
	listTempat.sort()
	if cPos not in jangkauan and cPos not in ledakan:
		return -1
	for i in listTempat:
		if i[1] == KANAN:
			cek = (cPos[0]+1,cPos[1])
		elif i[1] == KIRI:
			cek = (cPos[0]-1,cPos[1])
		elif i[1] == ATAS:
			cek = (cPos[0],cPos[1]-1)
		elif i[1] == BAWAH:
			cek = (cPos[0],cPos[1]+1)
		else:
			print("exception")
			cek = cPos
		if mat[i[3]][i[2]] == JALAN and (i[2],i[3]) not in jangkauan and (i[2],i[3]) not in ledakan:
			print((i[2],i[3]))
			return i[1]
	print("random")
	return random.choice([1,2,3,4])

def cariItem(pemain,target,mat,jangkauan,ledakan):
	listTempat = []
	cPos = (pemain["X"],pemain["Y"])
	matJarak = generateMatriksJarak(cPos,mat)
	for x in range(colSize+1):
		for y in range(rowSize+1):
			if matJarak[x][y][0] != INF and x == target[0] and y == target[1]: # and (x,y) not in jangkauan and (x,y) not in ledakan:
				print("ini x dan y = ",x," ",y)
				listTempat.append([matJarak[x][y][1],matJarak[x][y][0],x,y])
	for i in listTempat:
		if i[1] == KANAN:
			cek = (cPos[0]+1,cPos[1])
		elif i[1] == KIRI:
			cek = (cPos[0]-1,cPos[1])
		elif i[1] == ATAS:
			cek = (cPos[0],cPos[1]-1)
		elif i[1] == BAWAH:
			cek = (cPos[0],cPos[1]+1)
		else:
			print("exception")
			cek = pos
		if cek not in ledakan and cek not in jangkauan and (mat[cek[1]][cek[0]] == JALAN or mat[i[3]][i[2]] in [SUPER,POWERBAG,POWERBOMB]) : # : # and isTembokSekitar((i[2],i[3]),mat)
			print(cek)
			return i[1]
	print("exception1")
	return cariTempatAman(pemain,mat,jangkauan,ledakan)



def adaJalanKeMusuh(pemain,lawan,mat):
	return generatePathUtil((pemain["X"],pemain["Y"]),(lawan["X"],lawan["Y"]),mat ) != None


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



def dekatiMusuh(pemain,musuh,mat,jangkauan,ledakan):
	langkah = mat[musuh["X"]][musuh["Y"]][0]
	if 	langkah == KANAN:
		cek = (pemain["X"]+1,pemain["Y"])
	elif langkah == KIRI:
		cek = (pemain["X"]-1,pemain["Y"])
	elif langkah == ATAS:
		cek = (pemain["X"],pemain["Y"]-1)
	elif langkah == BAWAH:
		cek = (pemain["X"],pemain["Y"]+1)
	else:
		print("exception")
		return cariTempatAman(player,Peta,jangkauan,ledakan)
	if cek not in jangkauan and cek not in ledakan:
		print(cek)
		return langkah
	else:
		print("exception2")
		return cariTempatAman(player,Peta,jangkauan,ledakan)

def jarak(A,B):
	dx = A[0] - B[0]
	dy = A[1] - B[1]
	return dx*dx + dy*dy

def cariTembok(pemain,mat,jangkauan,ledakan):
	pos = (pemain["X"],pemain["Y"])
	listTempat = []
	matJarak = generateMatriksJarak(pos,mat)
	for x in range(colSize+1):
		for y in range(rowSize+1):
			# print(x," ",y," " , matJarak[x][y][0], " ",matJarak[x][y][1])
			if matJarak[x][y][0] != INF and (x,y) not in jangkauan and (x,y) not in ledakan  and isTembokSekitar((x,y),mat):
				listTempat.append([matJarak[x][y][1],matJarak[x][y][0],x,y])
	
	tengah = int(colSize/2) + 1
	for i in listTempat:
		if (mat[tengah][tengah] == SUPER):
			i[0] = jarak((i[2],i[3]),(tengah,tengah)) 
	listTempat.sort()
	
	for i in listTempat:
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
		if cek not in jangkauan and cek not in ledakan and cek not in jangkauan and mat[cek[1]][cek[0]] == JALAN: # and isTembokSekitar((i[2],i[3]),mat)
			return i[1]
	return cariTempatAman(pemain,mat,jangkauan,ledakan)


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

###KAMUS
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

explode = []

#Inisialisasi player dan musuh
player = {"X": 0,"Y" : 0,"R" : 0 ,"BombBag" : 0}
musuh = {"X": 0,"Y" : 0,"R" : 0 ,"BombBag" : 0}

#list berisi item power up
item = []

#Untuk Generate Peta, p
getData(data,Matriks,Peta,item)

#berisi item terbaik dari daftar item
itemBagus = []
ambilItemBagus(item,Peta,itemBagus)

#list berisi (x,y) posisi jangkaun bomb
bombJangkauan = []
generateBombJangkauan(bombJangkauan,bombs,Peta)
	
def isAdaJalandariPlayer(target,MatJarak):
	return Mat[target[0]][target[1]][0] != INF and Mat[target[0]][target[1]][0] <= 17
###ALGORITMA
move = -1
currentPos = (player["X"] , player["Y"])
print("cur pos = ",currentPos)
Mat = generateMatriksJarak(currentPos,Peta)
if currentPos in bombJangkauan:
	print(">> Ada bom")
	move = cariTempatAman(player,Peta,bombJangkauan,explode) 
else:
	if itemBagus and isAdaJalandariPlayer((itemBagus[0][1],itemBagus[0][2]),Mat):
		print(">> Ambil item")
		print(itemBagus)
		move  = cariItem(player,(itemBagus[0][1],itemBagus[0][2]),Peta,bombJangkauan,explode)
	else: #tidak ada item yang bisa diambil
		if isAdaJalandariPlayer((musuh["X"],musuh["Y"]),Mat):
			 print(">> Mendekati Musuh")
			 print(Mat[musuh["X"]][musuh["Y"]])
			 move = dekatiMusuh(player,musuh,Mat,bombJangkauan,explode)
			 if isMusuhDisamping(player,musuh):
			 	move = 5
		else:
			print(">> Cari tembok")
			move = cariTembok(player,Peta,bombJangkauan,explode)
			if isTembokSekitar(currentPos,Peta):# and isSekitarAman(pemain,Peta,bombJangkauan):
				move = 5
print(">> Move = ",move)

with open(data_directory + '\\move.txt',"w+") as f:
		f.write('{}\n'.format(move))

