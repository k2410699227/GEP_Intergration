import matplotlib.pyplot as plt
import os,numpy

duration = []
accuracy = []
round = 100

for i in range(0,round):
    print(i+1,end='\n')
    lines = os.popen('./GeneExpressionProgramming').readlines()
    accuracy.append(eval(lines[-1].split()[-1].split('%')[0])/100)
    duration.append(eval(lines[-3].split()[-1]))

order = numpy.arange(1,round+1)

plt.plot(order,accuracy)
plt.title('Accuracy of test')
plt.savefig('accuracy-{}.png'.format(round))

plt.close()
plt.plot(order,duration)
plt.title('Duration of test')
plt.savefig('duration-{}.png'.format(round))
