
export module just.array;

export import just.std;

export namespace just {

  template <c_not_ref Type, t_index Size>
    requires( Size > 0 )
  struct t_array
  {
    using value_type = Type;
    using size_type = decltype(Size);

    static constexpr inline const size_type
      size{Size};

    // data
    value_type
      data[size];
  };

} // ns just
