import os

evaluationFile = open('evaluation.txt','r+')
count = 0
invalid = 0
for line in evaluationFile.readlines():
    indep = [eval(k) for k in line.strip().split()]
    dependent = indep[-1]
    # a,b,c,d,e,f,g,h,i,j = indep[0:10]   #Forest_fire
    a,b,c,d = indep[0:4]    #Iris
    
    try:
        result = (d-((a+c)*(c+c))) + b + (((c-a)*a)*(c-d)) + c + (a*(d/d))

        if result > 0.5 and dependent == 1:
            count += 1
        elif result <= 0.5 and dependent == 0:
            count += 1
    except ZeroDivisionError:
        invalid += 1
        print(' '.join(line.strip().split()))

print("预测正确样本数：",count,"无效样本数：",invalid)