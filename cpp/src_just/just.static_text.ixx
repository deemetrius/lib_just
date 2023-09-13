
export module just.static_text;

export import just.std;
export import <string_view>;

export namespace just {

  template <typename T, t_index N>
    requires( N > 0 )
  struct t_array
  {
    using value_type = T;
    using size_type = decltype(N);

    static constexpr inline const size_type
      size{N};

    // data
    value_type
      data[size];
  };

  template <typename T, t_index N>
    requires( N > 0 )
  struct t_static_text
    : public t_array<T, N>
  {
    using base_type = t_array<T, N>;
    using typename base_type::size_type;
    using view_type = std::basic_string_view<T>;

    constexpr
      t_static_text(t_reference<const T [N]> p_str)
    { for( size_type i = 0; i != N; ++i ) { this->data[i] = p_str[i]; } }

    constexpr view_type
      sv() const
    { return {this->data, N}; }
  };

#if 0 && __cplusplus < 202002L
  template <typename T, t_index N>
  t_static_text(const T (&) [N]) -> t_static_text<T, N>;
#endif

  namespace literals_static_text {
  
    template <t_static_text S>
    consteval auto
      operator "" _st ()
    { return S; }
  
  } // ns

} // ns
