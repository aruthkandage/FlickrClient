#include <iostream>
#include <string>
#include <sstream>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <libxml/parser.h>

using namespace std;
using namespace boost::asio;

int main()
{
    io_service io;
    ip::tcp::resolver resolver(io);
    ip::tcp::resolver::query query("api.flickr.com", "http"); 
    
    try {
        ip::tcp::resolver::iterator endpoint_iter = resolver.resolve(query);
        ip::tcp::endpoint endpoint = (*endpoint_iter).endpoint();
        boost::system::error_code error;

        generic::stream_protocol::socket socket(io);
        socket.connect(endpoint);

        boost::asio::streambuf request_buffer;
        std::ostream request_buffer_stream(&request_buffer);

        request_buffer_stream << "GET /services/rest/?method=flickr.test.echo&hello=world&";
        request_buffer_stream << "api_key=e12803478647a404b627ca8b6b4fccfe ";
        request_buffer_stream << "HTTP/1.0\r\n"; // close connection after one request
        request_buffer_stream << "Host: api.flickr.com\r\n";
        request_buffer_stream << "Connection: close\r\n";
        request_buffer_stream << "\r\n"; // need a blank line

        boost::asio::write(socket, request_buffer);

        boost::asio::streambuf response_buffer;
        std::istream response_buffer_stream(&response_buffer);
       
        // read the first line 
        boost::asio::read_until(socket, response_buffer, "\r\n");

        std::string token;
        response_buffer_stream >> token;

        if(token.compare("HTTP/1.0") != 0) {
            return -1;
        }

        response_buffer_stream >> token;
        if(token.compare("200") != 0) {
            return -1;
        }

        // clear the rest of the line from the buffer
        std::getline(response_buffer_stream, token);

        while(boost::asio::read_until(socket, response_buffer, "\r\n")) {
            if(response_buffer_stream.peek() == '\r') {
                std::getline(response_buffer_stream, token);
                break;
            } else {
                response_buffer_stream >> token;

                if(token.compare("Content-Type:") == 0) {
                    // record content type
                } else if(token.compare("Content-Length:") == 0) {
                    // record content length
                }

                std::getline(response_buffer_stream, token);
            }
        }

        std::string message_body;
        char message_body_chunk[64] = "";

        while(!response_buffer_stream.eof()) {
            message_body += message_body_chunk;

            if(response_buffer.size() == 0) {
                if(!boost::asio::read(socket, response_buffer, error)) {
                    break;
                }
            }

            response_buffer_stream.getline(message_body_chunk, 64);
        }

        xmlDocPtr xml = xmlReadMemory(message_body.c_str(), message_body.length(), "FlickrResponse.xml", "utf-8", 0);

        if(xml != 0) {
            xmlFreeDoc(xml);   
        } else {
            cout << "Could not parse XML!" << endl;
        }             

        socket.close(); 
    } catch(boost::system::system_error& error) {
        cout << "Error!" << endl;
        cout << error.what() << endl;
    }

    return 0;
}
