CFLAGS = -Werror -O3

all: lib
	gcc $(CFLAGS) src/main.c build/lib/*.o -o build/main -lsqlite3 -lmd -luuid

lib:
	mkdir -p build/lib
	gcc $(CFLAGS) -c src/lib/database.c -o build/lib/database.o
	gcc $(CFLAGS) -c src/lib/fixtures.c -o build/lib/fixtures.o
	gcc $(CFLAGS) -c src/lib/user.c     -o build/lib/user.o
	gcc $(CFLAGS) -c src/lib/utils.c     -o build/lib/utils.o

clean:
	rm -rf .cache api build compile_commands.json librarize.db

vg:
	valgrind --leak-check=full --show-leak-kinds=all ./build/main

check:
	cppcheck --language=c --enable=all --template=gcc --suppress=missingIncludeSystem .

bear:
	bear -- make