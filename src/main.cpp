#include <iostream>
#include <getopt.h>

#include "webServerAnalyser.h"

using namespace std;

void printUsageInstructions()
{
    cout << endl
         << "Required arguments:" << endl
         << "  --action=webserver OR --action=resource" << endl
         << "    =webserver: provides number of accesses to webserver per host" << endl
         << "    =resource: provides number of successful resource accesses by URI" << endl
         << "  --filepath=<path_to_webserver_logs>" << endl
         << endl
         << "Optional arguments:" << endl
         << " --minimum_date=[DD:HH:MM:SS]" << endl
         << " --maximum_date=[DD:HH:MM:SS]" << endl
         << "   NOTE: minimum_date and maximum_date are used in conjunction" << endl
         << "   to filter logs between the two date times" << endl
         << endl
         << "Sample usage:" << endl
         << "./logParser --action=webserver --file=../tests/hostAccessTest_small.txt" << endl
         << "  (Request number of accesses to webserver per host, using provided log file" << endl
         << "./logParser --action=resource --file=../tests/resourceAccessTest_small.txt --minimum_date=[29:23:53:27] --maximum_date=[29:23:54:18]" << endl
         << "  (Request number of successful resource accesses by URI, ranging between provided date times, using provided log file" << endl;
}

int main( int argc, char **argv )
{
    int opt;
    string filePath, action, minDate, maxDate;
    vector <string> output;

    // Parse user provided arguments / options using getopt with long options
    option long_opts[] = { { "action", required_argument, nullptr, 'a' },
                           { "file", required_argument, nullptr, 'f' },
                           { "minimum_date", optional_argument, nullptr, 'm' },
                           { "maximum_date", optional_argument, nullptr, 'x' } };

    while ( ( opt = getopt_long( argc, argv, "a:f:mx", long_opts, nullptr ) ) != -1 )
    {
        switch ( opt )
        {
            case 'a':
                action = optarg;
                break;
            case 'f':
                filePath = optarg;
                break;
            case 'm':
                minDate = optarg;
                break;
            case 'x':
                maxDate = optarg;
                break;
            default:
                printUsageInstructions();
                return -1;
        }
    }

    // Perform actions depending on user provided arguments / options
    webServerAnalyser takeHomeAssignment;
    if ( action == "webserver" )
    {
        output = takeHomeAssignment.hostAccesses( filePath, minDate, maxDate );
    }
    else if ( action == "resource" )
    {
        output = takeHomeAssignment.resourceAccesses( filePath, minDate, maxDate );
    }
    else
    {
        printUsageInstructions();
        return -1;
    }

    // Print requested output
    for (auto it: output)
    {
        cout << it << endl;
    }

    return 0;
}
