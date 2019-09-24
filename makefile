objects = rmdir.o rmdir_test.o

test: $(objects)
	gcc $(objects) -o test

rmdir.o: rmdir.c rmdir.h
	gcc -c rmdir.c

rmdir_test.o: rmdir_test.c rmdir.h
	gcc -c rmdir_test.c

clean:
	rm -f test $(objects)
