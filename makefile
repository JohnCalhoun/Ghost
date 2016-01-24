CC=nvcc
OPTIONS=--std=c++11
RELEASE=
DEBUG=-g -G
INCLUDES=-I /usr/local/include
DEPENDS=main.cu ghost.h ghost.inl complex.h complex.inl julia.h julia.inl

UTIL=/home/john/projects/flamingo/utilities
TEST_INCLUDES=-I $(UTIL) 
TEST_LINKS=-lgtest -lpthread -lboost_thread-mt

ghost:$(DEPENDS)
	$(CC) -o ghost main.cu $(OPTIONS) $(RELEASE)

debug:$(DEPENDS)
	$(CC) -o ghostDebug main.cu $(OPTIONS) $(DEBUG)

complex_test:complex.h complex.inl complex_test.cu
	$(CC) -o complex_test main_test.cu -D COMPLEX_TEST $(TEST_LINKS) $(TEST_INCLUDES) $(OPTIONS)

run:ghost run.sh 
	sbatch run.sh

view: ghost
	eog julia.bmp
