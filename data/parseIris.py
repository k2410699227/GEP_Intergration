import os

originFile = open('iris.data', 'r+')
inputFile = open("input.txt", "w+")
outputFile = open("output.txt", "w+")
evaluationFile = open("evaluation.txt", "w+")

originData = originFile.readlines()
classes = dict({'Iris-setosa': 1, 'Iris-versicolor': 0, 'Iris-virginica': 0})
for i, o in enumerate(originData):
    line = o.strip().split(',')

    if (i + 1) % 5 == 0:
        print(line[0], line[1], line[2], line[3], classes[line[4]], file=evaluationFile)
    else:
        print(line[0], line[1], line[2], line[3], file=inputFile)
        print(classes[line[4]], file=outputFile)