CFLAGS = -std=c17 -pedantic -Wall -Wextra -Wformat-security -Wduplicated-cond\
         -Wfloat-equal -Wshadow -Wconversion -Wjump-misses-init\
         -Wlogical-not-parentheses -Wnull-dereference -Wvla -Werror\
         -fstack-protector-strong -fsanitize=undefined -fno-sanitize-recover\
         -g -fno-omit-frame-pointer -O1

test: build/test.o build/ary.o
	@gcc $(CFLAGS) build/test.o build/ary.o -o test

tester_franka: build/tester_franka.o build/ary.o
	@gcc $(CFLAGS) build/tester_franka.o build/ary.o -o tester_franka

build/test.o: test.c
	@gcc $(CFLAGS) -c test.c -o build/test.o

build/tester_franka.o: tester_franka.c
	@gcc $(CFLAGS) -c tester_franka.c -o build/tester_franka.o

build/ary.o: ary.c ary.h
	@gcc $(CFLAGS) -c ary.c -o build/ary.o