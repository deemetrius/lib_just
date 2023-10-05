
export module just.flags;

export import just.std;
export import just.errors;

import <type_traits>;
import <utility>;
import <limits>;
import <compare>;

export namespace just {

  template <typename Type>
  concept c_flag_enum = std::is_scoped_enum_v<Type>;

  template <c_flag_enum Type>
  struct t_flags
  {
    using enum_type = Type;
    using underlying_type = std::underlying_type_t<enum_type>;
    using value_type = t_size; //std::make_unsigned_t<underlying_type>;
    using limits_type = std::numeric_limits<value_type>;

    static constexpr inline const bool
      s_signed_hint = std::is_signed_v<underlying_type>;

    static constexpr inline const value_type
      s_zero{0},
      s_one{1};

    static constexpr inline const underlying_type
      s_bits{ static_cast<underlying_type>(limits_type::digits) },
      s_hint_bits{ static_cast<underlying_type>(
        s_signed_hint ? (- limits_type::digits) : 0
      ) };

    // static actions

    static constexpr value_type
      as_value(enum_type p)
    {
      if constexpr( s_signed_hint )
      {
        underlying_type ret{ std::to_underlying(p) };
        if( ret < 0 )
        {
          if( ret < s_hint_bits )
          { throw t_error_range{}; }
          return static_cast<value_type>(s_bits + ret);
        }
        if( ret >= s_bits )
        { throw t_error_range{}; }
        return static_cast<value_type>(ret);
      } else {
        value_type ret{std::to_underlying(p)};
        if( ret >= s_bits )
        { throw t_error_range{}; }
        return ret;
      }
    }

    static constexpr value_type
      as_flag(enum_type p)
    { return s_one << as_value(p); }

    template <c_same_type<enum_type> ... E>
    static constexpr value_type
      mix(E ... p)
    { return ( s_zero | ... | as_flag(p) ); }

    // data
    value_type
      value = 0;

    // check actions

    template <c_same_type<enum_type> ... E>
    constexpr bool
      has_all(enum_type p_first, E ... p_rest) const
    {
      value_type v{mix(p_first, p_rest ...)};
      return (value & v) == v;
    }

    template <c_same_type<enum_type> ... E>
    constexpr bool
      has_any(enum_type p_first, E ... p_rest) const
    {
      value_type v{mix(p_first, p_rest ...)};
      return (value & v) != s_zero;
    }

    template <c_same_type<enum_type> ... E>
    constexpr bool
      has_none(enum_type p_first, E ... p_rest) const
    {
      value_type v{mix(p_first, p_rest ...)};
      return (value & v) == s_zero;
    }

    constexpr bool
      empty() const
    { return value == s_zero; }

    // change actions

    template <c_same_type<enum_type> ... E>
    constexpr void
      set(enum_type p_first, E ... p_rest)
    {
      value_type v{mix(p_first, p_rest ...)};
      value |= v;
    }

    template <c_same_type<enum_type> ... E>
    constexpr void
      unset(enum_type p_first, E ... p_rest)
    {
      value_type v{mix(p_first, p_rest ...)};
      value &= compl v;
    }

    constexpr void
      reset()
    { value = s_zero; }

    //

    template <c_same_type<enum_type> ... E>
    constexpr t_flags
      operator () (enum_type p_first, E ... p_rest) const
    { return {value | mix(p_first, p_rest ...)}; }

    // bitwise assign

    constexpr ref_to<t_flags>
      operator |= (ref_to<const t_flags> p)
    {
      value |= p.value;
      return *this;
    }

    constexpr ref_to<t_flags>
      operator &= (ref_to<const t_flags> p)
    {
      value &= p.value;
      return *this;
    }

    constexpr ref_to<t_flags>
      operator ^= (ref_to<const t_flags> p)
    {
      value ^= p.value;
      return *this;
    }

    // bitwise

    friend constexpr t_flags
      operator | (ref_to<const t_flags> p1, ref_to<const t_flags> p2)
    { return {p1.value | p2.value}; }

    friend constexpr t_flags
      operator & (ref_to<const t_flags> p1, ref_to<const t_flags> p2)
    { return {p1.value & p2.value}; }

    friend constexpr t_flags
      operator ^ (ref_to<const t_flags> p1, ref_to<const t_flags> p2)
    { return {p1.value ^ p2.value}; }

    // comparison

    friend constexpr bool
      operator == (ref_to<const t_flags>, ref_to<const t_flags>) = default;

    friend constexpr std::strong_ordering
      operator <=> (ref_to<const t_flags>, ref_to<const t_flags>) = default;
  };

  template <auto First, decltype(First) ... Rest>
  requires ( c_flag_enum<decltype(First)> )
  constexpr inline const t_flags<decltype(First)>
    g_flags{t_flags<decltype(First)>::mix(First, Rest ...)};

  namespace detail {

    struct t_make_flags
    {
      template <c_flag_enum Type>
      constexpr inline
        operator t_flags<Type> () const
      { return t_flags<Type>{}; }

      template <c_flag_enum Type, c_same_type<Type> ... E>
      constexpr t_flags<Type>
        operator () (Type p_first, E ... p_rest) const
      { return {t_flags<Type>::mix(p_first, p_rest ...)}; }
    };

  } // ns

  constexpr inline const detail::t_make_flags
    g_make_flags;

} // ns
