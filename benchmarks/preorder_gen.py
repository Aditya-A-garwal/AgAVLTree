import sys

f               = open ('preorder_all.in', 'w')
sys.stdout      = f

n               = int (2e7)

lst = [[i, i] for i in range (n)]

def dnc (lo, hi, lvl):
    if lo > hi:
        return

    mid = lo + ((hi - lo + 1)>>1)
    lst[mid][0] = lvl
    lst[mid][1] = mid
    dnc (lo, mid - 1, lvl + 1)
    dnc (mid + 1, hi, lvl + 1)

dnc (0, n - 1, 0)
lst.sort ()

print (n)

for ff, ss in lst:
    print (ss)

for ff, ss in lst:
    print (ss)

for ff, ss in lst[::-1]:
    print (ss)
