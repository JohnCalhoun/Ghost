#include <gtest/gtest.h>

#if defined(COMPLEX_TEST) || defined(ALL)
#include "complex_test.cu"
#endif
i
int main(int argc, char **argv) {
     ::testing::InitGoogleTest(&argc, argv);
     return RUN_ALL_TESTS();
};
