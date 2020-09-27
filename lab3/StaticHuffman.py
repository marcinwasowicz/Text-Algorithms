from Queue import PriorityQueue
from copy import deepcopy
from bitarray import bitarray
import sys as System
import time

class Node:
    def __init__(self, weight, letter, left, right):
        self.weight = weight
        self.letter = letter
        self.left = left
        self.right = right
    
    def isLeaf(self):
        return self.left == None and self.right == None

def createWeightDictionary(fileName):
    D = dict()
    file = open(fileName, "r")
    char = file.read(1)
    while char:
        if char not in D:
            D[char] = 1
        else:
            D[char] += 1
        char = file.read(1)
    file.close()
    return D

def createHuffmanTree(weightDictionary):
    uniqueID = 0
    Q = PriorityQueue()

    for key, weight in weightDictionary.items():
        node = Node(weight, key, None, None)
        Q.put((weight, uniqueID, node))
        uniqueID += 1
    
    while Q.qsize() > 1:
        tup1 = Q.get()
        tup2 = Q.get()
        weight = tup1[0] + tup2[0]
        node = Node(weight, '_', tup1[2], tup2[2])
        Q.put((weight, uniqueID, node))
        uniqueID += 1

    return Q.get()[2]

def createCodingDictionary(root, resultDict,workingString):
    if root.isLeaf():
        resultDict[root.letter] = deepcopy(workingString)
        return
    
    workingString += '0'
    createCodingDictionary(root.left, resultDict, workingString)
    workingString = workingString[:-1]
    workingString += '1'
    createCodingDictionary(root.right, resultDict, workingString)
    workingString = workingString[:-1]

def encodeToBitarray(fileName, codingDictionary):
    file = open(fileName, 'r')
    res = bitarray()
    char = file.read(1)
    while char:
        res += bitarray(codingDictionary[char])
        char = file.read(1)
    return res

def decodeFromBitarray(root, barray):
    decodedText = ''
    temp = root
    for bit in barray:
        if bit:
            temp = temp.right
        else:
            temp = temp.left
        if temp.isLeaf():
            decodedText += temp.letter
            temp = root
    return decodedText

def staticHuffman(fileName):
    start = time.time()
    weightDictionary = createWeightDictionary(fileName)
    root = createHuffmanTree(weightDictionary)
    codingDictionary = dict()
    workingString = ''
    createCodingDictionary(root, codingDictionary,workingString)
    barray = encodeToBitarray(fileName, codingDictionary)
    end = time.time()
    print("encoding took ",(end - start))
    print("compressed size, ", len(barray))
    start = time.time()
    decodedText = decodeFromBitarray(root, barray)
    end = time.time()
    print("decoding took ", (end - start))
    
def main():
    staticHuffman(System.argv[1])
main()