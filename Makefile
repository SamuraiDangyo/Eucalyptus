all:
	clang -O2 -pedantic -Wall -o Eucalyptus Eucalyptus.c

generate:
	./Eucalyptus

clean:
	rm -f EucalyptusKPK* Eucalyptus
