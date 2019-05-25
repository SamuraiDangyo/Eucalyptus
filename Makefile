##
#
# Eucalyptus, KPK Bitbases Generator
# GNU General Public License version 3; for details see LICENSE
#
##

main: build
	./Eucalyptus
	
build: Eucalyptus.c
	rm -f Eucalyptus_KPK.cpp Eucalyptus_KPK.c Eucalyptus_KPK.hpp Eucalyptus_KPK.h Eucalyptus \
		&& gcc -O2 -pedantic -Wall -o Eucalyptus Eucalyptus.c
	
	
cpp: build
	./Eucalyptus --cpp
	
cleanup: 
	rm -f Eucalyptus_KPK.cpp Eucalyptus_KPK.c Eucalyptus_KPK.hpp Eucalyptus_KPK.h Eucalyptus
	
