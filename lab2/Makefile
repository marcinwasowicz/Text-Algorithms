Trie:
	g++ -c TrieNode.cpp
	g++ -c Trie.cpp
cleanTrie:
	rm Trie*.o
main:
	g++ main.cpp TrieNode.o Trie.o -o main
all:
	make Trie
	make main
cleanMain:
	rm main
clean:
	make cleanMain
	make cleanTrie
run:
	./main