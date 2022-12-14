

origin = open("Algerian_forest_fires_dataset_UPDATE.csv", 'r+')
trainFile = open("train.txt", "w+")
evaluationFile = open("evaluation.txt", "w+")
classes = dict({'fire': 1, 'not fire': 0})

for i, o in enumerate(origin.readlines()):
    line = o.strip().split(',')
    if (i + 1) % 4 == 0:
        print(' '.join(line[3:-1]), classes[line[-1]], file=evaluationFile)
    else:
        print(' '.join(line[3:-1]), classes[line[-1]], file=trainFile)
        
