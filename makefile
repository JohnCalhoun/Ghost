CC=nvcc
OPTIONS=--std=c++11
RELEASE=
DEBUG=-g -G
INCLUDES=-I /usr/local/include
DEPENDS=main.cu ghost.h ghost.inl complex.h complex.inl julia.h julia.inl

ghost:$(DEPENDS)
	$(CC) -o ghost main.cu $(OPTIONS) $(RELEASE)

debug:$(DEPENDS)
	$(CC) -o ghostDebug main.cu $(OPTIONS) $(DEBUG)	
