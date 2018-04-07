#include "gtest/gtest.h"
#include "./rcc.h"

TEST(MagicNumberTest, GetRCCMagicNumber) {
  EXPECT_EQ(cs160::rcc::GetRCCMagicNumber(), 0x12CC);
}
