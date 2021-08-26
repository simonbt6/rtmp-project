import sys

_input = sys.argv[1]

print()

for i in range(len(_input)):
    print(_input[i], end="")
    if (i + 1) % 2 == 0: print(" ", end="")

print()