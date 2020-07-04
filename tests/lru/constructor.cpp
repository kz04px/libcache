#include <catch2/catch.hpp>
#include <libcache/lru.hpp>

TEST_CASE("LRU()") {
    CHECK_NOTHROW(libcache::LRU<int, int>(1));
    CHECK_THROWS(libcache::LRU<int, int>(0));
}
