from tokenize import Double
import matplotlib.pyplot as plt
import os
import numpy
import argparse

duration = []
accuracy = []
round = 1
acc = ''
parser = argparse.ArgumentParser(description="your script description")        

parser.add_argument('-r', type= int)
parser.add_argument('-a', type= float)
args = parser.parse_args()
if args.r:
    round = args.r
if args.a:
    acc = args.a


for i in range(0, round):
    print(i+1, end='\n')
    lines = os.popen('./GeneExpressionProgramming -a '+str(acc)).readlines()
    print(lines[1])
    accuracy.append(eval(lines[-1].split()[-1].split('%')[0])/100)
    duration.append(eval(lines[-3].split()[-1]))

order = numpy.arange(1, round+1)
print(accuracy, duration,sep='\n')
plt.plot(order, accuracy)
plt.title('Accuracy of test')
plt.savefig('accuracy-{}-{}.png'.format(acc,round))

plt.close()
plt.plot(order, duration)
plt.title('Duration of test')
plt.savefig('duration-{}-{}.png'.format(acc,round))
