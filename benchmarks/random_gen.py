
# Generate 27 random numbers each between -1e8 and 1e8 to be inserted, searched for and erased
import random

a = int (2e7)
b = int (2e7)
c = int (2e7)

lo = -int (1e8)
hi = int (1e8)

print (a, b, c)

for i in range (a):
    print (random.randint (lo, hi))

for i in range(b):
    print (random.randint (lo, hi))

for i in range(c):
    print (random.randint (lo, hi))
