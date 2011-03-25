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

#if defined(WIN32) && defined(_MSC_VER)
  #pragma warning(disable : 4996)
#endif

struct DemoStrings
{
    std::string ss;             // stl string
    const char* cstr;           // const string
    char*       dstr;           // dynamic string
    char        sstr[4];        // static string

    DemoStrings()
    {
        ss   = "abc";
        cstr = "abc";
        dstr = new char[4];
        strcpy(dstr, "abc");
        strcpy(sstr, "abc");
    }

    ~DemoStrings()
    {
        delete[] dstr;
    }
};

TEST_F(TestStringAssertions, DemoStrings)
{
    CHECK_EQ("abc", "abc");
    CHECK_EQ("abc", ss);
    CHECK_EQ("abc", cstr);
    CHECK_EQ("abc", dstr);
    CHECK_EQ("abc", sstr);

    CHECK_EQ(ss, "abc");
    CHECK_EQ(ss, ss);
    CHECK_EQ(ss, cstr);
    CHECK_EQ(ss, dstr);
    CHECK_EQ(ss, sstr);

    CHECK_EQ(cstr, "abc");
    CHECK_EQ(cstr, ss);
    CHECK_EQ(cstr, cstr);
    CHECK_EQ(cstr, dstr);
    CHECK_EQ(cstr, sstr);

    CHECK_EQ(dstr, "abc");
    CHECK_EQ(dstr, ss);
    CHECK_EQ(dstr, cstr);
    CHECK_EQ(dstr, dstr);
    CHECK_EQ(dstr, sstr);

    CHECK_EQ(sstr, "abc");
    CHECK_EQ(sstr, ss);
    CHECK_EQ(sstr, cstr);
    CHECK_EQ(sstr, dstr);
    CHECK_EQ(sstr, sstr);
}

TEST(NullPointer)
{
    const char* dstr = NULL;

    CHECK_NULL(dstr);                  // suggested

    CHECK_EQ((const char*)NULL, dstr); // workable
    CHECK_FALSE(dstr);                 // workable
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
    CHECK_EQ(1, number) << "number is 2";

    CHECK_FALSE("Cannot reach here because previous check fails.");
}

DIS_TEST(LongTimeSample)
{
    CHECK_TRUE("Use --include-disabled to run this test.");
}

UTEST_MAIN
