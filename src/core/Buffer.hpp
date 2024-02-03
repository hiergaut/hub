/// © 2021-2024 Hub, All Rights Reserved
/// @author gauthier <gauthierbouyjou@aol.com>
/// @date 2023/11/04

#pragma once

#include <array>
#include <type_traits>
#include <vector>

#include "Macros.hpp"
#include "Traits.hpp"

namespace hub {

enum BufferOption {
    StaticMemory  = 0x1,
    DynamicMemory = 0x2,
};

template <class Type, Size_t Size, BufferOption Option>
class BufferBase
{
  public:
    virtual Data_t* data()             = 0;
    virtual const Data_t* data() const = 0;
    virtual Size_t size() const        = 0;

    virtual CONSTEXPR20 std::string toString() const = 0;

#if CPP_VERSION >= 20
    using Span             = std::span<Type, Size>;
    virtual Span getSpan() = 0;
    void getData( Span& span ) const {
        const auto& internalSpan = getSpan();
        std::copy( internalSpan.begin(), internalSpan.end(), span.begin() );
    }
    void setData( const Span& span ) {
        const auto& internalSpan = getSpan();
        std::copy( span.begin(), span.end(), internalSpan.begin() );
    }
#endif
};

////////////////////////////////////////////////////////////////////

template <class Type,
          Size_t Size,
          BufferOption Option = ( Size <= MAX_STACK_SIZE ) ? ( StaticMemory ) : ( DynamicMemory )>
class Buffer : public BufferBase<Type, Size, Option>
{
  public:
    BufferOption _Option = Option;
};

////////////////////////////////////////////////////////////////////

template <class Type, Size_t Size>
class Buffer<Type, Size, StaticMemory> : public BufferBase<Type, Size, StaticMemory>
{
  public:
    template <class... Args>
    constexpr Buffer( Args&&... args ) : m_array { std::forward<Type&&>( args )... } {}

    template <Size_t i>
    constexpr Type get() const {
        return m_array.at( i );
    }

    CONSTEXPR20 std::string toString() const override {
        std::string str;
        str += "(static)";
        str += hub::to_string( m_array );
        return str;
    }

    Data_t* data() override { return (Data_t*)m_array.data(); }

    const Data_t* data() const override { return (const Data_t*)m_array.data(); }
    Size_t size() const override { return m_array.size(); }

#if CPP_VERSION >= 20
    using Span = typename BufferBase<Type, Size, StaticMemory>::Span;
    Span getSpan() override { return Span { m_array.begin(), m_array.end() }; }
#endif

  private:
    std::array<Type, Size> m_array;
};

////////////////////////////////////////////////////////////////////

template <class Type, Size_t Size>
class Buffer<Type, Size, DynamicMemory> : public BufferBase<Type, Size, DynamicMemory>
{
  public:
    constexpr Buffer() : m_vector( Size ) {}

    template <class... Args>
    constexpr Buffer( Args&&... args ) : m_vector { std::forward<Type&&>( args )... } {}

    CONSTEXPR20 std::string toString() const override {
        std::string str;
        str += "(dynamic)";
        str += hub::to_string( m_vector );
        return str;
    }

    Data_t* data() override { return (Data_t*)m_vector.data(); }
    const Data_t* data() const override { return (const Data_t*)m_vector.data(); }
    Size_t size() const override { return m_vector.size(); }

#if CPP_VERSION >= 20
    using Span = typename BufferBase<Type, Size, DynamicMemory>::Span;
    Span getSpan() override { return std::span<Type, Size> { m_vector.begin(), m_vector.end() }; }
#endif

  private:
    std::vector<Type> m_vector;
};

} // namespace hub
