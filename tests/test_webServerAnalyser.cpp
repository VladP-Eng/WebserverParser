#include <iostream>
#include <gtest/gtest.h>

#include "../src/webServerAnalyser.h"

/*
 * @brief: verifies webServerAnalyser::hostAccesses without a date time range specified
 */
TEST( hostAccessTest, smallFile_noDateTime_test )
{
    // Generate output:
    webServerAnalyser testObj;
    std::vector <std::string> output = testObj.hostAccesses( "../tests/hostAccessTest_small.txt", "", "" );

    // Expected output:
    std::vector <std::string> expectedOut = { "wpbfl2-45.gate.net 6",
                                              "141.243.1.172 1",
                                              "query2.lycos.cs.cmu.edu 1",
                                              "tanuki.twics.com 1",
                                              "140.112.68.165 1" };

    for (int i = 0; i < expectedOut.size(); i++)
    {
        EXPECT_EQ( output[i], expectedOut[i] );
    }
}

/*
 * @brief: verifies webServerAnalyser::hostAccesses with an invalid date time range specified (no filtering will occur)
 */
TEST( hostAccessTest, smallFile_invalidDateTime_test )
{
    // Generate output:
    webServerAnalyser testObj;
    std::vector <std::string> output = testObj.hostAccesses( "../tests/hostAccessTest_small.txt", "[29:23:53:20]", "" );

    // Expected output:
    std::vector <std::string> expectedOut = { "wpbfl2-45.gate.net 6",
                                              "141.243.1.172 1",
                                              "query2.lycos.cs.cmu.edu 1",
                                              "tanuki.twics.com 1",
                                              "140.112.68.165 1" };

    for (int i = 0; i < expectedOut.size(); i++)
    {
        EXPECT_EQ( output[i], expectedOut[i] );
    }
}

/*
 * @brief: verifies webServerAnalyser::hostAccesses with a valid date time range specified
 */
TEST( hostAccessTest, smallFile_dateTimeRange_test )
{
    // Generate output:
    webServerAnalyser testObj;
    std::vector <std::string> output = testObj.hostAccesses( "../tests/hostAccessTest_small.txt", "[29:23:53:20]", "[29:23:54:16]" );

    // Expected output:
    std::vector <std::string> expectedOut = { "wpbfl2-45.gate.net 2",
                                              "141.243.1.172 1",
                                              "query2.lycos.cs.cmu.edu 1",
                                              "tanuki.twics.com 1" };

    for (int i = 0; i < expectedOut.size(); i++)
    {
        EXPECT_EQ( output[i], expectedOut[i] );
    }
}

/*
 * @brief: verifies webServerAnalyser::resourceAccesses without a date time range specified
 */
TEST( resourceAccessTest, smallFile_noDateTime_test )
{
    // Generate output:
    webServerAnalyser testObj;
    std::vector <std::string> output = testObj.resourceAccesses( "../tests/resourceAccessTest_small.txt", "", "" );

    // Expected output:
    std::vector <std::string> expectedOut = { "/Software.html 4",
                                              "/icons/circle_logo_small.gif 3",
                                              "/News.html 1",
                                              "/ 1",
                                              "/logos/us-flag.gif 1",
                                              "/icons/book.gif 1" };

    for (int i = 0; i < expectedOut.size(); i++)
    {
        EXPECT_EQ( output[i], expectedOut[i] );
    }
}

/*
 * @brief: verifies webServerAnalyser::resourceAccesses with an invalid date time range specified (no filtering will occur)
 */
TEST( resourceAccessTest, smallFile_invalidDateTime_test )
{
    // Generate output:
    webServerAnalyser testObj;
    std::vector <std::string> output = testObj.resourceAccesses( "../tests/resourceAccessTest_small.txt", "[29:23:53:27]", "" );

    // Expected output:
    std::vector <std::string> expectedOut = { "/Software.html 4",
                                              "/icons/circle_logo_small.gif 3",
                                              "/News.html 1",
                                              "/ 1",
                                              "/logos/us-flag.gif 1",
                                              "/icons/book.gif 1" };

    for (int i = 0; i < expectedOut.size(); i++)
    {
        EXPECT_EQ( output[i], expectedOut[i] );
    }
}

/*
 * @brief: verifies webServerAnalyser::resourceAccesses with a valid date time range specified
 */
TEST( resourceAccessTest, smallFile_dateTimeRange_test )
{
    // Generate output:
    webServerAnalyser testObj;
    std::vector <std::string> output = testObj.resourceAccesses( "../tests/resourceAccessTest_small.txt", "[29:23:53:27]", "[29:23:54:18]" );

    // Expected output:
    std::vector <std::string> expectedOut = { "/icons/circle_logo_small.gif 3",
                                              "/Software.html 2",
                                              "/News.html 1",
                                              "/ 1" };

    for (int i = 0; i < expectedOut.size(); i++)
    {
        EXPECT_EQ( output[i], expectedOut[i] );
    }
}

int main( int argc, char **argv )
{
    testing::InitGoogleTest( &argc, argv );
    return RUN_ALL_TESTS();
}
