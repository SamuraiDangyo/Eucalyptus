all:
	clang -O2 -pedantic -Wall -o eucalyptus Eucalyptus.c

generate:
	./eucalyptus

clean:
	rm -f EucalyptusKPK* eucalyptus
