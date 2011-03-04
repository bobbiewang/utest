// Copyright 2011, Bo Wang
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//
// * Redistributions of source code must retain the above copyright
//   notice, this list of conditions and the following disclaimer.
// * Redistributions in binary form must reproduce the above copyright
//   notice, this list of conditions and the following disclaimer in
//   the documentation and/or other materials provided with the
//   distribution.
// * The names of its contributors cannot be used to endorse or
//   promote products derived from this software without specific
//   prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
// FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
// COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
// HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
// STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
// OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Author: Bo Wang <Bobbie.Wang@gmail.com>
//
// The Ultra Light Weight C++ Unit Testing Framework (UTest)

#include "../src/utest.h"

#pragma warning(disable : 4996)

struct Strings
{
    std::string ss;             // stl string
    const char* cstr;           // const string
    char*       dstr;           // dynamic string
    char        sstr[4];        // static string

    Strings()
    {
        ss   = "abc";
        cstr = "abc";
        dstr = new char[4];
        strcpy(dstr, "abc");
        strcpy(sstr, "abc");
    }

    ~Strings()
    {
        delete[] dstr;
    }
};

TEST_F(TestStringAssertions, Strings)
{
    CHECK_EQ("abc", ss.c_str()); // TODO: doesn't support CHECK_EQ("abc", ss), CHECK_EQ(ss, ss)
    CHECK_EQ("abc", cstr);
    CHECK_EQ("abc", dstr);
    CHECK_EQ("abc", sstr);
}

struct HelperMethodInFixture
{
    int i;

    void setup()
    {
        i = 99;
    }

    void verify()
    {
        CHECK_EQ(99, i);
    }
};

TEST_F(Demo, HelperMethodInFixture)
{
    setup();
    verify();
}

TEST(SampleFailure)
{
    int number = 2;
    CHECK_EQ(1, number);

    CHECK_FALSE("Cannot reach here because previous check fails.");
}

DIS_TEST(LongTimeSample)
{
    CHECK_TRUE("Use --include-disabled to run this test.");
}

UTEST_MAIN
