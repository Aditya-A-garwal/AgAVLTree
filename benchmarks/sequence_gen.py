import sys

f               = open ('sequence_all.in', 'w')
sys.stdout      = f

n               = int (2e7)

print (n)

for i in range (n):
    print (i)

for i in range (n):
    print (i)

for i in range (n):
    print (i)

f.close ()
