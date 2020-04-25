all:
	clang++ Eucalyptus.cpp -O2 -pedantic -Wall -o eucalyptus

generate:
	./eucalyptus

clean:
	rm -f EucalyptusKPK* eucalyptus
