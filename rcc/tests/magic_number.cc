
#include "gtest/gtest.h"
#include "rcc.h"

TEST(MagicNumberTest, RCCMagicNumber) {
	  EXPECT_EQ(rcc_magic_number(), 0x12CC);
}
