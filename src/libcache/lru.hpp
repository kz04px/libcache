#ifndef LIBCACHE_LRU_HPP
#define LIBCACHE_LRU_HPP

#include <list>
#include <map>
#include <optional>
#include <stdexcept>

namespace libcache {

enum class policy : bool
{
    Overwrite,
    Quiet
};

enum class poll : bool
{
    Update,
    Quiet
};

template <class K, class V, policy default_policy = policy::Overwrite, poll default_poll = poll::Quiet>
class LRU {
   public:
    typedef K key_type;
    typedef V value_type;
    typedef std::list<key_type> list_type;
    typedef std::pair<value_type, typename list_type::iterator> pair_type;
    typedef std::map<key_type, pair_type> map_type;

    LRU(const std::size_t capacity) : m_capacity{capacity}, m_entries{}, m_lut{} {
        if (capacity < 1) {
            throw std::out_of_range("LRU cache capacity must be >= 1");
        }
    }

    ~LRU() {
    }

    void clear() noexcept {
        m_entries.clear();
        m_lut.clear();
    }

    [[nodiscard]] std::size_t capacity() const noexcept {
        return m_capacity;
    }

    [[nodiscard]] std::size_t size() const noexcept {
        return m_entries.size();
    }

    [[nodiscard]] bool has(const key_type &key) const noexcept {
        return m_lut.find(key) != m_lut.end();
    }

    [[nodiscard]] bool full() const noexcept {
        return size() == capacity();
    }

    [[nodiscard]] bool empty() const noexcept {
        return m_entries.empty();
    }

    template <policy p = default_policy>
    void insert(const key_type &key, const value_type &value) noexcept {
        const auto iter = m_lut.find(key);
        // Overwrite lru or create new entry
        if (iter == m_lut.end()) {
            if (full()) {
                pop_back();
            }
            m_entries.push_front(key);
            m_lut[key] = {value, m_entries.begin()};
        }
        // Overwrite existing entry
        else if constexpr (p == policy::Overwrite) {
            m_entries.erase(iter->second.second);
            m_entries.push_front(key);
            m_lut[key] = {value, m_entries.begin()};
        }
    }

    template <poll p = default_poll>
    [[nodiscard]] std::optional<value_type> get(const key_type &key) noexcept {
        const auto i = m_lut.find(key);
        if (i == m_lut.end()) {
            return {};
        }
        if constexpr (p == poll::Update) {
            const auto value = i->second.first;
            use(key, i);
            return value;
        } else {
            return i->second.first;
        }
    }

    void use(const key_type &key) noexcept {
        const auto iter = m_lut.find(key);
        if (iter == m_lut.end()) {
            return;
        }
        use(key, iter);
    }

   private:
    void use(const key_type &key, const typename map_type::iterator &i) noexcept {
        const auto &[value, iter] = i->second;
        m_entries.erase(iter);
        m_entries.push_front(key);
        m_lut[key] = {value, m_entries.begin()};
    }

    void pop_back() noexcept {
        const auto iter = --m_entries.end();
        m_lut.erase(*iter);
        m_entries.erase(iter);
    }

    const std::size_t m_capacity;
    list_type m_entries;
    map_type m_lut;
};

}  // namespace libcache

#endif
