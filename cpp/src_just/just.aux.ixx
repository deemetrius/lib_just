
export module just.aux;

export import just.std;

export namespace just {

  // entire_size<>()
  template <c_allocatable Type>
  constexpr inline t_size
    entire_size(t_index p_count)
  {
    return sizeof(Type) * static_cast<t_size>(p_count);
  }

  //

  template <typename Value, typename Status>
  struct t_result
  {
    using value_type = Value;
    using status_type = Status;

    // data
    value_type
      value{};
    [[no_unique_address]]
    status_type
      status{};
  };

  //

  namespace aux {

    // min
    template <typename First, typename Second, typename ... Rest>
    constexpr First
      min(First p_first, Second p_second, Rest ... p_rest)
    {
      if constexpr( sizeof...(Rest) == 0 )
      {
        return (p_first < p_second ? p_first : p_second);
      } else {
        return (p_first < p_second ? p_first : min(p_second, p_rest ...) );
      }
    }

    // g_min
    template <auto First, auto Second, auto ... Rest>
    constexpr const decltype(First)
      g_min{ min(First, Second, Rest ...) };

  } // ns aux

} // ns just
