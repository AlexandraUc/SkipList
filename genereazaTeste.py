import random

f = open("abce2.in", "w")
q = 100000
L1 = []
op6 = 0

f.write(str(q) + "\n")

x = random.randint(-1000000000, 1000000000)
f.write("1 " + str(x) + "\n")
L1.append(x);
q -= 1

x = random.randint(-1000000000, 1000000000)
if x not in L1:
    f.write("1 " + str(x) + "\n")
    L1.append(x)
    q -= 1

while q:
    operatie = random.randint(1, 6)

    if operatie == 1:
        x = random.randint(-1000000000, 1000000000)
        if x not in L1:
            f.write("1 " + str(x) + "\n")
            L1.append(x)
            q -= 1

    if operatie == 2:
        k = random.randint(0, 1)
        if k == 0:
            x = random.randint(-1000000000, 1000000000)
            f.write("2 " + str(x) + "\n")
            q -= 1
        elif len(L1):
            x = L1[random.randint(0, len(L1)-1)]
            f.write("2 " + str(x) + "\n")
            L1.remove(x)
            q -= 1

    if operatie == 3:
        k = random.randint(0, 1)
        if k == 0:
            x = random.randint(-1000000000, 1000000000)
            f.write("3 " + str(x) + "\n")
            q -= 1
        elif len(L1):
            x = L1[random.randint(0, len(L1) - 1)]
            f.write("3 " + str(x) + "\n")
            q -= 1

    if operatie == 4 and len(L1):
        x = L1[random.randint(0, len(L1)-1)]
        k = random.randint(0, 1)
        if k == 0:
            f.write("4 " + str(x) + "\n")
            q -= 1
        else:
            x += 1
            f.write("4 " + str(x) + "\n")
            q -= 1

    if operatie == 5 and len(L1):
        x = L1[random.randint(0, len(L1) - 1)]
        k = random.randint(0, 1)
        if k == 0:
            f.write("5 " + str(x) + "\n")
            q -= 1
        else:
            x -= 1
            f.write("5 " + str(x) + "\n")
            q -= 1

    if operatie == 6 and op6 < 50 and len(L1) >= 2:
        op6 += 1
        x = L1[random.randint(0, len(L1)-1)]
        y = L1[random.randint(0, len(L1) - 1)]
        while x == y:
            y = L1[random.randint(0, len(L1) - 1)]

        if x > y:
            x, y = y, x

        k = random.randint(1, 4)
        if k == 1:
            pass
        if k == 2:
            x -= 100
        if k == 3:
            y += 100
        if k == 4:
            x -= 200
            y += 200

        f.write("6 " + str(x) + " " + str(y) + "\n")
        q -= 1