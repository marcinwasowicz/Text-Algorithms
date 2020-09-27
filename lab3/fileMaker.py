import sys
import string
import random

fileName = sys.argv[1]
size = int(sys.argv[2])

file = open(fileName, "w+")

for i in range(size):
    file.write(random.choice(string.ascii_letters))
file.close()

