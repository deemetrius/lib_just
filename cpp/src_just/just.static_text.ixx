
export module just.static_text;

export import just.array;
export import <string_view>;

export namespace just {

  template <typename T, t_index N>
    requires( N > 0 )
  struct t_static_text
    : public t_array<T, N>
  {
    using source_type = t_reference<const T [N]>;
    using base_type = t_array<T, N>;
    using typename base_type::value_type;
    using typename base_type::size_type;
    using view_type = std::basic_string_view<T>;

    constexpr
      t_static_text(source_type p_str)
    {
      for( size_type i = 0; i != N; ++i ) { this->data[i] = p_str[i]; }
    }

    constexpr
      t_static_text() = default;

    constexpr view_type
      sv() const
    { return {this->data, N}; }
  };

#if 0 && __cplusplus < 202002L
  template <typename T, t_index N>
  t_static_text(const T (&) [N]) -> t_static_text<T, N>;
#endif

  //

  template <t_static_text From>
  constexpr t_index
    copy_st(t_pointer<typename decltype(From)::value_type> to, t_index to_position)
  {
    for( t_index i = 0; i != From.size; ++i, ++to_position ) { to[to_position] = From.data[i]; }
    return to_position - 1;
  }

  template <t_static_text First, t_static_text ... Rest>
  constexpr inline t_index
    g_static_implode_size = First.size + (Rest.size + ...) - sizeof...(Rest);

  template <t_static_text First, t_static_text ... Rest>
  using t_static_implode_result = t_static_text<
    typename decltype(First)::value_type,
    g_static_implode_size<First, Rest ...>
  >;

  template <t_static_text First, t_static_text ... Rest>
  constexpr t_static_implode_result<First, Rest ...>
    static_implode()
  {
    t_static_implode_result<First, Rest ...> ret;
    t_index pos{copy_st<First>(ret.data, 0)};
    ( (pos = copy_st<Rest>(ret.data, pos)), ... );
    return ret;
  }

  namespace literals_static_text {
  
    template <t_static_text S>
    consteval auto
      operator "" _st ()
    { return S; }
  
  } // ns

} // ns
