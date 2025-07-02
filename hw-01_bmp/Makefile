all: obj obj/main.o obj/bmp.o obj/stego.o hw-01_bmp

obj:
	mkdir obj

obj/main.o: src/main.c include/bmp.h include/stego.h
	gcc -c src/main.c -I include -o obj/main.o

obj/bmp.o: src/bmp.c include/bmp.h
	gcc -c src/bmp.c -I include -o obj/bmp.o

obj/stego.o: src/stego.c include/stego.h
	gcc -c src/stego.c -I include -o obj/stego.o

hw-01_bmp: obj/main.o obj/bmp.o obj/stego.o
	gcc obj/main.o obj/bmp.o obj/stego.o -o hw-01_bmp

clean:
	rm -rf obj
	rm hw-01_bmp
