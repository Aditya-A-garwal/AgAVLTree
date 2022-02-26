import random
import sys

f               = open ('random_all.in', 'w')
sys.stdout      = f

n               = int (2e7)
step            = int (1e6)

lst             = []

for i in range (0, n, step):
    lst.append ([i for i in range(i, i + step)])

print (n)

for i in lst:
    random.shuffle (i)
    for j in i:
        print (j)

for i in lst:
    random.shuffle (i)
    for j in i:
        print (j)

for i in lst:
    random.shuffle (i)
    for j in i:
        print (j)
