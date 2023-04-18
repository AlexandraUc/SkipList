f = open("abce.in")
g = open("abce2.out", "w")

L = []
q = int(f.readline())

for i in range(q):
    linie = [int(x) for x in f.readline().split()]

    op = linie[0]
    if op == 1:
        L.append(linie[1])

    if op == 2:
        if linie[1] in L:
            L.remove(linie[1])

    if op == 3:
        if linie[1] in L:
            g.write("1\n")
        else:
            g.write("0\n")

    if op == 4:
        L.sort()
        i = 0
        while i < len(L) and L[i] <= linie[1]:
            i += 1
        g.write(str(L[i - 1]) + "\n")

    if op == 5:
        L.sort()
        i = 0
        while i < len(L) and L[i] < linie[1]:
            i += 1
        g.write(str(L[i]) + "\n")

    if op == 6:
        L.sort()
        for x in L:
            if linie[1] <= x <= linie[2]:
                g.write(str(x) + " ")
        g.write("\n")

f.close()
g.close()