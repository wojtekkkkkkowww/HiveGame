#pragma once

#include <array>
#include <cstddef>
#include <initializer_list>
#include <stdexcept>
#include <utility>

namespace hive
{
template <typename T, std::size_t Capacity> class StaticVector
{
  public:
    using iterator = typename std::array<T, Capacity>::iterator;
    using const_iterator = typename std::array<T, Capacity>::const_iterator;

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

    constexpr T &operator[](std::size_t index) noexcept { return data[index]; }
    constexpr const T &operator[](std::size_t index) const noexcept { return data[index]; }

    constexpr T &back() noexcept { return data[count - 1]; }
    constexpr const T &back() const noexcept { return data[count - 1]; }

    constexpr void push_back(const T &value)
    {
        ensureCapacity();
        data[count++] = value;
    }

    constexpr void push_back(T &&value)
    {
        ensureCapacity();
        data[count++] = std::move(value);
    }

    constexpr void pop_back() noexcept
    {
        if (count > 0)
        {
            --count;
        }
    }

    constexpr void clear() noexcept { count = 0; }

  private:
    constexpr void ensureCapacity() const
    {
        if (count >= Capacity)
        {
            throw std::length_error("StaticVector capacity exceeded");
        }
    }

    std::array<T, Capacity> data{};
    std::size_t count = 0;
};

template <typename T, std::size_t Capacity> class StaticSet
{
  public:
    using iterator = typename StaticVector<T, Capacity>::iterator;
    using const_iterator = typename StaticVector<T, Capacity>::const_iterator;

    [[nodiscard]] constexpr std::size_t size() const noexcept { return values.size(); }
    [[nodiscard]] static constexpr std::size_t capacity() noexcept { return Capacity; }
    [[nodiscard]] constexpr bool empty() const noexcept { return values.empty(); }

    constexpr iterator begin() noexcept { return values.begin(); }
    constexpr const_iterator begin() const noexcept { return values.begin(); }
    constexpr iterator end() noexcept { return values.end(); }
    constexpr const_iterator end() const noexcept { return values.end(); }

    constexpr iterator find(const T &value) noexcept
    {
        for (auto iterator = begin(); iterator != end(); ++iterator)
        {
            if (*iterator == value)
            {
                return iterator;
            }
            if (value < *iterator)
            {
                break;
            }
        }
        return end();
    }

    constexpr const_iterator find(const T &value) const noexcept
    {
        for (auto iterator = begin(); iterator != end(); ++iterator)
        {
            if (*iterator == value)
            {
                return iterator;
            }
            if (value < *iterator)
            {
                break;
            }
        }
        return end();
    }

    [[nodiscard]] constexpr bool contains(const T &value) const noexcept
    {
        return find(value) != end();
    }

    constexpr void insert(const T &value)
    {
        std::size_t insertIndex = 0;
        while (insertIndex < values.size() && values[insertIndex] < value)
        {
            ++insertIndex;
        }

        if (insertIndex < values.size() && values[insertIndex] == value)
        {
            return;
        }

        values.push_back(value);
        for (std::size_t index = values.size() - 1; index > insertIndex; --index)
        {
            values[index] = std::move(values[index - 1]);
        }
        values[insertIndex] = value;
    }

    constexpr void clear() noexcept { values.clear(); }

  private:
    StaticVector<T, Capacity> values{};
};

template <typename T, std::size_t Capacity> class StaticQueue
{
  public:
    [[nodiscard]] constexpr std::size_t size() const noexcept { return count; }
    [[nodiscard]] static constexpr std::size_t capacity() noexcept { return Capacity; }
    [[nodiscard]] constexpr bool empty() const noexcept { return count == 0; }

    constexpr T &front() noexcept { return data[head]; }
    constexpr const T &front() const noexcept { return data[head]; }

    constexpr void push(const T &value)
    {
        ensureCapacity();
        data[tail] = value;
        tail = increment(tail);
        ++count;
    }

    constexpr void push(T &&value)
    {
        ensureCapacity();
        data[tail] = std::move(value);
        tail = increment(tail);
        ++count;
    }

    constexpr void pop() noexcept
    {
        if (count == 0)
        {
            return;
        }
        head = increment(head);
        --count;
    }

    constexpr void clear() noexcept
    {
        head = 0;
        tail = 0;
        count = 0;
    }

  private:
    [[nodiscard]] static constexpr std::size_t increment(std::size_t index) noexcept
    {
        return (index + 1) % Capacity;
    }

    constexpr void ensureCapacity() const
    {
        if (count >= Capacity)
        {
            throw std::length_error("StaticQueue capacity exceeded");
        }
    }

    std::array<T, Capacity> data{};
    std::size_t head = 0;
    std::size_t tail = 0;
    std::size_t count = 0;
};

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
