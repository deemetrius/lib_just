
export module just.array_exact; // checked: 2023-10-03 #2

export import just.std;

export namespace just {

  // array_exact<Type, Size>
  template <c_not_ref Type, t_index Size>
    requires( Size > 0 )
  struct array_exact
  {
    using value_type = Type;
    using size_type = decltype(Size);

    using reference = ref_to<value_type>;
    using const_reference = ref_to<const value_type>;

    static constexpr inline const size_type
      size{ Size };

    // data
    value_type
      data[size];

    reference
      operator [] (size_type p_index)
    { return this->data[p_index]; }

    const_reference
      operator [] (size_type p_index) const
    { return this->data[p_index]; }
  };

} // ns just
