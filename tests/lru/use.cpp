#include <catch2/catch.hpp>
#include <libcache/lru.hpp>

TEST_CASE("LRU::use()") {
    auto lru = libcache::LRU<int, int>(3);

    // Fill cache
    // (3,2,1)
    lru.insert(1, 1);
    lru.insert(2, 2);
    lru.insert(3, 3);
    REQUIRE(lru.has(3));
    REQUIRE(lru.has(2));
    REQUIRE(lru.has(1));

    // Move entry to the front by using it
    // (3,2,1) ---> (1,3,2)
    lru.use(1);
    REQUIRE(lru.has(1));
    REQUIRE(lru.has(3));
    REQUIRE(lru.has(2));

    // insert
    // (1,3,2) ---> (4,1,3)
    lru.insert(4, 4);
    REQUIRE(lru.has(4));
    REQUIRE(lru.has(1));
    REQUIRE(lru.has(3));

    // More reorganising with 'use'
    // (4,1,3) ---> (1,4,3) ---> (3,1,4)
    lru.use(1);
    lru.use(3);
    REQUIRE(lru.has(3));
    REQUIRE(lru.has(1));
    REQUIRE(lru.has(4));

    // insert
    // (3,1,4) ---> (5,3,1)
    lru.insert(5, 5);
    REQUIRE(lru.has(5));
    REQUIRE(lru.has(3));
    REQUIRE(lru.has(1));

    // insert
    // (5,3,1) ---> (6,5,3)
    lru.insert(6, 6);
    REQUIRE(lru.has(6));
    REQUIRE(lru.has(5));
    REQUIRE(lru.has(3));

    REQUIRE(lru.size() == 3);
    REQUIRE(!lru.empty());
    REQUIRE(lru.full());
}
