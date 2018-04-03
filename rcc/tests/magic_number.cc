
#include "gtest/gtest.h"
#include "rcc.h"

TEST(MagicNumberTest, GetRCCMagicNumber) {
	EXPECT_EQ(rcc::GetRCCMagicNumber(), 0x12CC);
}

