
#include "gtest/gtest.h"
#include "rcc.h"

TEST(MagicNumberTest, GetRCCMagicNumber) {
	EXPECT_EQ(GetRCCMagicNumber(), 0x12CC);
}

