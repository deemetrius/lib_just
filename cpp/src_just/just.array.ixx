
export module just.array;

export import just.std;

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

} // ns just
