#!usr/bin/python
import sys
import numpy as np
# python3 Patch_Bezier <slices> <patch> <ponto.3d>
#print(sys.argv[1])
slices =int( sys.argv[1])
#slices = 10
nome_ficheiro = sys.argv[2]
#nome_ficheiro = "teapot.patch"
ponto3d = sys.argv[3]

fd = open(nome_ficheiro,'r')
n_Patch = int(fd.readline())
#print(n_Patch)

patches = []
for x in range(n_Patch):
    patches.append(list(map(lambda x: int(x) ,fd.readline().strip('\n').split(','))))
#print(patches)

n_pontos = int(fd.readline())
#print(n_pontos)

pontos = []
for x in range(n_pontos):
    pontos.append(list(map(lambda x: float(x) ,fd.readline().strip('\n').split(','))))
#print(pontos)
P = [[0, 0, 0, 0],
     [0, 0, 0, 0],
     [0, 0, 0, 0],
     [0, 0, 0, 0]]

for p in range(4):
    for x in range(4):
        P[p][x] = (pontos[ patches[p][x] ][0],pontos[ patches[p][x] ][1],pontos[ patches[p][x] ][2],1)
M = [[-1, 3, -3, 1],
     [3, -6, 3, 0 ],
     [-3, 3, 0, 0 ],
     [1,  0, 0, 0 ]]
aux = np.array(M)
MT =aux.transpose()
#MxP = np.matmul(M,P)
#MxPxMT = np.matmul(MxP,MT)
##print(P)
def multME(vector,pontos):
    res = [[0, 0, 0, 0],
          [0, 0, 0, 0],
          [0, 0, 0, 0],
          [0, 0, 0, 0]]
    for x in range(4):
        for j in range(len(pontos)):
            for i in range(j):
                res[i][x] += pontos[j][i][x]*vector[x] 
            
#    print("------")
#    print(vector)
#    print(pontos)
#    print(res)
    return res
            

#a = [1,2,3,4]
#b = [[3,4,5,2],[3,4,5,1],[2,8,3,3],[3,5,6,1]]
#print(np.matmul(a,b))
def B(u,v):
    U = [u**3, u**2, u, 1]
    V = [[v**3],
         [v**2],
         [v   ],
         [1   ]]
    UM = np.matmul(U,M)
#    print(UM)
    UMP = multME(UM,P)
#    print(UMP)
#    UMP = np.matmul(UM,P)
    UMPMT =np.matmul(UMP,MT)
    UMPMTV = np.matmul(UMPMT,V)
    return(UMPMTV)
fd2 = open(ponto3d,'a')
u = 0
v = 0
while(u<1):
    while(v<1):
        print(B(u,v))
        x = B(u,v)[0][0]
        y = B(u,v)[1][0]
        z = B(u,v)[2][0]
        string = str(x)+' '+str(y)+' '+str(z)+'\n'
        fd2.write(string)
        x = B(u+1/slices,v)[0][0]
        y = B(u+1/slices,v)[1][0]
        z = B(u+1/slices,v)[2][0]
        string = str(x)+' '+str(y)+' '+str(z)+'\n'
        fd2.write(string)
        x = B(u,v+1/slices)[0][0]
        y = B(u,v+1/slices)[1][0]
        z = B(u,v+1/slices)[2][0]
        string = str(x)+' '+str(y)+' '+str(z)+'\n'
        fd2.write(string)


        x = B(u+1/slices,v)[0][0]
        y = B(u+1/slices,v)[1][0]
        z = B(u+1/slices,v)[2][0]
        string = str(x)+' '+str(y)+' '+str(z)+'\n'
        fd2.write(string)
        x = B(u+1/slices,v+1/slices)[0][0]
        y = B(u+1/slices,v+1/slices)[1][0]
        z = B(u+1/slices,v+1/slices)[2][0]
        string = str(x)+' '+str(y)+' '+str(z)+'\n'
        fd2.write(string)
        x = B(u,v+1/slices)[0][0]
        y = B(u,v+1/slices)[1][0]
        z = B(u,v+1/slices)[2][0]
        string = str(x)+' '+str(y)+' '+str(z)+'\n'
        fd2.write(string)
        v += 1/slices

    u += 1/slices
#        print( str((u,v)[0])++str( B(u,v)[1])++str( B(u,v)[2]))

    
