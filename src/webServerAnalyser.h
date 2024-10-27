#include <iostream>
#include <vector>
#include <string>

#define HTTP_OK "200"

#define MAX_COLUMNS 5 // number of entries within the webServerLog
#define REQUEST_INDEX 3
/*
 * @struct: webServerLog
 * @brief: stores the components of the webserver logs
 * @member - host: hostname / IP address of the host making the request
 * @member - date: date time in [DD:HH:MM:SS] format
 * @member - request: request URI
 * @member - httpResponse: HTTP reply code
 * @member - retSize: number of bytes in the reply
 */
typedef struct webServerLog
{
    std::string host;
    std::string date;
    std::string request; // REQUEST_INDEX offset
    std::string httpResponse;
    std::string retSize;
} webServerLog;

#define DATE_ENTRIES 4 // number of entries within webServerLog.date
#define REQUEST_ENTRIES 3 // number of entries within webServerLog.request

class webServerAnalyser
{
    private:
       /*
        * @method: parseDate
        * @brief: converts a date time into a vector format
        *         ex. "[29:23:54:18] -> [29] [23] [54] [18]
        * @input - request: the date time to be converted
        * @return: returns the vector form of the date time, split using the ":" delimiter
        */
        std::vector <int> parseDate( std::string date );

       /*
        * @method: parseHTTPRequest
        * @brief: converts an HTTP request at column #3 into a vector format
        *         ex. "GET /Software.html HTTP/1.0" -> ["GET"] ["/Software.html"] ["HTTP/1.0"]
        * @input - request: the HTTP request to be converted
        * @return: returns the vector form of the HTTP request, split using the " " delimiter
        */
        std::vector <std::string> parseHTTPRequest( std::string request );

       /*
        * @method: verifyDateTimeRange
        * @brief: verifies that the provided inputDate ranges between the minDate and the maxDate
        * @input - inputDate: the date time to be compared
        * @input - minDate: the date time located at the minimum range of comparison
        * @input - maxDate: the date time located at the maximum range of comparison
        * @return: returns true if the provided input date time is between the minimum and maximum input ranges, false otherwise
        *          returns true if minDate or maxDate are not populated (not used to filter the input date time)
        */
        bool verifyDateTimeRange( std::string inputDate, std::string minDate, std::string maxDate );

       /*
        * @method: verifyHTTPRequest
        * @brief: verifies the request member of the webServerLog with the filtered inputs and extracts the requested resource
        * @input - inputEntry: the webServerLog struct that contains the request to be verified
        * @input - filteredHTTPMethod: the HTTP method to compare against in the request
        * @input - filteredHTTPResponse: the HTTP response to compare against in the request
        * @output - resource: the resource that is being accessed in the URI
        * @return: returns true if the request member has been verified with the expected filtered inputs, false otherwise
        */
        bool verifyHTTPRequest( webServerLog inputEntry, std::string filteredHTTPMethod, std::string filteredHTTPResponse, std::string &resource );

       /*
        * @method: parseLine
        * @brief: converts a line of the webserver logs into a webServerLog structure format
        * @input - line: the webserver log line to be converted
        * @return: returns the corresponding webServerLog structure from the line input
        */
        webServerLog parseLine( std::string line );

    public:
        webServerAnalyser() = default;

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
        std::vector <std::string> hostAccesses( std::string filePath, std::string minDate, std::string maxDate );

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
        std::vector <std::string> resourceAccesses( std::string filePath, std::string minDate, std::string maxDate );
};
