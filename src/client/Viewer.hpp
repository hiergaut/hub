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
 *   - Nicolas Mellado (IRIT)
 *   - Gauthier Bouyjou (IRIT)
 *******************************************************************************/

#pragma once

#include "viewer/ViewerServer2.hpp"

#include "io/input/InputStream.hpp"

namespace hub {
namespace client {

///
/// \brief Viewer
///
using Viewer = ViewerServer2;

static_assert( std::is_base_of<client::ViewerInterface<input::InputStream>, Viewer>::value,
               "ViewerInterface is base class of Viewer" );

} // namespace client
} // namespace hub
