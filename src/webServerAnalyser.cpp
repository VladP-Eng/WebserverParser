#include <iostream>
#include <fstream>

#include <map>
#include <unordered_map>
#include <vector>
#include <string>

#include "webServerAnalyser.h"

/*
 * @method: parseDate
 * @brief: converts a date time into a vector format
 *         ex. "[29:23:54:18] -> [29] [23] [54] [18]
 * @input - date: the date time to be converted
 * @return: returns the vector form of the date time, split using the ":" delimiter
 */
std::vector <int> webServerAnalyser::parseDate( std::string date )
{
    std::vector <int> outputDate;
    size_t linePos = 1, endPos = 0; // Start at linePos = 1 to ignore the first "["
    std::string dateEntry;

    for (int dateIndex = 1; dateIndex <= DATE_ENTRIES; dateIndex++)
    {
        // Look for : or the ] delimiter (indicating last entry of date time)
        endPos = date.find_first_of( ":]", linePos );
        dateEntry = date.substr( linePos, endPos - linePos );
        linePos = endPos + 1;

        outputDate.push_back( std::stoi( dateEntry ) );
    }

    return outputDate;
}

/*
 * @method: parseHTTPRequest
 * @brief: converts an HTTP request at column #3 into a vector format
 *         ex. "GET /Software.html HTTP/1.0" -> ["GET"] ["/Software.html"] ["HTTP/1.0"]
 * @input - request: the HTTP request to be converted
 * @return: returns the vector form of the HTTP request, split using the " " delimiter
 */
std::vector <std::string> webServerAnalyser::parseHTTPRequest( std::string request )
{
    std::vector <std::string> outputRequest;
    size_t linePos = 0, endPos = 0; // No quotes in request, start at linePos = 0
    std::string requestEntry;

    for (int requestIndex = 1; requestIndex <= REQUEST_ENTRIES; requestIndex++)
    {
        endPos = request.find( " ", linePos );
        requestEntry = request.substr( linePos, endPos - linePos );
        linePos = endPos + 1;

        outputRequest.push_back( requestEntry );
    }

    return outputRequest;

}

/*
 * @method: verifyDateTimeRange
 * @brief: verifies that the provided inputDate ranges between the minDate and the maxDate
 * @input - inputDate: the date time to be compared
 * @input - minDate: the date time located at the minimum range of comparison
 * @input - maxDate: the date time located at the maximum range of comparison
 * @return: returns true if the provided input date time is between the minimum and maximum input ranges, false otherwise
 *          returns true if minDate or maxDate are not populated (not used to filter the input date time)
 */
bool webServerAnalyser::verifyDateTimeRange( std::string inputDate, std::string minDate, std::string maxDate)
{
    bool ret = true;

    // If minDate or maxDate are not provided, filtering is not used: returns true
    if ( !minDate.empty() && !maxDate.empty() )
    {
        std::vector <int> formattedDate = parseDate( inputDate );
        std::vector <int> formattedMin = parseDate( minDate );
        std::vector <int> formattedMax = parseDate( maxDate );

        // Compares the days, hours, minutes and seconds of the date time until violation occurs
        // If no violation occurs, then input date time is within the valid range
        for (int i = 0; i < DATE_ENTRIES; i++)
        {
            if ( formattedMin[i] > formattedDate[i] && formattedMax[i] < formattedDate[i] )
            {
                // inputDate is outside of minimum/maximum boundaries, exit early
                ret = false;
                break;
            }
        }
    }

    return ret;
}

/*
 * @method: verifyHTTPRequest
 * @brief: verifies the request member of the webServerLog with the filtered inputs and extracts the requested resource
 * @input - inputEntry: the webServerLog struct that contains the request to be verified
 * @input - filteredHTTPMethod: the HTTP method to compare against in the request
 * @input - filteredHTTPResponse: the HTTP response to compare against in the request
 * @output - resource: the resource that is being accessed in the URI
 * @return: returns true if the request member has been verified with the expected filtered inputs, false otherwise
 */
bool webServerAnalyser::verifyHTTPRequest( webServerLog inputEntry, std::string filteredHTTPMethod, std::string filteredHTTPResponse, std::string &resource )
{
    bool ret = false;

    // Parse the HTTP request into vector format for easier handling:
    // ex. "GET /Software.html HTTP/1.0" -> ["GET"] ["/Software.html"] ["HTTP/1.0"]
    std::vector <std::string> formattedRequest = parseHTTPRequest( inputEntry.request );

    // Verify that HTTP request method and HTTP response match the filtered values and populate the resource output if true
    if ( formattedRequest[0] == filteredHTTPMethod && inputEntry.httpResponse == filteredHTTPResponse )
    {
        resource = formattedRequest[1];
        ret = true;
    }

    return ret;
}

/*
 * @method: parseLine
 * @brief: converts a line of the webserver logs into a webServerLog structure format
 * @input - line: the webserver log line to be converted
 * @return: returns the corresponding webServerLog structure from the line input
 */
