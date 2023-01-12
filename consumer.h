#pragma once
#include <algorithm>
#include <string_view>

class consumer
{
public:
    explicit consumer(const std::string& string) : m_string(string), m_index(0U) {}

    void advance(size_t view_size) { m_index += view_size; }

    bool eof() const { return m_index >= m_string.size(); }

    std::string_view peek(size_t view_size) const { return std::string_view(m_string.data() + m_index, clamp_view_size(view_size)); }

    std::string_view consume(size_t view_size)
    {
        const auto clamped_view_size = clamp_view_size(view_size);
        const auto consumed = peek(clamped_view_size);
        m_index += clamped_view_size;
        return consumed;
    }

private:
    size_t clamp_view_size(size_t view_size) const
    {
        if (eof()) return 0;
        const size_t remaining_characters = m_string.size() - m_index;
        return std::clamp(view_size, 0ULL, remaining_characters);
    }

    std::string m_string;
    size_t m_index;
};
