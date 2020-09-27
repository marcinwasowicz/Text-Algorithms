from Queue import PriorityQueue
from copy import deepcopy
from bitarray import bitarray
import sys as System
from sortedcontainers import SortedDict
import time

def topOfMap(Map):
    if len(Map) == 0:
        return None
    return Map.peekitem() 

def putToDictOfMaps(el, D):
    if el.weight not in D:
        D[el.weight] = SortedDict();
    D[el.weight][el.index] = el

class Node:
    def __init__(self,weight, index, letter,parent, left, right):
        self.weight = weight
        self.index = index
        self.letter = letter
        self.parent = parent
        self.left = left
        self.right = right
    
    def splitNYT(self,letter):
        self.left = Node(0, self.index-2,'_',self, None, None)
        self.right = Node(1, self.index -1,letter, self, None, None)

    def isLeaf(self):
        return self.left is None and self.right is None

def increment(node, weightClasses):
    if node is None:
        return
    elif node == topOfMap(weightClasses[node.weight])[1]:
        tempDict = weightClasses[node.weight]
        del tempDict[node.index]
        node.weight += 1
        putToDictOfMaps(node, weightClasses)
        increment(node.parent, weightClasses)
    else:
        currMax = weightClasses[node.weight].peekitem()[1]
        tempDict = weightClasses[node.weight]
        del tempDict[node.index]
        del tempDict[currMax.index]
        temp = node.index
        node.index = currMax.index
        currMax.index = temp
        node.weight += 1
        putToDictOfMaps(node, weightClasses)
        putToDictOfMaps(currMax, weightClasses)
        parentNode = node.parent
        parentCurr = currMax.parent
        if parentNode.left == node:
            parentNode.left = currMax
        else:
            parentNode.right = currMax
        if parentCurr.left == currMax:
            parentCurr.left = node
        else:
            parentCurr.right = node
        node.parent = parentCurr
        currMax.parent = parentNode

        increment(node.parent, weightClasses)

def getLetterCode(char, root, letters, nyt):
    node = None
    if char not in letters:
        node = nyt
    else:
        node = letters[char]

    result = ''
    while node.parent is not None:
        if node.parent.left == node:
            result  = '0' + result
        else:
            result = '1' + result
        node = node.parent

    result = bitarray(result)
    if char not in letters:
        temp = bitarray()
        temp.frombytes(char.encode('utf-8'))
        result += temp
    return result

def encodeToBitaray(fileName, alf):
    root = Node(0, 2*alf-1, '_', None, None, None)
    nyt = root
    lettersDict = dict()
    weightClasses = dict()
    putToDictOfMaps(root, weightClasses)
    file = open(fileName, "r")
    char = file.read(1)
    encodedText = bitarray()
    while char:
        if char not in lettersDict:
            encodedText += getLetterCode(char, root, lettersDict, nyt)
            nyt.splitNYT(char)
            node = nyt.right
            nyt = nyt.left
            putToDictOfMaps(nyt, weightClasses)
            lettersDict[char] = node
            putToDictOfMaps(node, weightClasses)
            node = node.parent
            increment(node, weightClasses)
        else:
            encodedText += getLetterCode(char, root, lettersDict, nyt)
            node = lettersDict[char]
            tempDict = weightClasses[node.weight]
            del tempDict[node.index]
            node.weight += 1
            putToDictOfMaps(node, weightClasses)
            node = node.parent
            increment(node, weightClasses)
        char = file.read(1)
    file.close()
    return encodedText

def decodeFromBitarray(barray, alf):
    root = Node(0, 2*alf-1, '_', None, None, None)
    nyt = root
    lettersDict = dict()
    weightClasses = dict()
    putToDictOfMaps(root, weightClasses)
    temp = root
    readingNewLetter = True
    i = 0
    newLetter = ''
    decodedText = ''
    for bit in barray:
        if readingNewLetter:
           if i == 7:
               i = 0
               if bit:
                   newLetter += '1'
               else:
                   newLetter += '0'
               readingNewLetter = False
               letter = bitarray(newLetter).tobytes()
               decodedText += letter
               nyt.splitNYT(letter)
               node = nyt.right
               nyt = nyt.left
               weightClasses[0][nyt.index] = nyt
               lettersDict[letter] = node
               putToDictOfMaps(node, weightClasses)
               node = node.parent
               increment(node, weightClasses)
               newLetter = ''
           else:
                if bit:
                   newLetter += '1'
                else:
                    newLetter += '0'
                i += 1
        else:
            if bit:
                temp = temp.right
            else:
                temp = temp.left
            if temp.isLeaf():
                if temp == nyt:
                    readingNewLetter = True
                else:
                    decodedText += temp.letter
                    node = lettersDict[temp.letter]
                    tempDict = weightClasses[node.weight]
                    del tempDict[node.index]
                    node.weight += 1
                    putToDictOfMaps(node, weightClasses)
                    node = node.parent
                    increment(node, weightClasses)
                temp = root
    return decodedText

def runOnFile(fileName, alf):
    start = time.time()
    barray = encodeToBitaray(fileName, alf)
    end = time.time()
    print("endoding took ", (end - start))
    print("compressed size, ", len(barray))
    start = time.time()
    decodedText = decodeFromBitarray(barray, alf)
    end = time.time()
    print("decoding took ", (end - start))

def main():
    runOnFile(System.argv[1], int(System.argv[2]))
main()