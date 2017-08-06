#zipffy: src/zipffy.c
#	gcc -o dist/zipffy src/zipffy.c -I.

zipffy : zipffy.o
				gcc -o dist/zipffy zipffy.o

zipffy.o : src/zipffy.c
				gcc -c src/zipffy.c

clean :
				rm dist/zipffy zipffy.o
