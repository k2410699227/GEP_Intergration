import os
from random import random
round = 100

input = open("input.txt","w+")
output = open("output.txt","w+")

for t in range(0,round):
    i = random()*10-5
    print(i,file=input)
    print(i**3+i**2+i+1,file=output)
 
