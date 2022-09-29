import os

originFile = open('iris.data', 'r+')
trainFile = open("train.txt", "w+")
evaluationFile = open("evaluation.txt", "w+")

originData = originFile.readlines()
classes = dict({'Iris-setosa': 0, 'Iris-versicolor': 1, 'Iris-virginica': 0})
for i, o in enumerate(originData):
    line = o.strip().split(',')

    if (i + 1) % 5 == 0:
        print(line[0], line[1], line[2], line[3], classes[line[4]], file=evaluationFile)
    else:
        print(line[0], line[1], line[2], line[3], classes[line[4]], file=trainFile)