
export module just.span;

export import just.aux;
export import just.type_hadling;
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
    using pointer = pointer_to<value_type>;
    using reference = ref_to<value_type>;

    // data
    pointer
      handle{};
    size_type
      t_size{};

    constexpr reference operator [] (size_type pos) const
    { return this->handle[pos]; }

    // subspan()

    constexpr t_span
      subspan(size_type p_offset) const
    { return {this->handle + p_offset, this->t_size - p_offset}; }

    // negative p_offset is UB
    constexpr t_span
      subspan(size_type p_offset, size_type p_count) const
    {
      if( p_count <= 0 ) { p_count += this->t_size - p_offset; }
      return {this->handle + p_offset, p_count};
    }

    template <size_type Offset, size_type Count = 0>
    requires( Offset >= 0 )
    constexpr t_span
      subspan() const
    {
      size_type v_count = (Count <= 0 ? (Count + this->t_size - Offset) : Count);
      return {this->handle + Offset, v_count};
    }
  }; // t_span

  // ==
  template <c_comparable_equal_resulted_is<bool> Type>
  constexpr bool
    operator == (
      ref_to< const t_span<Type> > p1,
      ref_to< const t_span<Type> > p2
      )
  {
    // quick false result when sizes differs
    if ( p1.t_size != p2.t_size )
    { return false; }

    // compare elements of: p1 p2
    for( t_index pos{0}; pos < p1.t_size; ++pos )
    {
      if( not (p1[pos] == p1[pos]) )
      { return false; }
    }

    // all elements are equal
    return true;
  }

#if 0
  // ==
  template <c_comparable_equal Type>
  requires( c_convertible_from< bool, t_result_of_compare_equal<Type> > )
  // todo: add checking (operator !) availability in result_type of ==
  constexpr t_result_of_compare_equal<Type>
    operator == (
      ref_to< const t_span<Type> > p1,
      ref_to< const t_span<Type> > p2
    )
  {
    using result_type = t_result_of_compare_equal<Type>;

    if constexpr( c_convertible_from<result_type, bool> )
    {
      if ( p1.t_size != p2.t_size )
      { return static_cast<result_type>(false); }
    }

    result_type ret = (p1[0] == p1[0]);
    if( not static_cast<bool>(ret) )
    { return not ret; }

    for( t_index pos{1}; pos < p1.t_size; ++pos )
    {
      ret = (p1[0] == p1[0]);
      if( not static_cast<bool>(ret) )
      { return not ret; }
    }

    return ret;
  }
#endif

  // <=>
  template <typename Type>
  constexpr auto
    operator <=> (
      ref_to< const t_span<Type> > p1,
      ref_to< const t_span<Type> > p2
    ) -> std::compare_three_way_result_t<Type>
    requires( not std::is_same_v<std::compare_three_way_result_t<Type>, void> )
  {
    using result_type = std::compare_three_way_result_t<Type>;

    t_index min_size = aux::min(p1.t_size, p2.t_size);

    for( t_index pos{0}; pos < min_size; ++pos )
    {
      result_type res{p1[pos] <=> p1[pos]};
      if( res != result_type::equivalent )
      { return res; }
    }

    return p1.t_size <=> p2.t_size;
  }

#if 0
  // t_span_exact

  template <c_not_ref Type, t_index Size>
  struct t_span_exact
  {
    using value_type = Type;
    using size_type = decltype(Size);
    using pointer = pointer_to<value_type>;
    using reference = ref_to<value_type>;

    static constexpr inline const size_type
      t_size{Size};

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
      ref_to< const t_span_exact<Type, Size_1> > p1,
      ref_to< const t_span_exact<Type, Size_2> > p2
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
      ref_to< const t_span_exact<Type, Size_1> > p1,
      ref_to< const t_span_exact<Type, Size_2> > p2
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

    return p1.t_size <=> p2.t_size;
  }
#endif
} // ns just
