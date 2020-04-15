programs:
	g++ StaticHuffman.cpp -o statHuff
Test:
	./statHuff $(path)
clean:
	rm compressed
	rm decompressed
	rm huffmanTree
cleanPrograms:
	rm statHuff