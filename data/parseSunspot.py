origin = [101, 82, 66, 35, 31, 7, 20, 92,
          154, 125, 85, 68, 38, 23, 10, 24,
          83, 132, 131, 118, 90, 67, 60, 47,
          41, 21, 16, 6, 4, 7, 14, 34,
          45, 43, 48, 42, 28, 10, 8, 2,
          0, 1, 5, 12, 14, 35, 46, 41,
          30, 24, 16, 7, 4, 2, 8, 17,
          36, 50, 62, 67, 71, 48, 28, 8,
          13, 57, 122, 138, 103, 86, 63, 37,
          24, 11, 15, 40, 62, 98, 124, 96,
          66, 64, 54, 39, 21, 7, 4, 23,
          55, 94, 96, 77, 59, 44, 47, 30,
          16, 7, 37, 74
          ]
input = open('input.txt','w+')
output = open('output.txt','w+')
leftIndex = 0
rightIndex = 10
while(True):
    if(rightIndex==100):
        break
    # print(origin[leftIndex:rightIndex],'  ',origin[rightIndex], '\n')
    for o in origin[leftIndex:rightIndex]:
        print(o,file= input,end=' ')
    print('\n',file= input,end='')
    print(origin[rightIndex],file=output)
    leftIndex = leftIndex + 1
    rightIndex = rightIndex + 1