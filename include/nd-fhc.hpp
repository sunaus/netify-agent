// Netify Agent
// Copyright (C) 2015-2024 eGloo Incorporated
// <http://www.egloo.ca>
//
// This program is free software: you can redistribute it
// and/or modify it under the terms of the GNU General
// Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your
// option) any later version.
//
// This program is distributed in the hope that it will be
// useful, but WITHOUT ANY WARRANTY; without even the
// implied warranty of MERCHANTABILITY or FITNESS FOR A
// PARTICULAR PURPOSE.  See the GNU General Public License
// for more details.
//
// You should have received a copy of the GNU General Public
// License along with this program.  If not, see
// <http://www.gnu.org/licenses/>.

#pragma once

#include <ostream>
#include <string>
#include <vector>

#include "nd-apps.hpp"
#include "nd-flow.hpp"
#include "nd-protos.hpp"
#include "nd-util.hpp"

class ndFlowHashCacheEntry
{
public:
    ndFlowHashCacheEntry()
      : app_id(ND_APP_UNKNOWN), proto_id(ndProto::Id::UNKNOWN) { }
    ndFlowHashCacheEntry(const nd_flow_ptr &flow)
      : digest(flow->digest_mdata.begin(),
          flow->digest_mdata.end()),
        app_id(flow->detected_application),
        proto_id(flow->detected_protocol) { }

    friend std::ostream &operator<<(std::ostream &stream,
      const ndFlowHashCacheEntry &entry) {
        std::string _digest;
        nd_sha1_to_string(entry.digest, _digest);
        stream << _digest << "," << entry.app_id << ","
               << static_cast<unsigned>(entry.proto_id);
        return stream;
    }

    std::vector<uint8_t> digest;
    nd_app_id_t app_id;
    ndProto::Id proto_id;
};

class ndFlowHashCache :
  public ndLRUCache<std::string, ndFlowHashCacheEntry>
{
public:
    ndFlowHashCache(size_t cache_size);

    inline void Insert(const nd_flow_ptr &flow) {
        const std::string key(flow->digest_lower.begin(),
          flow->digest_lower.end());
        CacheInsert(key, ndFlowHashCacheEntry(flow));
    }

    inline void Insert(const std::vector<uint8_t> &digest_lower,
      const ndFlowHashCacheEntry &entry) {
        const std::string key(digest_lower.begin(),
          digest_lower.end());
        CacheInsert(key, entry);
    }

    inline bool Lookup(const std::vector<uint8_t> &digest_lower,
      ndFlowHashCacheEntry &entry) {
        const std::string key(digest_lower.begin(),
          digest_lower.end());
        if (! CacheLookup(key, entry)) return false;
        return true;
    }

    void Load(void);
    void Save(void) const;
};
