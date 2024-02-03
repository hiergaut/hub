/// © 2021-2024 Hub, All Rights Reserved
/// @author gauthier <gauthierbouyjou@aol.com>
/// @date 2023/11/20

#pragma once

#include <string>

#include "core/io/InputOutput.hpp"
#include "net/ClientSocket.hpp"

namespace hub {
namespace io {

class InputOutputSocket : public InputOutput<>, public net::ClientSocket
{
  public:
    static struct {
    } not_endable;
    using InputOutput<>::read;
    using InputOutput<>::write;

    InputOutputSocket( net::ClientSocket&& clientSocket ) :
        net::ClientSocket( std::move( clientSocket ) ) {}
    InputOutputSocket( InputOutputSocket&& )      = default;
    InputOutputSocket( const InputOutputSocket& ) = delete;

    void read( hub::Data_t* data, hub::Size_t size ) override {
        net::ClientSocket::read( data, size );
    }

    void write( const hub::Data_t* data, hub::Size_t size ) override {
        net::ClientSocket::write( data, size );
    }

    void close() override { disconnect(); };

    bool isOpen() const override { return isConnected(); }

    bool isEnd() const override { return false; }

    void clear() override {}

    void setRetain( bool retain ) override {}

    bool operator==( const InputOutputSocket& inputOutputSocket ) const {
        return m_fdSock == inputOutputSocket.m_fdSock;
    }
};

} // namespace io
} // namespace hub
