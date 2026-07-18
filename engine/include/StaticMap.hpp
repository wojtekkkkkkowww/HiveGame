#pragma once

#include <array>
#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <utility>

namespace hive
{
template <typename Key, typename Value, std::size_t Capacity> class StaticMap
{
  public:
    using value_type = std::pair<Key, Value>;
    using iterator = typename std::array<value_type, Capacity>::iterator;
    using const_iterator = typename std::array<value_type, Capacity>::const_iterator;

    constexpr StaticMap() = default;

    constexpr StaticMap(std::initializer_list<value_type> values)
    {
        if (values.size() > Capacity)
        {
            throw std::length_error("StaticMap initializer exceeds capacity");
        }
        for (const auto &value : values)
        {
            insert(value.first, value.second);
        }
    }

    [[nodiscard]] constexpr std::size_t size() const noexcept { return count; }
    [[nodiscard]] static constexpr std::size_t capacity() noexcept { return Capacity; }
    [[nodiscard]] constexpr bool empty() const noexcept { return count == 0; }

    constexpr iterator begin() noexcept { return data.begin(); }
    constexpr const_iterator begin() const noexcept { return data.begin(); }
    constexpr iterator end() noexcept { return data.begin() + static_cast<std::ptrdiff_t>(count); }
    constexpr const_iterator end() const noexcept
    {
        return data.begin() + static_cast<std::ptrdiff_t>(count);
    }

    constexpr iterator find(const Key &key) noexcept
    {
        for (auto iterator = begin(); iterator != end(); ++iterator)
        {
            if (iterator->first == key)
            {
                return iterator;
            }
        }
        return end();
    }

    constexpr const_iterator find(const Key &key) const noexcept
    {
        for (auto iterator = begin(); iterator != end(); ++iterator)
        {
            if (iterator->first == key)
            {
                return iterator;
            }
        }
        return end();
    }

    [[nodiscard]] constexpr bool contains(const Key &key) const noexcept
    {
        return find(key) != end();
    }

    constexpr Value &at(const Key &key)
    {
        const auto iterator = find(key);
        if (iterator == end())
        {
            throw std::out_of_range("StaticMap key not found");
        }
        return iterator->second;
    }

    constexpr const Value &at(const Key &key) const
    {
        const auto iterator = find(key);
        if (iterator == end())
        {
            throw std::out_of_range("StaticMap key not found");
        }
        return iterator->second;
    }

    constexpr Value &operator[](const Key &key)
    {
        if (auto iterator = find(key); iterator != end())
        {
            return iterator->second;
        }
        ensureSpace();
        data[count] = value_type{key, Value{}};
        return data[count++].second;
    }

    constexpr void insert(const Key &key, const Value &value)
    {
        if (auto iterator = find(key); iterator != end())
        {
            iterator->second = value;
            return;
        }
        ensureSpace();
        data[count++] = value_type{key, value};
    }

    constexpr void erase(const Key &key)
    {
        const auto iterator = find(key);
        if (iterator == end())
        {
            return;
        }
        *iterator = std::move(data[count - 1]);
        data[count - 1] = value_type{};
        --count;
    }

    constexpr void clear()
    {
        while (count > 0)
        {
            data[--count] = value_type{};
        }
    }

  private:
    constexpr void ensureSpace() const
    {
        if (count >= Capacity)
        {
            throw std::length_error("StaticMap is full");
        }
    }

    std::array<value_type, Capacity> data{};
    std::size_t count = 0;
};
} // namespace hive
