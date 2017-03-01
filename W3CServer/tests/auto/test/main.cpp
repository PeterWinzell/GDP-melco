// Need to add all tst_*.h files here for them to run.
//#include "tst_src.h"
#include "tst_w3cserver.h"
#include "tst_jsonrequestparser.h"

#include <gtest/gtest.h>

int main(int argc, char *argv[])
{

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
