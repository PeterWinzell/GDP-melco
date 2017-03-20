#include "testcasedescriptions.h"

////
/// @brief TestCaseDescriptions::TestCaseDescriptions
/// A (temporary?) class to hold metadata about all available test cases. Could be converted to a test case class instead,
/// but that would require more work that goes out of scope of the issue. For the future to decide.
///
TestCaseDescriptions::TestCaseDescriptions()
{   
    m_descriptions[(int)TestCase::SUBSCRIBE_UNSUBSCRIBE] = TestCaseDescription(
                (int)TestCase::SUBSCRIBE_UNSUBSCRIBE,
                TestCase::SUBSCRIBE_UNSUBSCRIBE,
                "Subscribe / Unsubscribe",
                "subscribe",
                "Will send one 'subscribe' request to one signal and send 'unsubscribe' request after 10 seconds."
                );
    m_descriptions[(int)TestCase::SUBSCRIBEALL_UNSUBSCRIBEALL] = TestCaseDescription(
                (int)TestCase::SUBSCRIBEALL_UNSUBSCRIBEALL,
                TestCase::SUBSCRIBEALL_UNSUBSCRIBEALL,
                "Subscribe / UnsubscribeAll",
                "subscribeall",
                "Will send three 'subscribe' requests to different signals and send 'unsubscribeAll' request after ten seconds."
                );
    m_descriptions[(int)TestCase::AUTHORIZE_SUCCESS] = TestCaseDescription(
                (int)TestCase::AUTHORIZE_SUCCESS,
                TestCase::AUTHORIZE_SUCCESS,
                "Authorize",
                "authorize",
                "Will send an 'authorize' request to the server and expect to be authorized."
                );
    m_descriptions[(int)TestCase::GET_VSS] = TestCaseDescription(
                (int)TestCase::GET_VSS,
                TestCase::GET_VSS,
                "GetVSS",
                "getvss",
                "Will send one 'getVSS' request and expect and answer."
                );
    m_descriptions[(int)TestCase::SET_GET] = TestCaseDescription(
                (int)TestCase::SET_GET,
                TestCase::SET_GET,
                "Set / Get",
                "setget",
                " [Not implemented] "
                );
    m_descriptions[(int)TestCase::STATUS] = TestCaseDescription(
                (int)TestCase::STATUS,
                TestCase::STATUS,
                "Status",
                "status",
                " [Not implemented] "
                );
}
