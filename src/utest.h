#include <vector>
#include <iostream>
#include <sstream>

#include <cstdlib>
#include <cstring>


namespace UTEST {
    class TestDetail
    {
    public:
        TestDetail(const char* testname, const char* filename, int linenumber):
          m_testname(testname), m_filename(filename), m_linenumber(linenumber) {}

          const char* getTestName() const   { return m_testname.c_str(); }
          const char* getFileName() const   { return m_filename.c_str(); }
          int         getLineNumber() const { return m_linenumber;       }

    private:
        std::string m_testname;
        std::string m_filename;
        int         m_linenumber;
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

    template< typename Expected, typename Actual >
    void reportFailure(const Expected& expected, const Actual& actual,
                       bool require_eq, const TestDetail& testdetail)
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

    }

    template< typename Expected, typename Actual >
    struct check_traits
    {
        static bool equal(const Expected& x, const Actual& y) {
            return x == y;
        }
    };

    template< typename T >
    void checkTrue(const T& actual, const TestDetail& testdetail)
    {
        if (! actual) {
            reportFailure("true", "false", true, testdetail);
            TestResult::increaseFailedCount();
        } else {
            TestResult::increasePassedCount();
        }
    }

    template< typename T >
    void checkFalse(const T& actual, const TestDetail& testdetail)
    {
        if (actual) {
            reportFailure("false", "true", true, testdetail);
            TestResult::increaseFailedCount();
        } else {
            TestResult::increasePassedCount();
        }
    }

    template< typename Expected, typename Actual >
    void checkEqual(const Expected&   expected,
                    const Actual&     actual,
                    bool              require_eq,
                    const TestDetail& testdetail)
    {
        if (check_traits<Expected, Actual>::equal(expected, actual) != require_eq) {
            reportFailure(expected, actual, require_eq, testdetail);
            TestResult::increaseFailedCount();
        } else {
            TestResult::increasePassedCount();
        }
    }

    void checkEqual(const char* expected, const char* actual,
                    bool  require_eq, const TestDetail& testdetail);
    void checkEqual(char* expected, char* actual,
                    bool  require_eq, const TestDetail& testdetail);
    void checkEqual(char* expected, const char* actual,
                    bool  require_eq, const TestDetail& testdetail);
    // TODO: why cannot use check_traits to compare "xxx" with char[4]
    void checkEqual(const char* expected, char* actual,
                    bool  require_eq, const TestDetail& testdetail);

    class Test
    {
    public:
        Test(const char* name): m_name(name), m_next(NULL) {};
        // use default ~Test()

        const char* getName() const     { return m_name.c_str(); }
        Test*       getNext() const     { return m_next;         }
        void        setNext(Test* next) { m_next = next;         }

        virtual void run() {};

    private:
        std::string m_name;
        Test*       m_next;
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

    int runAllTests();
}

#ifdef DO_CHECK_LOG
#define LOG_CHECK std::cout << "TEST " << getName() << " @ " << __FILE__ << ":" << __LINE__ << std::endl;
#else
#define LOG_CHECK
#endif

#define CHECK_TRUE(actual) \
    LOG_CHECK \
    (UTEST::checkTrue((actual), UTEST::TestDetail(getName(), __FILE__, __LINE__)))

#define CHECK_FALSE(actual) \
    LOG_CHECK \
    (UTEST::checkFalse((actual), UTEST::TestDetail(getName(), __FILE__, __LINE__)))

#define CHECK_EQ(expected, actual) \
    LOG_CHECK \
    (UTEST::checkEqual((expected), (actual), true, \
                       UTEST::TestDetail(getName(), __FILE__, __LINE__)))

#define CHECK_NE(expected, actual) \
    LOG_CHECK \
    (UTEST::checkEqual((expected), (actual), false, \
                       UTEST::TestDetail(getName(), __FILE__, __LINE__)))

#define TEST(name)                                           \
    class Test##name : public UTEST::Test                    \
    {                                                        \
    public:                                                  \
        Test##name() : Test(#name) {}                        \
        virtual void run();                                  \
    } test##name##Instance;                                  \
                                                             \
    UTEST::TestListAdder adder##name(&test##name##Instance); \
                                                             \
    void Test##name::run()

#define TEST_F(name, Fixture)                                                  \
    class Test##Fixture##name : public UTEST::Test, public Fixture             \
    {                                                                          \
    public:                                                                    \
        Test##Fixture##name() : Test(#Fixture "/" #name) {}                    \
    private:                                                                   \
        virtual void run();                                                    \
    } test##Fixture##name##Instance;                                           \
                                                                               \
    UTEST::TestListAdder adder##Fixture##name(&test##Fixture##name##Instance); \
                                                                               \
    void Test##Fixture##name::run()
