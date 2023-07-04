import numpy as np
import random,cv2
from PIL import Image
def img():
    data = cv2.imread('baijingting1.png')
    image = cv2.resize(data, dsize=(200,200), fx=1, fy=1, interpolation=cv2.INTER_LINEAR)

    mat= np.array(image)
    r,g,b = [image[:,:,i] for i in range(3)]
    img_gray = r*0.299+g*0.587+b*0.114#灰度处理
    image2 = Image.fromarray(img_gray)
    image2.show()
    a=1
    b=1
    N=200
    an=np.zeros([200,200])
    for i in range(0,150):#200*200矩阵150轮变换复原
        for y in range(0,200):
            for x in range(0,200):#变换每个像素点
                xx=(a*x+b*y)%N
                yy=(a*x+(a*b+1)*y)%N
                an[xx][yy]=img_gray[x][y]
        img_gray[:][:]=an[:][:]
        print(i)
        if (i+1)%50==0:#每50轮输出一次变换图像
            image2 = Image.fromarray(img_gray)
            image2.show()

if __name__=="__main__":
    img()
