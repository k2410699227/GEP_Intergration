import os

evaluationFile = open('evaluation.txt','r+')
count = 0
invalid = 0
for line in evaluationFile.readlines():
    indep = [eval(k) for k in line.strip().split()]
    dependent = indep[-1]
    a,b,c,d,e,f,g,h,i,j = indep[0:10]
    
    try:
        result = (j/j) + ((i/e)+j) + (j-c) + (h*j) + (h-j)

        if result > 0.5 and dependent == 1:
            count += 1
        elif result <= 0.5 and dependent == 0:
            count += 1
    except ZeroDivisionError:
        invalid += 1
        print(' '.join(line.strip().split()))

print("预测正确样本数：",count,"无效样本数：",invalid)