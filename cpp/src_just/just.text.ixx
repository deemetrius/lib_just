
export module just.text; // checked: 2023-10-03 #2

// text is compile-time storage for template param
// todo: cast to span

export import just.array_exact;

export namespace just {

  // text<Type, Size>
  // Empty text (only zero char): Size == 1
  template <c_integral Type, t_index Size>
    requires( Size > 0 )
  struct text
    : public array_exact<Type, Size>
  {
    using base_type = array_exact<Type, Size>;

    using typename base_type::value_type;
    using typename base_type::size_type;

    using param_type = ref_to<const Type [Size]>;

    // For empty text: length == 0
    static constexpr inline const size_type
      length{ Size - 1 };

    constexpr
      text(param_type p_string)
    {
      for( size_type i = 0; i != Size; ++i )
      { this->data[i] = p_string[i]; }
    }

    constexpr
      text() : base_type{}
    {}
  };

  // copy_text<From>(to, to_position)
  template <text From>
  constexpr t_index
    copy_text(
      pointer_to<typename decltype(From)::value_type> to,
      t_index to_position
    )
  {
    for( t_index i = 0; i != From.t_size; ++i, ++to_position )
    { to[to_position] = From.data[i]; }

    return to_position - 1;
  }

  // concat_size<First, ... Rest>
  template <
    text First,
    text ... Rest
  >
  constexpr inline t_index
    concat_size{ First.length + (Rest.length + ...) + 1 };

  // concat_result<First, ... Rest>
  template <
    text First,
    text ... Rest
  >
  using concat_result = text<
    typename decltype(First)::value_type,
    concat_size<First, Rest ...>
  >;

  // concat<First, ... Rest>()
  template <
    text First,
    text ... Rest
  >
  constexpr concat_result<First, Rest ...>
    concat()
  {
    concat_result<First, Rest ...> ret;
    t_index pos{copy_text<First>(ret.data, 0)};
    ( (
      pos = copy_text<Rest>(ret.data, pos)
    ), ... );
    return ret;
  }

  // concat_separated_size<Separator, First, ... Rest>
  template <
    text Separator,
    text First,
    text ... Rest
  >
  constexpr inline t_index
    concat_separated_size
  { First.length + (Rest.length + ...) + 1 + Separator.length * sizeof...(Rest) };

  // concat_separated_result<Separator, First, ... Rest>
  template <
    text Separator,
    text First,
    text ... Rest
  >
  using concat_separated_result = text<
    typename decltype(First)::value_type,
    concat_separated_size<Separator, First, Rest ...>
  >;

  // concat_separated<Separator, First, ... Rest>()
  template <
    text Separator,
    text First,
    text ... Rest
  >
  constexpr concat_separated_result<Separator, First, Rest ...>
    concat_separated()
  {
    concat_separated_result<Separator, First, Rest ...> ret;
    t_index pos{ copy_text<First>(ret.data, 0) };
    ( (
      ( pos = copy_text<Separator>(ret.data, pos) ),
      ( pos = copy_text<Rest>(ret.data, pos) )
    ), ... );
    return ret;
  }

  //

  namespace literals_text {

    // ""_jt
    template <text S>
    constexpr auto
      operator "" _jt ()
    { return S; }

  } // ns literals_text

} // ns just
