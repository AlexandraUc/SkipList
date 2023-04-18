f = open("abce.out")
g = open("abce2.out")

linie1 = f.read().split()
linie2 = g.read().split()

if linie1 == linie2:
    print("egale\n")
else:
    print("diferite\n")

f.close()
g.close()