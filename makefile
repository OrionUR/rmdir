objects = rmdir.o rmdir_test.o

test: $(objects)
	gcc $(objects) -o test

$(objects): rmdir.h

rmdir.o: rmdir.c

rmdir_test.o: rmdir_test.c

clean:
	rm test $(objects)
