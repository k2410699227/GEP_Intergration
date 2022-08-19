import os

evaluationFile = open('evaluation.txt','r+')
count = 0
for e in evaluationFile.readlines():
    indep = [eval(k) for k in e.strip().split()]
    dependent = indep[4]
    a,b,c,d = indep[0:4]
    # print(a,b,c,d)
    result = (((b-d)/(c*b))/(c/a)) + (((b+d)/b)-(c*c)) + ((d-b)/(b+d)) + ((a-(b-c))-c) + (a-(d/(a*a)))
    # result = indep[1]-indep[2]
    # print(result)
    if result > 0.5 and dependent == 1:
        count += 1
    elif result <= 0.5 and dependent == 0:
        count += 1
print(count)