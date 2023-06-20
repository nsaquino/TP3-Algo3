import numpy as np  ##/*ctrl+p luego -> Python: Select Interpreter -> Python 3.10.6
import os

np.random.seed(1337)

dir_path = os.path.dirname(os.path.realpath(__file__))
final_path = os.path.join(dir_path, 'inputs')
if not os.path.exists(final_path):
   os.makedirs(final_path)

limit = 10**4
for pow in range (1,5): #[1,5)
  for x in range(1,10): #[1,10)
#for n in range (10, 1001, 10):
    n = x * (10**pow)
    if n > limit:
       break
    f = open(final_path + "/input_" + str(n), "w")
    print(final_path + "/input_" + str(n)) ####debug####
    
    m = np.random.randint(1, 1 + min((n*(n-1))/2, 10**5))
    k = np.random.randint(1, 300)
    s = np.random.randint(1, n+1)
    t = np.random.choice(np.setdiff1d(range(1,n+1), s))
    f.write(str(n) + ' ' + str(m) + ' ' + str(k) + ' ' + str(s) + ' ' + str(t) + "\n")

    conexiones = set()
    while len(conexiones) < m:
        u = np.random.randint(1, n+1)
        v = np.random.choice(np.setdiff1d(range(1,n+1), u))
        l = np.random.randint(0, 1001)
        if not ((u, v, l) in conexiones or (v, u, l) in conexiones):
            conexiones.add((u,v,l))
    
    candidatos = set()
    while len(candidatos) < k:
        u = np.random.randint(1, n+1)   
        v = np.random.choice(np.setdiff1d(range(1,n+1), u))
        l = np.random.randint(0, 1001)
        candidatos.add((min(u,v), max(u,v), l))
    
    for c in conexiones:
        u, v, l = c
        f.write(str(u) + ' ' + str(v) + ' ' + str(l) + '\n')
    
    for c in candidatos:
        u, v, l = c
        f.write(str(u) + ' ' + str(v) + ' ' + str(l) + '\n')

    f.close()