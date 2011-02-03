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

int main()
{
    UTEST::runAllTests();
}
