
export module just.flags;

export import just.std;
export import just.errors;

import <type_traits>;
import <utility>;
import <limits>;
import <compare>;

export namespace just {

  template <typename T>
  concept c_flag_enum = std::is_scoped_enum_v<T>;

  template <c_flag_enum T>
  struct t_flags
  {
    using t_enum = T;
    using t_underlying = std::underlying_type_t<t_enum>;
    using t_value = t_size; //std::make_unsigned_t<t_underlying>;
    using t_limits = std::numeric_limits<t_value>;

    static constexpr inline const bool
      s_signed_hint = std::is_signed_v<t_underlying>;

    static constexpr inline const t_value
      s_zero{0},
      s_one{1};

    static constexpr inline const t_underlying
      s_bits{ static_cast<t_underlying>(t_limits::digits) },
      s_hint_bits{ static_cast<t_underlying>(
        s_signed_hint ? (- t_limits::digits) : 0
      ) };

    // static actions

    static constexpr t_value
      as_value(t_enum p)
    {
      if constexpr( s_signed_hint )
      {
        t_underlying ret{ std::to_underlying(p) };
        if( ret < 0 )
        {
          if( ret < s_hint_bits )
          { throw t_error_range{}; }
          return static_cast<t_value>(s_bits + ret);
        }
        if( ret >= s_bits )
        { throw t_error_range{}; }
        return static_cast<t_value>(ret);
      } else {
        t_value ret{std::to_underlying(p)};
        if( ret >= s_bits )
        { throw t_error_range{}; }
        return ret;
      }
    }

    static constexpr t_value
      as_flag(t_enum p)
    { return s_one << as_value(p); }

    template <c_same_type<t_enum> ... E>
    static constexpr t_value
      mix(E ... p)
    { return ( s_zero | ... | as_flag(p) ); }

    // data
    t_value
      value = 0;

    // check actions

    template <c_same_type<t_enum> ... E>
    constexpr bool
      has_all(t_enum p_first, E ... p_rest) const
    {
      t_value v{mix(p_first, p_rest ...)};
      return (value & v) == v;
    }

    template <c_same_type<t_enum> ... E>
    constexpr bool
      has_any(t_enum p_first, E ... p_rest) const
    {
      t_value v{mix(p_first, p_rest ...)};
      return (value & v) != s_zero;
    }

    template <c_same_type<t_enum> ... E>
    constexpr bool
      has_none(t_enum p_first, E ... p_rest) const
    {
      t_value v{mix(p_first, p_rest ...)};
      return (value & v) == s_zero;
    }

    constexpr bool
      empty() const
    { return value == s_zero; }

    // change actions

    template <c_same_type<t_enum> ... E>
    constexpr void
      set(t_enum p_first, E ... p_rest)
    {
      t_value v{mix(p_first, p_rest ...)};
      value |= v;
    }

    template <c_same_type<t_enum> ... E>
    constexpr void
      unset(t_enum p_first, E ... p_rest)
    {
      t_value v{mix(p_first, p_rest ...)};
      value &= compl v;
    }

    constexpr void
      reset()
    { value = s_zero; }

    //

    template <c_same_type<t_enum> ... E>
    constexpr t_flags
      operator () (t_enum p_first, E ... p_rest) const
    { return {value | mix(p_first, p_rest ...)}; }

    // bitwise assign

    constexpr t_reference<t_flags>
      operator |= (t_reference<const t_flags> p)
    {
      value |= p.value;
      return *this;
    }

    constexpr t_reference<t_flags>
      operator &= (t_reference<const t_flags> p)
    {
      value &= p.value;
      return *this;
    }

    constexpr t_reference<t_flags>
      operator ^= (t_reference<const t_flags> p)
    {
      value ^= p.value;
      return *this;
    }

    // bitwise

    friend constexpr t_flags
      operator | (t_reference<const t_flags> p1, t_reference<const t_flags> p2)
    { return {p1.value | p2.value}; }

    friend constexpr t_flags
      operator & (t_reference<const t_flags> p1, t_reference<const t_flags> p2)
    { return {p1.value & p2.value}; }

    friend constexpr t_flags
      operator ^ (t_reference<const t_flags> p1, t_reference<const t_flags> p2)
    { return {p1.value ^ p2.value}; }

    // comparison

    friend constexpr bool
      operator == (t_reference<const t_flags>, t_reference<const t_flags>) = default;

    friend constexpr std::strong_ordering
      operator <=> (t_reference<const t_flags>, t_reference<const t_flags>) = default;
  };

  template <auto First, decltype(First) ... Rest>
  requires ( c_flag_enum<decltype(First)> )
  constexpr inline const t_flags<decltype(First)>
    g_flags{t_flags<decltype(First)>::mix(First, Rest ...)};

  namespace detail {

    struct t_make_flags
    {
      template <c_flag_enum T>
      constexpr inline
        operator t_flags<T> () const
      { return t_flags<T>{}; }

      template <c_flag_enum T, c_same_type<T> ... E>
      constexpr t_flags<T>
        operator () (T p_first, E ... p_rest) const
      { return {t_flags<T>::mix(p_first, p_rest ...)}; }
    };

  } // ns

  constexpr inline const detail::t_make_flags
    g_make_flags;

} // ns
