#include <catch2/catch.hpp>
#include <libcache/lru.hpp>
#include <vector>

TEST_CASE("LRU::insert<overwrite>") {
    using key_type = int;
    using value_type = char;
    using entry = std::pair<key_type, value_type>;
    using expected = std::vector<entry>;

    const std::pair<entry, expected> tests[] = {
        // Add regular entry - cache not full
        {{1, 'a'}, {{1, 'a'}}},
        // Replace entry - only entry
        {{1, 'b'}, {{1, 'b'}}},
        // Add regular entry - cache not full
        {{2, 'b'}, {{2, 'b'}, {1, 'b'}}},
        // Replace entry - tail - cache not full
        {{1, 'c'}, {{1, 'c'}, {2, 'b'}}},
        // Replace entry - tail - cache not full
        {{2, 'e'}, {{2, 'e'}, {1, 'c'}}},
        // Add regular entry - cache not full
        {{3, 'c'}, {{3, 'c'}, {2, 'e'}, {1, 'c'}}},
        // Replace entry - tail
        {{1, 'd'}, {{1, 'd'}, {3, 'c'}, {2, 'e'}}},
        // Add regular entry - LRU entry overwritten
        {{4, 'd'}, {{4, 'd'}, {1, 'd'}, {3, 'c'}}},
        // Replace entry - middle
        {{1, 'e'}, {{1, 'e'}, {4, 'd'}, {3, 'c'}}},
        // Add regular entry - LRU entry overwritten
        {{2, 'g'}, {{2, 'g'}, {1, 'e'}, {4, 'd'}}},
        // Replace entry - head
        {{2, 'f'}, {{2, 'f'}, {1, 'e'}, {4, 'd'}}},
        // Add regular entry - LRU entry overwritten
        {{5, 'e'}, {{5, 'e'}, {2, 'f'}, {1, 'e'}}},
        {{1, 'a'}, {{1, 'a'}, {5, 'e'}, {2, 'f'}}},
        {{3, 'c'}, {{3, 'c'}, {1, 'a'}, {5, 'e'}}},
    };

    auto lru = libcache::LRU<key_type, value_type>(3);
    REQUIRE(lru.empty());
    REQUIRE(!lru.full());
    REQUIRE(lru.size() == 0);
    REQUIRE(lru.capacity() == 3);

    for (const auto &[input, data] : tests) {
        lru.insert(input.first, input.second);

        REQUIRE(!lru.empty());
        REQUIRE(lru.size() == data.size());
        REQUIRE(lru.full() == (data.size() == 3));

        for (const auto &[key, val] : data) {
            REQUIRE(lru.has(key));
            REQUIRE(lru.get(key).value() == val);
        }
    }

    REQUIRE(lru.full());
    lru.clear();
    REQUIRE(!lru.full());
    REQUIRE(lru.empty());
    REQUIRE(lru.size() == 0);
    REQUIRE(lru.capacity() == 3);
}

TEST_CASE("LRU::insert<quiet>") {
    using key_type = int;
    using value_type = char;
    using entry = std::pair<key_type, value_type>;
    using expected = std::vector<entry>;

    const std::pair<entry, expected> tests[] = {
        // Add regular entry - cache not full
        {{1, 'a'}, {{1, 'a'}}},
        // Replace entry - only entry (Nothing happens)
        {{1, 'b'}, {{1, 'a'}}},
        // Add regular entry - cache not full
        {{2, 'b'}, {{2, 'b'}, {1, 'a'}}},
        // Replace entry - tail - cache not full (Nothing happens)
        {{1, 'c'}, {{2, 'b'}, {1, 'a'}}},
        // Replace entry - tail - cache not full (Nothing happens)
        {{2, 'e'}, {{2, 'b'}, {1, 'a'}}},
        // Add regular entry - cache not full
        {{3, 'c'}, {{3, 'c'}, {2, 'b'}, {1, 'a'}}},
        // Replace entry - tail (Nothing happens)
        {{1, 'd'}, {{3, 'c'}, {2, 'b'}, {1, 'a'}}},
        // Add regular entry - LRU entry overwritten
        {{4, 'd'}, {{4, 'd'}, {3, 'c'}, {2, 'b'}}},
        // Replace entry - middle (Nothing happens)
        {{3, 'e'}, {{4, 'd'}, {3, 'c'}, {2, 'b'}}},
        // Add regular entry - LRU entry overwritten
        {{1, 'g'}, {{1, 'g'}, {4, 'd'}, {3, 'c'}}},
        // Replace entry - head (Nothing happens)
        {{2, 'f'}, {{2, 'f'}, {1, 'g'}, {4, 'd'}}},
        // Add regular entry - LRU entry overwritten
        {{5, 'e'}, {{5, 'e'}, {2, 'f'}, {1, 'g'}}},
        {{4, 'a'}, {{4, 'a'}, {5, 'e'}, {2, 'f'}}},
        {{3, 'c'}, {{3, 'c'}, {4, 'a'}, {5, 'e'}}},
    };

    auto lru = libcache::LRU<key_type, value_type, libcache::policy::Quiet>(3);
    REQUIRE(lru.empty());
    REQUIRE(!lru.full());
    REQUIRE(lru.size() == 0);
    REQUIRE(lru.capacity() == 3);

    for (const auto &[input, data] : tests) {
        lru.insert(input.first, input.second);

        REQUIRE(!lru.empty());
        REQUIRE(lru.size() == data.size());
        REQUIRE(lru.full() == (data.size() == 3));

        for (const auto &[key, val] : data) {
            REQUIRE(lru.has(key));
            REQUIRE(lru.get(key).value() == val);
        }
    }

    REQUIRE(lru.full());
    lru.clear();
    REQUIRE(!lru.full());
    REQUIRE(lru.empty());
    REQUIRE(lru.size() == 0);
    REQUIRE(lru.capacity() == 3);
}
