#include <CppUTest/CommandLineTestRunner.h>
#include <CppUTest/TestHarness.h>

TEST_GROUP(BTS7960DriverUnitTests) {};

TEST(BTS7960DriverUnitTests, dummyTest) { CHECK_EQUAL(true, false); }

int main(int ac, char **av) { return CommandLineTestRunner::RunAllTests(ac, av); }
