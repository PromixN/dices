import matplotlib.pyplot as pyt
import skimage.io as io
import skimage.transform as trans
import numpy as np
import random
scale = 1024
a=[1,2,3,4,5,6]

def rand():
    k = random.random()
    if k<0.125:
        return -1,-1
    elif k<0.25:
        return -1,0
    elif k<0.375:
        return -1,1
    elif k<0.5:
        return 0,-1
    elif k < 0.625:
        return 1,-1
    elif k < 0.75:
        return 0,1
    elif k<0.875:
        return 1,0
    else:
        return 1,1


for i in range(0,6):
    tem = np.zeros((scale, scale, 3))
    temm = np.zeros((scale,scale,3))
    for j in range(0,5000):
        temm[int(random.random()*scale)][int(random.random()*scale)][0]=0.4
    for j in range(0,scale):
        for k in range(0,scale):
            if(temm[j][k][0]>0):
                for n in range(0,int(random.random()*8)):
                    x,y=rand()
                    if j+x ==scale or j+x<0:
                        x=0
                    if k+y ==scale or k+y<0:
                        y=0
                    temm[j+x][k+y][0]+=0.08
    tem[:,:,0]=temm[:,:,0]
    tem[:,:,1]=tem[:,:,0]
    tem[:,:,2]=tem[:,:,0]

    pyt.imshow(tem)
    pyt.show()
    io.imsave('./texture/noise_%d.jpg'% a[i], tem)

