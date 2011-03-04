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

#include "utest.h"

#include <algorithm>
#include <functional>

namespace UTEST {
    bool MonoState::s_include_disabled = false;

    std::string TestInfo::s_name;

    int TestResult::s_passed_count = 0;
    int TestResult::s_failed_count = 0;

    void CheckStringsEqual(const char* expected, const char* actual,
                           bool require_eq, const CheckDetail& testdetail)
    {
        if (expected == NULL && actual == NULL) {
            if (require_eq)
                TestResult::increasePassedCount();
            else {
                TestResult::increaseFailedCount();
                reportFailure("NULL", "NULL", require_eq, testdetail);
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
            TestResult::increaseFailedCount();
            reportFailure(expected, actual, require_eq, testdetail);
        }
    }

    void checkEqual(const char* expected, const char* actual,
                    bool require_eq, const CheckDetail& testdetail)
    {
        CheckStringsEqual(expected, actual, require_eq, testdetail);
    }

    void checkEqual(char* expected, char* actual,
                    bool require_eq, const CheckDetail& testdetail)
    {
        CheckStringsEqual(expected, actual, require_eq, testdetail);
    }

    void checkEqual(char* expected, const char* actual,
                    bool require_eq, const CheckDetail& testdetail)
    {
        CheckStringsEqual(expected, actual, require_eq, testdetail);
    }

    void checkEqual(const char* expected, char* actual,
                    bool require_eq, const CheckDetail& testdetail)
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
            TestInfo::setName(test->getName());
            try {
                if (test->enabled() || MonoState::getIncludeDisabled())
                    test->run();
            }
            catch (CheckFailure&)
            {
                // can count failed tests later
            }

            test = test->getNext();
        }
    }

    TestListAdder::TestListAdder(Test* test)
    {
        TestList::addTest(test);
    }

    void init(int argc, char** argv)
    {
        if (argc == 2 && strcmp("--include-disabled", argv[1]) == 0)
            MonoState::setIncludeDisabled(true);
    }

    int runAllTests()
    {
        TestList::run();

        int passed_count = TestResult::getPassedCount();
        int failed_count = TestResult::getFailedCount();
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
