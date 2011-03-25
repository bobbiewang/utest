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
    class Test
    {
    public:
        Test(const char* name);
        Test(): m_next(NULL) {};

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

    class StringCache
    {
    public:
        bool operator<<(const char* str) { m_cache = str; return true; }

        std::string str()
        {
            std::string s = m_cache;
            m_cache.clear();
            return s;
        }

        operator bool() { return true; }

    private:
        std::string m_cache;
    };

    class DataSpace
    {
    public:
        DataSpace()
        {
            m_include_disabled = false;

            m_test_list_head = NULL;

            m_passed_count = 0;
            m_failed_count = 0;
        }

        bool getIncludeDisabled() const { return m_include_disabled; }
        void setIncludeDisabled(bool v) { m_include_disabled = v;    }

        Test* getTestList() const { return m_test_list_head; }

        void addTest(Test* test)
        {
            test->setNext(m_test_list_head);
            m_test_list_head = test;
        }

        const char* getTestName() const    { return m_current_test_name.c_str(); }
        void setTestName(const char* name) { m_current_test_name = name;         }

        StringCache& getUserMsg() { return m_user_msg; }

        int  getPassedCount() const { return m_passed_count; }
        void increasePassedCount()  { m_passed_count++;      }
        int  getFailedCount() const { return m_failed_count; }
        void increaseFailedCount()  { m_failed_count++;      }

    private:
        bool m_include_disabled;

        Test* m_test_list_head;

        std::string m_current_test_name;

        StringCache m_user_msg;

        int m_passed_count;
        int m_failed_count;
    };

    inline DataSpace& getDataSpace()
    {
        static DataSpace ds;
        return ds;
    }

    inline Test::Test(const char* name): m_name(name), m_next(NULL), m_enabled(true)
    {
        getDataSpace().addTest(this);
    };

    class CheckDetail
    {
    public:
        CheckDetail(const char* testname, const char* filename, int linenumber, const char* expression):
            m_testname(testname), m_filename(filename), m_linenumber(linenumber), m_expression(expression) {}

          const char* getTestName()   const { return m_testname.c_str();   }
          const char* getFileName()   const { return m_filename.c_str();   }
          int         getLineNumber() const { return m_linenumber;         }
          const char* getExpression() const { return m_expression.c_str(); }

    private:
        std::string m_testname;
        std::string m_filename;
        int         m_linenumber;
        std::string m_expression;
    };

    class CheckFailure {};

    template< typename Expected, typename Actual >
    void reportFailure(const Expected& expected, const Actual& actual,
                       bool require_eq, const CheckDetail& testdetail)
    {
        std::ostringstream os;

        std::string user_msg = getDataSpace().getUserMsg().str();
        if (!user_msg.empty())
            os << user_msg << ": ";

        if (require_eq)
            os << "Expected \"";
        else
            os << "Exprectd NOT \"";

        os << expected << "\" but was \"" << actual << "\"";

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
    bool& checkTrue(const T& actual, const CheckDetail& testdetail)
    {
        if (! actual) {
            getDataSpace().increaseFailedCount();
            reportFailure("true", "false", true, testdetail);
        } else {
            getDataSpace().increasePassedCount();
        }

        static bool useless = true;
        return useless;
    }

    template< typename T >
    bool& checkFalse(const T& actual, const CheckDetail& testdetail)
    {
        if (actual) {
            getDataSpace().increaseFailedCount();
            reportFailure("false", "true", true, testdetail);
        } else {
            getDataSpace().increasePassedCount();
        }

        static bool useless = true;
        return useless;
    }

    template< typename T >
    bool& checkNULL(const T* actual, const CheckDetail& testdetail)
    {
        if (actual != NULL) {
            getDataSpace().increaseFailedCount();
            reportFailure("NULL", actual, true, testdetail);
        } else {
            getDataSpace().increasePassedCount();
        }

        static bool useless = true;
        return useless;
    }

    template< typename Expected, typename Actual >
    bool& checkEqual(const Expected&    expected,
                     const Actual&      actual,
                     bool               require_eq,
                     const CheckDetail& testdetail)
    {
        if (check_traits<Expected, Actual>::equal(expected, actual) != require_eq) {
            getDataSpace().increaseFailedCount();
            reportFailure(expected, actual, require_eq, testdetail);
        } else {
            getDataSpace().increasePassedCount();
        }

        static bool useless = true;
        return useless;
    }

    inline bool& CheckStringsEqual(const char* expected, const char* actual,
                                  bool require_eq, const CheckDetail& testdetail)
    {
        static bool useless = true;

        if (expected == NULL && actual == NULL) {
            if (require_eq) {
                getDataSpace().increasePassedCount();
            } else {
                getDataSpace().increaseFailedCount();
                reportFailure("NULL", "NULL", require_eq, testdetail);
            }
            return useless;
        }

        if (expected == NULL ) {
            if (require_eq)
                reportFailure("NULL", actual, require_eq, testdetail);
        }

        if (actual == NULL) {
            if (require_eq)
                reportFailure(expected, "NULL", require_eq, testdetail);
        }

        if (expected == NULL || actual == NULL) {
            if (require_eq)
                getDataSpace().increaseFailedCount();
            else
                getDataSpace().increasePassedCount();
            return useless;
        }

        if (require_eq && strcmp(expected, actual) == 0) {
            getDataSpace().increasePassedCount();
        } else {
            getDataSpace().increaseFailedCount();
            reportFailure(expected, actual, require_eq, testdetail);
        }

        return useless;
    }

    inline bool& checkEqual(const char* expected, const char* actual,
                           bool  require_eq, const CheckDetail& testdetail)
    {
        return CheckStringsEqual(expected, actual, require_eq, testdetail);
    }

    inline bool& checkEqual(char* expected, char* actual,
                           bool  require_eq, const CheckDetail& testdetail)
    {
        return CheckStringsEqual(expected, actual, require_eq, testdetail);
    }
    inline bool& checkEqual(char* expected, const char* actual,
                           bool  require_eq, const CheckDetail& testdetail)
    {
        return CheckStringsEqual(expected, actual, require_eq, testdetail);
    }
    // TODO: why cannot use check_traits to compare "xxx" with char[4]
    inline bool& checkEqual(const char* expected, char* actual,
                           bool  require_eq, const CheckDetail& testdetail)
    {
        return CheckStringsEqual(expected, actual, require_eq, testdetail);
    }

    inline int utest_main(int argc, char** argv)
    {
        if (argc == 2 && strcmp("--include-disabled", argv[1]) == 0)
            getDataSpace().setIncludeDisabled(true);

        Test* test = getDataSpace().getTestList();

        while(test) {
            getDataSpace().setTestName(test->getName());
            try {
                if (test->enabled() || getDataSpace().getIncludeDisabled())
                    test->run();
            }
            catch (CheckFailure&)
            {
                // can count failed tests later
            }

            test = test->getNext();
        }

        int passed_count = getDataSpace().getPassedCount();
        int failed_count = getDataSpace().getFailedCount();
        int total_count  = passed_count + failed_count;

        if (total_count == 0) {
            std::cout << "No check!" << std::endl;
            return 1;
        }

        if (failed_count)
            std::cout << "Summary : error: " << failed_count << " out of " << total_count
                      << " checks failed." << std::endl;
        else
            std::cout << "Summary : All " << total_count << " checks passed." << std::endl;

        return failed_count;
    }
}

#ifdef DO_CHECK_LOG
#define LOG_CHECK std::cout << "TEST " << UTEST::getDataSpace().getTestName() \
                            << " @ " << __FILE__ << ":" << __LINE__ << std::endl;
#else
#define LOG_CHECK
#endif

#define SOME_CHECK_DETAIL UTEST::getDataSpace().getTestName(), __FILE__, __LINE__
#define USER_MSG UTEST::getDataSpace().getUserMsg()

#define CHECK_TRUE(actual) \
    LOG_CHECK \
    UTEST::checkTrue((actual), UTEST::CheckDetail(SOME_CHECK_DETAIL, #actual)) = USER_MSG

#define CHECK_FALSE(actual) \
    LOG_CHECK \
    UTEST::checkFalse((actual), UTEST::CheckDetail(SOME_CHECK_DETAIL, #actual)) = USER_MSG

#define CHECK_NULL(actual) \
    LOG_CHECK \
    UTEST::checkNULL((actual), UTEST::CheckDetail(SOME_CHECK_DETAIL, #actual)) = USER_MSG

#define CHECK_EQ(expected, actual) \
    LOG_CHECK \
    UTEST::checkEqual((expected), (actual), true, UTEST::CheckDetail(SOME_CHECK_DETAIL, #actual)) = USER_MSG

#define CHECK_NE(expected, actual) \
    LOG_CHECK \
    UTEST::checkEqual((expected), (actual), false, UTEST::CheckDetail(SOME_CHECK_DETAIL, #actual)) = USER_MSG

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
    void Test##Fixture##Name::run()

#define UTEST_MAIN \
    int main(int argc, char** argv) \
    {                               \
        return UTEST::utest_main(argc, argv); \
    }
#endif
