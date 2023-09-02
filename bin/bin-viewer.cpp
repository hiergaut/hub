
#include <iostream>

#include <client/Viewer.hpp>

#define HEADER_MSG "\t[Viewer] "

int main() {

    auto onNewStreamer = [=]( const std::string& streamName, const hub::SensorSpec& sensorSpec ) {
        std::cout << HEADER_MSG "onNewStreamer : " << streamName << std::endl;
        return true;
    };
    auto onDelStreamer = []( const std::string& streamName, const hub::SensorSpec& sensorSpec ) {
        std::cout << HEADER_MSG "onDelStreamer : " << streamName << std::endl;
    };
    auto onServerNotFound = []( const std::string& ipv4, int port ) {
        std::cout << HEADER_MSG "onServerNotFound : " << ipv4 << " " << port << std::endl;
    };
    auto onServerConnected = []( const std::string& ipv4, int port ) {
        std::cout << HEADER_MSG "onServerConnected : " << ipv4 << " " << port << std::endl;
    };
    auto onServerDisconnected = []( const std::string& ipv4, int port ) {
        std::cout << HEADER_MSG "onServerDisconnected : " << ipv4 << " " << port << std::endl;
    };
    auto onNewAcquisition = []( const std::string& streamName, const hub::Acquisition& acq ) {
        std::cout << HEADER_MSG "onNewAcquisition : " << acq << std::endl;
    };
    auto onSetProperty = []( const std::string& streamName,
                             const std::string& objectName,
                             int property,
                             const hub::Any& value ) {
        std::cout << HEADER_MSG "onSetProperty " << streamName << " " << objectName << " "
                  << property << " " << value << std::endl;
    };
    auto onLogMessage = []( const std::string& logMessage ) {
        std::cout << HEADER_MSG "onLogMessage '" << logMessage << "'" << std::endl;
    };

    hub::client::Viewer viewer( __FILE_NAME__,
                                onNewStreamer,
                                onDelStreamer,
                                onServerNotFound,
                                onServerConnected,
                                onServerDisconnected,
                                onNewAcquisition,
                                onSetProperty,
                                onLogMessage );

    std::cout << "\t[viewer] Ctrl+C to exit" << std::endl;

    while ( true ) {
        std::this_thread::sleep_for( std::chrono::milliseconds( 1000 ) );
    }
}
