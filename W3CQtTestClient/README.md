# Melco Gothenburg W3C VISS Reference Implementation - Qt Test Client Application
A test application created in Qt to test and verify the W3C VISS Implementation.

Usage: W3CQtTestClient.exe [options] tests
Melco Gothenburg W3C VISS Reference Implementation - Test Client Application

Options:
  -?, -h, --help                 Displays this help.
  -r, --random                   Shuffles the order the test cases are
                                 excecuted in. The order will be different on
                                 each client.
  -s, --secure                   Use Secure Web Sockets. Does nothing at the
                                 moment since we are able to ignore all errors.
  -c, --clients <nr of clients>  Number of clients to use.
  --url <url>                    Full W3CServer target implementation url.
  --software <version>           Software version the test is run upon. When
                                 run from Bamboo this is the git commit id.
  --timestamp <timestamp>        Timestamp when the test were initialized. When
                                 run from bamboo this is the same timestamp
                                 found in the image created.

Arguments:
  tests                          Test cases to run, [subscribe, subscribeall,
                                 authorize, getvss, getset, status]. Each test
                                 case can be entered several times.

example: "W3CQtTestClient.exe -c 5 -secure authorize subscribe getvss subscribe" will use 5 clients all running the 4 tests in the same order, using the default secure url (wss://127.0.0.1:8080) 


Output will be a a xml test report, currently hardcoded to be put at /usr/bin/w3c-tests.xml