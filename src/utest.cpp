#include "utest.h"

#include <algorithm>
#include <functional>

namespace UTEST {
    int TestResult::s_passed_count = 0;
    int TestResult::s_failed_count = 0;

    void CheckStringsEqual(const char* expected, const char* actual,
                           bool require_eq, const TestDetail& testdetail)
    {
        if (expected == NULL && actual == NULL) {
            if (require_eq)
                TestResult::increasePassedCount();
            else {
                reportFailure("NULL", "NULL", require_eq, testdetail);
                TestResult::increaseFailedCount();
            }
            return;
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
                TestResult::increaseFailedCount();
            else
                TestResult::increasePassedCount();
            return;
        }

        if (require_eq && strcmp(expected, actual) == 0) {
            TestResult::increasePassedCount();
        } else {
            reportFailure(expected, actual, require_eq, testdetail);
            TestResult::increaseFailedCount();
        }
    }

    void checkEqual(const char* expected, const char* actual,
                    bool require_eq, const TestDetail& testdetail)
    {
        CheckStringsEqual(expected, actual, require_eq, testdetail);
    }

    void checkEqual(char* expected, char* actual,
                    bool require_eq, const TestDetail& testdetail)
    {
        CheckStringsEqual(expected, actual, require_eq, testdetail);
    }

    void checkEqual(char* expected, const char* actual,
                    bool require_eq, const TestDetail& testdetail)
    {
        CheckStringsEqual(expected, actual, require_eq, testdetail);
    }

    void checkEqual(const char* expected, char* actual,
                    bool require_eq, const TestDetail& testdetail)
    {
        CheckStringsEqual(expected, actual, require_eq, testdetail);
    }

    Test* TestList::s_head = NULL;

    void TestList::addTest(Test* test)
    {
        test->setNext(s_head);
        s_head = test;
    }

    void TestList::run()
    {
        Test* test = s_head;

        while(test) {
            test->run();
            test = test->getNext();
        }
    }

    TestListAdder::TestListAdder(Test* test)
    {
        TestList::addTest(test);
    }

    int runAllTests()
    {
        TestList::run();

        int passed_count = TestResult::getPassedCount();
        int failed_count = TestResult::getFailedCount();
        int total_count  = passed_count + failed_count;

        if (total_count == 0) {
            std::cout << "No test!" << std::endl;
            return 1;
        }

        if (failed_count)
            std::cout << "Summary : error: " << failed_count << " out of " << total_count
                      << " tests failed." << std::endl;
        else
            std::cout << "Summary : All " << total_count << " tests passed." << std::endl;

        return failed_count;
    }
}
