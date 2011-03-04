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

#ifndef UTEST_H
#define UTEST_H

#include <vector>
#include <iostream>
#include <sstream>

#include <cstdlib>
#include <cstring>


namespace UTEST {
    class MonoState
    {
    public:
        static bool getIncludeDisabled()       { return s_include_disabled; }
        static void setIncludeDisabled(bool v) { s_include_disabled = v;    }

    private:
        static bool s_include_disabled;
    };

    class CheckDetail
    {
    public:
        CheckDetail(const char* testname, const char* filename, int linenumber, const char* expression):
            m_testname(testname), m_filename(filename), m_linenumber(linenumber), m_expression(expression) {}

          const char* getTestName() const     { return m_testname.c_str();   }
          const char* getFileName() const     { return m_filename.c_str();   }
          int         getLineNumber() const   { return m_linenumber;         }
          const char* getExpression() const   { return m_expression.c_str(); }

    private:
        std::string m_testname;
        std::string m_filename;
        int         m_linenumber;
        std::string m_expression;
    };

    class TestInfo
    {
    public:
        static void        setName(const char* name) { s_name = name;         }
        static const char* getName()                 { return s_name.c_str(); }

    private:
        static std::string s_name;
    };

    class TestResult
    {
    public:
        static void increasePassedCount() { ++s_passed_count; }
        static void increaseFailedCount() { ++s_failed_count; }

        static int getPassedCount() { return s_passed_count; }
        static int getFailedCount() { return s_failed_count; }

    private:
        static int s_passed_count;
        static int s_failed_count;
    };

    class CheckFailure {};

    template< typename Expected, typename Actual >
    void reportFailure(const Expected& expected, const Actual& actual,
                       bool require_eq, const CheckDetail& testdetail)
    {
        std::ostringstream os;

        if (require_eq)
            os << "Expected \"";
        else
            os << "Exprectd NOT \"";

        // Following codes handle NULL string, since os<<(const
        // char*)NULL will cause fatal error
        if (expected == 0)
            os << 0;
        else
            os << expected;

        os << "\" but was \"";

        if (actual == 0)
            os << 0;
        else
            os << actual;

        os  << "\"";

        std::cout << testdetail.getFileName() << "(" << testdetail.getLineNumber()
                  << ") : error: Failure in  *" << testdetail.getTestName()
                  << "* : " << os.str() << std::endl;
        std::cout << "  Expression: " << testdetail.getExpression() << std::endl
                  << "    Expected: " << expected << std::endl
                  << "      Actual: " << actual << std::endl;

        throw CheckFailure();
    }

    template< typename Expected, typename Actual >
    struct check_traits
    {
        static bool equal(const Expected& x, const Actual& y) {
            return x == y;
        }
    };

    template< typename T >
    void checkTrue(const T& actual, const CheckDetail& testdetail)
    {
        if (! actual) {
            TestResult::increaseFailedCount();
            reportFailure("true", "false", true, testdetail);
        } else {
            TestResult::increasePassedCount();
        }
    }

    template< typename T >
    void checkFalse(const T& actual, const CheckDetail& testdetail)
    {
        if (actual) {
            TestResult::increaseFailedCount();
            reportFailure("false", "true", true, testdetail);
        } else {
            TestResult::increasePassedCount();
        }
    }

    template< typename Expected, typename Actual >
    void checkEqual(const Expected&    expected,
                    const Actual&      actual,
                    bool               require_eq,
                    const CheckDetail& testdetail)
    {
        if (check_traits<Expected, Actual>::equal(expected, actual) != require_eq) {
            TestResult::increaseFailedCount();
            reportFailure(expected, actual, require_eq, testdetail);
        } else {
            TestResult::increasePassedCount();
        }
    }

