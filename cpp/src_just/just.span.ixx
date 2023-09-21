
export module just.span;

export import just.std;
export import <compare>;

export namespace just {

  // я не добавил конструкторов в t_span для разных типов
  // так как считаю, что те типы пусть сами решают как формировать t_span
  // для итерирования запланированы отдельно рэнджи

  template <c_not_ref Type>
  struct t_span
  {
    using value_type = Type;
    using size_type = t_index;
    using pointer = t_pointer<value_type>;
    using reference = t_reference<value_type>;
    using const_reference = t_reference<const value_type>;

    // data
    pointer
      data{};
    size_type
      size{};

    constexpr reference operator [] (size_type pos)
    { return data[pos]; }

    constexpr const_reference operator [] (size_type pos) const
    { return data[pos]; }
  };

  // ==
  template <c_bool_comparable_equal_both Type>
  constexpr bool
    operator == (
      t_reference< const t_span<Type> > p1,
      t_reference< const t_span<Type> > p2
    )
  {
    if ( p1.size != p2.size )
    { return false; }

    for( t_index pos{0}; pos < p1.size; ++pos )
    {
      if( p1[pos] != p1[pos] )
      { return false; }
    }

    return true;
  }

  // <=>
  template <typename Type>
  constexpr auto
    operator <=> (
      t_reference< const t_span<Type> > p1,
      t_reference< const t_span<Type> > p2
    ) -> std::compare_three_way_result_t<Type>
    requires( not std::is_same_v<std::compare_three_way_result_t<Type>, void> )
  {
    using result_type = std::compare_three_way_result_t<Type>;

    t_index min_size = aux::min(p1.size, p2.size);

    for( t_index pos{0}; pos < min_size; ++pos )
    {
      result_type res{p1[pos] <=> p1[pos]};
      if( res != result_type::equivalent )
      { return res; }
    }

    return p1.size <=> p2.size;
  }

  //

  template <c_not_ref Type, t_index Size>
  struct t_span_exact
  {
    using value_type = Type;
    using size_type = decltype(Size);
    using pointer = t_pointer<value_type>;
    using reference = t_reference<value_type>;

    static constexpr inline const size_type
      size{Size};

    // data
    pointer
      data{};

    constexpr reference operator [] (size_type pos)
    { return data[pos]; }
  };

  // ==
  template <c_bool_comparable_equal_both Type, t_index Size_1, t_index Size_2>
  constexpr bool
    operator == (
      t_reference< const t_span_exact<Type, Size_1> > p1,
      t_reference< const t_span_exact<Type, Size_2> > p2
    )
  {
    if constexpr( Size_1 != Size_2 )
    { return false; }

    for( t_index pos{0}; pos < Size_1; ++pos )
    {
      if( p1[pos] != p1[pos] )
      { return false; }
    }

    return true;
  }

  // <=>
  template <typename Type, t_index Size_1, t_index Size_2>
  constexpr auto
    operator <=> (
      t_reference< const t_span_exact<Type, Size_1> > p1,
      t_reference< const t_span_exact<Type, Size_2> > p2
    ) -> std::compare_three_way_result_t<Type>
    requires( not std::is_same_v<std::compare_three_way_result_t<Type>, void> )
  {
    using result_type = std::compare_three_way_result_t<Type>;

    t_index min_size = aux::min(Size_1, Size_2);

    for( t_index pos{0}; pos < min_size; ++pos )
    {
      result_type res{p1[pos] <=> p1[pos]};
      if( res != result_type::equivalent )
      { return res; }
    }

    return p1.size <=> p2.size;
  }

} // ns just
