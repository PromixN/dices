import os
import re

of = open("out.txt")
count = 0
tem = 1
pos = 0
faces=face = dot = []
num = [['','','8']]
num +=re.findall('(DONE\n)(\d+)\n(\d+)',of.read())
print(num)
of = open("out.txt")
for line in of.readlines():
    if(re.match("::::::",line)):
        if(tem > 1):
            print("face:")

            print(faces)
            print("dot:")

            print(dot)
        face=[0]*(int(num[tem-1][2])+2)
        faces=[]
        dot=[0]*int(num[tem][2])
        tem +=1
    if(re.match("DONE",line)):
        if(tem<4):
            continue
        else:
            break
    if(re.match(":::",line)):
        continue
    if(re.match("f:",line)):
        face[int(re.search('\d+',line).group())]+=1
    if(re.match("d:",line)):
        dot[int(re.search('\d+',line).group())]+=1
    if(re.match("face:",line)):
        faces.append(re.search('(\d+):(\d+):(\d+):(\d+):',line).group())
print("face:")

print(faces)
print("dot:")

print(dot)