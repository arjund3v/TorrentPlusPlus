cmake_minimum_required(VERSION 4.1)
project(TorrentPlusPlus)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)
include_directories(include/app)
include_directories(include/bencode)
include_directories(include/peer)
include_directories(include/torrent)
include_directories(include/tracker)
include_directories(include/util)

add_executable(TorrentPlusPlus
        include/app/cli.hpp
        include/bencode/bencode.hpp
        include/bencode/value.hpp
        include/peer/peer_client.hpp
        include/torrent/infohash.hpp
        include/torrent/metainfo.hpp
        include/tracker/http_tracker.hpp
        include/util/bytes.hpp
        include/util/sha1.hpp
        include/util/url_encode.hpp
        src/app/cli.cpp
        src/bencode/bencode.cpp
        src/peer/peer_client.cpp
        src/torrent/infohash.cpp
        src/torrent/metainfo.cpp
        src/tracker/http_tracker.cpp
        src/util/bytes.cpp
        src/util/sha1.cpp
        src/util/url_encode.cpp
        src/main.cpp
        tests/test_bencode.cpp)
