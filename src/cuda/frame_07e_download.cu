/*
 * Copyright 2016, Simula Research Laboratory
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#include "onoff.h"

// #include <iostream>
// #include <algorithm>
// #include <limits>
#include <cuda_runtime.h>
// #include <cub/cub.cuh>
// #include <thrust/system/cuda/detail/cub/cub.cuh>
// #include <stdio.h>
// #include "debug_macros.hpp"
// #include "debug_is_on_edge.h"

#include "frame.h"
// #include "assist.h"

using namespace std;

namespace popart
{

/* After vote_eval_chosen, _voters is no longer changed
 * we can copy it to the host for edge linking
 */

__host__
void Frame::applyVoteDownload( )
{
#ifdef EDGE_LINKING_HOST_SIDE
    /* After vote_eval_chosen, _voters is no longer changed
     * we can copy it to the host for edge linking
     */
    _voters.copySizeFromDevice( _stream, EdgeListWait );
    _voters.copyDataFromDeviceAsync( _download_stream );
    _v_chosen_idx.copyDataFromDeviceAsync( _voters.host.size, _download_stream );
    _inner_points.copyDataFromDeviceAsync( _download_stream );
#endif // EDGE_LINKING_HOST_SIDE
}

} // namespace popart

