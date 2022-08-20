#pragma once

#include <functional>
#include <map>
#include <deque>

//#include "Net/Socket.hpp"
#include "ClientSocket.hpp"
#include "ServerSocket.hpp"
#include "InputSensor.hpp"
#include "OutputSensor.hpp"

//#include "IO/Client.hpp"

#include <mutex>

namespace hub {
namespace net {

//struct Streamer {
//    hub::InputSensor mInputSensor;
//    std::string mSensorName;

//    std::list<hub::OutputSensor> mOutputSensors;
//    std::map<std::string, std::list<hub::OutputSensor>> mSensor2syncViewers;

//    Streamer* mSyncMaster = nullptr;
//    std::map<std::string, std::deque<hub::Acquisition>> mSensor2acqs;
//};

//struct Viewer {
//    const ClientSocket* const mSock = nullptr;

//    void notifyNewStreamer( const Streamer& streamer ) const;
//};


} // namespace net
} // namespace hub
