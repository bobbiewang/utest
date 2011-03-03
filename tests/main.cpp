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

struct DemoHelperMethodInFixture
{
    int i;

    void setup()
    {
        i = 99;
    }

    void verify()
    {
        CHECK_EQ(98, i);
    }
};

TEST_F(Demo, DemoHelperMethodInFixture)
{
    setup();
    verify();
}

TEST(SampleFailure)
{
    CHECK_EQ(1, 2);
}

int main()
{
    UTEST::runAllTests();
}
