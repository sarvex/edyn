#ifndef EDYN_SERIALIZATION_MEMORY_ARCHIVE_HPP
#define EDYN_SERIALIZATION_MEMORY_ARCHIVE_HPP

#include <cstdint>
#include <type_traits>
#include <vector>
#include <map>
#include "edyn/util/tuple.hpp"

namespace edyn {

class memory_input_archive {
public:
    using data_type = uint8_t;
    using buffer_type = std::vector<data_type>;
    using is_input = std::true_type;
    using is_output = std::false_type;

    memory_input_archive(buffer_type &buffer)
        : m_buffer(&buffer)
        , m_position(0)
    {}

    template<typename T>
    void operator()(T& t) {
        if constexpr(has_type<T, archive_fundamental_types>::value) {
            read_bytes(t);
        } else {
            serialize(*this, t);
        }
    }

    template<typename... Ts>
    void operator()(Ts&... t) {
        (operator()(t), ...);
    }

protected:
    template<typename T>
    void read_bytes(T &t) {
        auto* buff = reinterpret_cast<const T*>(&(*m_buffer)[m_position]);
        t = *buff;
        m_position += sizeof(T);
    }

    buffer_type *m_buffer;
    size_t m_position;
};

class memory_output_archive {
public:
    using data_type = uint8_t;
    using buffer_type = std::vector<data_type>;
    using is_input = std::false_type;
    using is_output = std::true_type;

    memory_output_archive(buffer_type& buffer) 
        : m_buffer(&buffer) 
    {}

    template<typename T>
    void operator()(T& t) {
        if constexpr(has_type<T, archive_fundamental_types>::value) {
            write_bytes(t);
        } else {
            serialize(*this, t);
        }
    }

    template<typename... Ts>
    void operator()(Ts&... t) {
        (operator()(t), ...);
    }

protected:
    template<typename T>
    void write_bytes(T &t) { 
        auto idx = m_buffer->size();
        m_buffer->resize(idx + sizeof(T));
        auto *dest = reinterpret_cast<T*>(&(*m_buffer)[idx]);
        *dest = t;
    }

    buffer_type *m_buffer;
};

}

#endif // EDYN_SERIALIZATION_MEMORY_ARCHIVE_HPP