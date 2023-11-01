CFLAGS = -Werror -O3 -lsqlite3 -lmd -luuid

all: lib api
	gcc $(CFLAGS) src/setup.c build/lib/*.o               -o build/setup
	gcc $(CFLAGS) src/tests.c build/lib/*.o build/api/*.o -o build/tests
	./build/setup

lib:
	mkdir -p build/lib
	gcc $(CFLAGS) -c src/lib/database.c -o build/lib/database.o
	gcc $(CFLAGS) -c src/lib/fixtures.c -o build/lib/fixtures.o
	gcc $(CFLAGS) -c src/lib/user.c     -o build/lib/user.o
	gcc $(CFLAGS) -c src/lib/utils.c    -o build/lib/utils.o

api:
	rm -rf api
	mkdir api
	mkdir build/api
	gcc $(CFLAGS) -c src/api/user.c                             -o build/api/user.o
	gcc $(CFLAGS)    src/user.c build/lib/*.o build/api/*.o -o api/user.cgi

clean:
	rm -rf .cache api build compile_commands.json librarize.db

vg:
	valgrind --leak-check=full --show-leak-kinds=all ./build/tests

check:
	cppcheck --language=c --enable=all --template=gcc --suppress=missingIncludeSystem .

bear:
	bear -- make