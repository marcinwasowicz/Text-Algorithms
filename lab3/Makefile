programs:
	g++ StaticHuffman.cpp -o statHuff
	g++ AdaptiveHuffman.cpp -o adaptHuff
Test:
	./statHuff $(path)
	make clean
	./adaptHuff $(path)
	make clean
clean:
	rm compressed
	rm decompressed
	rm huffmanTree
cleanPrograms:
	rm statHuff
	rm adaptHuff