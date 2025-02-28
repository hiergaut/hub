/*******************************************************************************
 * Copyright (c) 2021 IRIT, computer science research laboratory, Toulouse, France.
 * Please visit https://www.irit.fr/tplay/.
 *
 * All rights reserved.
 * This code belongs to tplay/hub project (https://github.com/T-PLAY/hub).
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 *
 * Initial Contributors:
 *   - Gauthier Bouyjou
 *******************************************************************************/

#pragma once

#include <fstream>

#include "core/Output.hpp"
#include "io/File.hpp"
#include "io/Header.hpp"

namespace hub {
namespace output {

///
/// \brief The OutputFile class
/// is an implement of File only used by output communication
///
class SRC_API OutputFile : public Output, public io::File
{
  public:
    using io::File::getHeader;
    using Output::write;

    ///
    /// \brief OutputFile
    /// \param header
    /// \param filePath
    ///
    explicit OutputFile( const io::Header& header, const std::string& filePath );

  public:
    ///
    /// \brief isOpen
    /// \return
    ///
    bool isOpen() const override;

    ///
    /// \brief close
    ///
    void close() override;
    ///
    /// \brief write
    /// \param data
    /// \param len
    ///
    void write( const Data_t* data, Size_t len ) override;
    ///
    /// \brief setRetain
    /// \param retained
    ///
    void setRetain( bool retained ) override;

  private:
    std::ofstream m_file;
};

} // namespace output
} // namespace hub
