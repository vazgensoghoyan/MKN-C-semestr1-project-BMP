all: obj obj/main.o obj/bmp.o hw-01_bmp

obj:
	mkdir obj

obj/main.o: src/main.c
	gcc -c src/main.c -I include -o obj/main.o

obj/bmp.o: src/bmp.c
	gcc -c src/bmp.c -I include -o obj/bmp.o

hw-01_bmp: obj/main.o obj/bmp.o
	gcc obj/main.o obj/bmp.o -o hw-01_bmp

clean:
	rm -rf obj
	rm hw-01_bmp
