import os
from random import random
import numpy as np
import matplotlib.pyplot as plt
round = 100

inputFile = open("input.txt","r+")
outputFile = open("output.txt","r+")
prediction = []
groundTruth = []
# for t in range(0,round):
#     i = random()*10-5
#     print(i,file=input)
#     print(i**3+i**2+i+1,file=output)
 
inputData = inputFile.readlines()
outputData = outputFile.readlines()
for i in range(80,90):
    split = inputData[i].split()
    prediction.append(eval(split[-1]) * 2 - eval(split[-2]))
    # prediction.append(eval(split[-1]))
    groundTruth.append(eval(outputData[i].strip()))

xAxis = np.arange(1,11,1)
plt.plot(xAxis,prediction,label = "prediction")
plt.plot(xAxis,groundTruth,label = "actual")
plt.legend()
plt.show()