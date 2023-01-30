/*
 * Copyright (c) 2021 Alfredo Mazzinghi
 * All rights reserved.
 *
 * This software was developed by SRI International and the University of
 * Cambridge Computer Laboratory (Department of Computer Science and
 * Technology) under DARPA contract HR0011-18-C-0016 ("ECATS"), as part of the
 * DARPA SSITH research programme.
 *
 * @BERI_LICENSE_HEADER_START@
 *
 * Licensed to BERI Open Systems C.I.C. (BERI) under one or more contributor
 * license agreements.  See the NOTICE file distributed with this work for
 * additional information regarding copyright ownership.  BERI licenses this
 * file to you under the BERI Hardware-Software License, Version 1.0 (the
 * "License"); you may not use this file except in compliance with the
 * License.  You may obtain a copy of the License at:
 *
 *   http://www.beri-open-systems.org/legal/license-1-0.txt
 *
 * Unless required by applicable law or agreed to in writing, Work distributed
 * under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
 * CONDITIONS OF ANY KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations under the License.
 *
 * @BERI_LICENSE_HEADER_END@
 */

/*
 * Internal instruction logging metric aggregation
 */

#pragma once

#include <map>
#include <perfetto.h>
#include <boost/icl/interval_map.hpp>

namespace cheri
{
class qemu_stats
{
    /*
     * Histogram of basic-block hit during tracing.
     */
    boost::icl::interval_map<uint64_t, uint64_t> bb_hit_map;
    /*
     * Histogram of branch instruction addresses.
     * Post-processing must be used to distinguish between function
     * calls and conditional branches.
     */
    std::map<uint64_t, uint64_t> branch_map;
    /*
     *  PC-tracking info to detect branches.
     */
    uint64_t last_pc;
    uint64_t pc_range_start;

  public:
    qemu_stats() : last_pc(0), pc_range_start(0) {}
    void process_next_pc(uint64_t pc, int insn_size);
    void flush(perfetto::Track &track);
    void clear();
};

} // namespace cheri
