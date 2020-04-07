file = open("test.txt", "w+")
text = ("a"*30000+"b")*100
file.write(text)
file.close()