    void checkEqual(const char* expected, const char* actual,
                    bool  require_eq, const CheckDetail& testdetail);
    void checkEqual(char* expected, char* actual,
                    bool  require_eq, const CheckDetail& testdetail);
    void checkEqual(char* expected, const char* actual,
                    bool  require_eq, const CheckDetail& testdetail);
    // TODO: why cannot use check_traits to compare "xxx" with char[4]
    void checkEqual(const char* expected, char* actual,
                    bool  require_eq, const CheckDetail& testdetail);

    class Test
    {
    public:
        Test(const char* name): m_name(name), m_next(NULL), m_enabled(true) {};
        Test(): m_next(NULL) {};
        // use default ~Test()

        const char* getName() const     { return m_name.c_str(); }
        Test*       getNext() const     { return m_next;         }
        void        setNext(Test* next) { m_next = next;         }
        bool        enabled() const     { return m_enabled;      }

        virtual void run() {};

    protected:
        bool        m_enabled;

    private:
        std::string m_name;
        Test*       m_next;
    };

    class DisabledTest : public Test
    {
    public:
        DisabledTest(const char* name): Test(name) { m_enabled = false; }
    };

    class TestList
    {
    public:
        static void addTest(Test* test);
        static void run();
    private:
        static Test* s_head;
    };

    class TestListAdder
    {
    public:
        TestListAdder(Test* test);
    };

    void init(int argc, char** argv);
    int runAllTests();
}

#ifdef DO_CHECK_LOG
#define LOG_CHECK std::cout << "TEST " << UTEST::TestInfo::getName() << " @ " << __FILE__ << ":" << __LINE__ << std::endl;
#else
#define LOG_CHECK
#endif

#define SOME_CHECK_DETAIL UTEST::TestInfo::getName(), __FILE__, __LINE__

#define CHECK_TRUE(actual) \
    LOG_CHECK \
    (UTEST::checkTrue((actual), UTEST::CheckDetail(SOME_CHECK_DETAIL, #actual)))

#define CHECK_FALSE(actual) \
    LOG_CHECK \
    (UTEST::checkFalse((actual), UTEST::CheckDetail(SOME_CHECK_DETAIL, #actual)))

#define CHECK_EQ(expected, actual) \
    LOG_CHECK \
    (UTEST::checkEqual((expected), (actual), true, UTEST::CheckDetail(SOME_CHECK_DETAIL, #actual)))

#define CHECK_NE(expected, actual) \
    LOG_CHECK \
    (UTEST::checkEqual((expected), (actual), false, UTEST::CheckDetail(SOME_CHECK_DETAIL, #actual)))

#define TEST(Name)     TEST_EX(Name, UTEST::Test)
#define DIS_TEST(Name) TEST_EX(Name, UTEST::DisabledTest)

#define TEST_F(Name, Fixture)     TEST_F_EX(Name, Fixture, UTEST::Test)
#define DIS_TEST_F(Name, Fixture) TEST_F_EX(Name, Fixture, UTEST::DisabledTest)

#define TEST_EX(Name, TestClass)                             \
    class Test##Name : public TestClass                      \
    {                                                        \
    public:                                                  \
        Test##Name() : TestClass(#Name) {}                   \
        virtual void run();                                  \
    } test##Name##Instance;                                  \
                                                             \
    UTEST::TestListAdder adder##Name(&test##Name##Instance); \
                                                             \
    void Test##Name::run()

#define TEST_F_EX(Name, Fixture, TestClass)                                    \
    class Test##Fixture##Name : public TestClass, public Fixture               \
    {                                                                          \
    public:                                                                    \
        Test##Fixture##Name() : TestClass(#Fixture "/" #Name) {}               \
    private:                                                                   \
        virtual void run();                                                    \
    } test##Fixture##Name##Instance;                                           \
                                                                               \
    UTEST::TestListAdder adder##Fixture##Name(&test##Fixture##Name##Instance); \
                                                                               \
    void Test##Fixture##Name::run()

#define UTEST_MAIN \
    int main(int argc, char** argv) \
    {                               \
        UTEST::init(argc, argv);    \
        UTEST::runAllTests();       \
    }

#endif
