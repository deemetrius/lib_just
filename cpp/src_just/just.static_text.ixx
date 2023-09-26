
export module just.static_text;

export import just.array;
export import just.span;
export import <string_view>;

export namespace just {

  // t_static_text
  template <c_not_ref Type, t_index N>
    requires( N > 0 )
  struct t_static_text
    : public t_array<Type, N>
  {
    using source_type = t_reference<const Type [N]>;
    using base_type = t_array<Type, N>;
    using typename base_type::value_type;
    using typename base_type::size_type;
    using view_type = std::basic_string_view<Type>;

    static constexpr inline const size_type
      length{N - 1};

    constexpr
      t_static_text(source_type p_str)
    {
      for( size_type i = 0; i != N; ++i ) { this->data[i] = p_str[i]; }
    }

    constexpr
      t_static_text() = default;

    constexpr view_type
      sv() const
    { return {this->data, length}; }

    constexpr t_span<const value_type>
      span() const
    { return {this->data, length}; }
  };

#if 0 && __cplusplus < 202002L
  // deduction guide
  template <typename Type, t_index N>
  t_static_text(const Type (&) [N]) -> t_static_text<Type, N>;
#endif

  // copy_st()
  template <t_static_text From>
  constexpr t_index
    copy_st(t_pointer<typename decltype(From)::value_type> to, t_index to_position)
  {
    for( t_index i = 0; i != From.size; ++i, ++to_position ) { to[to_position] = From.data[i]; }
    return to_position - 1;
  }

  // g_static_concat_size
  template <
    t_static_text First,
    t_static_text ... Rest
  >
  constexpr inline t_index
    g_static_concat_size{ First.length + (Rest.length + ...) + 1 };

  // t_static_concat_result
  template <
    t_static_text First,
    t_static_text ... Rest
  >
  using t_static_concat_result = t_static_text<
    typename decltype(First)::value_type,
    g_static_concat_size<First, Rest ...>
  >;

  // static_concat()
  template <
    t_static_text First,
    t_static_text ... Rest
  >
  constexpr t_static_concat_result<First, Rest ...>
    static_concat()
  {
    t_static_concat_result<First, Rest ...> ret;
    t_index pos{copy_st<First>(ret.data, 0)};
    ( (
      pos = copy_st<Rest>(ret.data, pos)
    ), ... );
    return ret;
  }

  // g_static_concat_separated_size
  template <
    t_static_text Separator,
    t_static_text First,
    t_static_text ... Rest
  >
  constexpr inline t_index
    g_static_concat_separated_size
  { First.length + (Rest.length + ...) + 1 + Separator.length * sizeof...(Rest) };

  // t_static_concat_separated_result
  template <
    t_static_text Separator,
    t_static_text First,
    t_static_text ... Rest
  >
  using t_static_concat_separated_result = t_static_text<
    typename decltype(First)::value_type,
    g_static_concat_separated_size<Separator, First, Rest ...>
  >;

  // static_concat_separated()
  template <
    t_static_text Separator,
    t_static_text First,
    t_static_text ... Rest
  >
  constexpr t_static_concat_separated_result<Separator, First, Rest ...>
    static_concat_separated()
  {
    t_static_concat_separated_result<Separator, First, Rest ...> ret;
    t_index pos{copy_st<First>(ret.data, 0)};
    ( (
      ( pos = copy_st<Separator>(ret.data, pos) ),
      ( pos = copy_st<Rest>(ret.data, pos) )
    ), ... );
    return ret;
  }

  namespace literals_static_text {

    // ""_st
    template <t_static_text S>
    constexpr auto
      operator "" _st ()
    { return S; }

  } // ns

} // ns just
