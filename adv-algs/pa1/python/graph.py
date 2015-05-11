import os
import random

n = 10000
m = 1000
f = open('rpath' + str(n) + '.txt','w')
g = {}
for i in range(0, n-1):
    g[i] = [i+1]
g[n-1] = [n-3]
g[n] = [n - 3]
g[n+1] = [n - 3]
for i in range(0, m):
    source = random.randint(0, n-1)
    sink = random.randint(0, n-1)
    g[source].append(sink)
for i in g.keys():
    t = ""
    for j in g[i]:
        t += str(j) + ' 1 '
    t = t[0:len(t)-1]
    f.write(t + '\n')
f.close()