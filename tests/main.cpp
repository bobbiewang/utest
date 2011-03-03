#include "../src/utest.h"

struct Strings
{
    char*       str;
    std::string s;

    Strings()
    {
        str = "abc";
        s   = "abc";
    }
};

TEST_F(TestStringAssertions, Strings)
{
    CHECK_EQ(str,  s.c_str());
}

TEST(SampleFailure)
{
    CHECK_EQ(1, 2);
}

int main()
{
    UTEST::runAllTests();
}