webServerLog webServerAnalyser::parseLine( std::string line )
{
    webServerLog outputLine;
    size_t linePos = 0, endPos = 0;
    std::string column;

    // Go through the line, extracting all MAX_COLUMNS # of columns into individual members of the webServerLog struct
    for (int columnIndex = 1; columnIndex <= MAX_COLUMNS; columnIndex++)
    {
        if ( columnIndex == REQUEST_INDEX )
        {
            // Request field in webserver log needs to be handled differently since spaces are not valid delimiters
            endPos = line.find( "\" ", linePos );
            column = line.substr( linePos + 1, endPos - linePos - 1 );
            linePos = endPos + 2;
        }
        else
        {
            endPos = line.find_first_of( " \n", linePos );
            column = line.substr( linePos, endPos - linePos );
            linePos = endPos + 1;
        }

        // Assign each member of the webServerLog struct to the extracted entry
        switch (columnIndex)
        {
            case 1:
                outputLine.host = column;
                break;
            case 2:
                outputLine.date = column;
                break;
            case REQUEST_INDEX:
                outputLine.request = column;
                break;
            case 4:
                outputLine.httpResponse = column;
                break;
            case 5:
                outputLine.retSize = column;
                break;
            default:
                std::cout << "MAX_COLUMNS exceeded # of case statements, webServerLog will stop being populated" << std::endl;
        }
    }

#if 0
    std::cout << "host: " << outputLine.host << std::endl
              << "date: " << outputLine.date << std::endl
              << "request: " << outputLine.request << std::endl
              << "httpResponse: " << outputLine.httpResponse << std::endl
              << "retSize: " << outputLine.retSize << std::endl << std::endl;
#endif

    return outputLine;
}

/*
 * @method: hostAccesses
 * @brief: provides number of accesses to a webserver per host
 * @input - filepath: file containing the webserver logs to process
 * @input - minDate: minimum date time used to filter the webserver logs
 * @input - maxDate: maximum date time used to filter the webserver logs
 *          Note: both minDate and maxDate are optional: can be blank to indicate no filtering
 *          Note: both minDate and maxDate are used in conjunction, cannot use one without the other
 * @return: returns the number of successful accesses to a webserver per host in vector format
 */
std::vector <std::string> webServerAnalyser::hostAccesses( std::string filePath, std::string minDate, std::string maxDate )
{
    std::unordered_map <std::string, int> accessCount;
    std::multimap <int, std::string> sortedAccessCount;
    std::vector <std::string> output;
    std::string currentLine;

    // Step 1: read file
    std::ifstream inputStream( filePath );

    // Step 2: parse file line by line
    while( std::getline( inputStream, currentLine ) )
    {
        // Step 3: parse the single line into a webServerLog structure
        webServerLog parsedLine = parseLine( currentLine );

        // Step 4: verify that entry is within a valid time range (if specified)
        if ( verifyDateTimeRange( parsedLine.date, minDate, maxDate ) )
        {
            // Step 5: once verified, increment number of accesses for the host
            accessCount[parsedLine.host]++;
        }
    }

    // Step 6: sort entries based on the # of accesses, most # of entries at the top
    for (auto it: accessCount)
    {
        sortedAccessCount.insert( { it.second, it.first } );
    }
    for (auto rit = sortedAccessCount.rbegin(); rit != sortedAccessCount.rend(); rit++)
    {
        output.push_back( std::string( rit->second + " " + std::to_string( rit->first ) ) );
    }

    return output;
}

/*
 * @method: resourceAccesses
 * @brief: provides number of accesses to a resource for requests with HTTP "GET" and HTTP_OK (200) response
 * @input - filepath: file containing the webserver logs to process
 * @input - minDate: minimum date time used to filter the webserver logs
 * @input - maxDate: maximum date time used to filter the webserver logs
 *          Note: both minDate and maxDate are optional: can be blank to indicate no filtering
 *          Note: both minDate and maxDate are used in conjunction, cannot use one without the other
 * @return: returns the number of successful accesses to a resource per host in vector format
 */
std::vector <std::string> webServerAnalyser::resourceAccesses( std::string filePath, std::string minDate, std::string maxDate )
{
    std::unordered_map <std::string, int> accessCount;
    std::multimap <int, std::string> sortedAccessCount;
    std::vector <std::string> output;
    std::string currentLine, resource;

    // Step 1: create filestream
    std::ifstream inputStream( filePath );

    // Step 2: parse file line by line
    while( std::getline( inputStream, currentLine ) )
    {
        // Step 3: parse the single line into a webServerLog structure
        webServerLog parsedLine = parseLine( currentLine );

        // Step 4: verify that only HTTP "GET" and HTTP_OK (200) entries are counted, within a valid time range (if specified)
        if ( verifyDateTimeRange( parsedLine.date, minDate, maxDate ) &&
             verifyHTTPRequest( parsedLine, "GET", HTTP_OK, resource ) )
        {
            // Step 5: once verified, resource populated with verifyHTTPRequest, increment number of accesses
            accessCount[resource]++;
        }
    }

    // Step 6: sort entries based on the # of accesses, most # of entries at the top
    for (auto it: accessCount)
    {
        sortedAccessCount.insert( { it.second, it.first } );
    }
    for (auto rit = sortedAccessCount.rbegin(); rit != sortedAccessCount.rend(); rit++)
    {
        output.push_back( std::string( rit->second + " " + std::to_string( rit->first ) ) );
    }

    return output;
}
