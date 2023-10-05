
export module just.variadic;

import <type_traits>;

export namespace just { namespace ex {

  template <typename ... Types>
  struct variadic;

  template <typename V1, typename V2>
  struct tt_join;

  template <typename ... V1, typename ... V2>
  struct tt_join< variadic<V1 ...>, variadic<V2 ...> >
  {
    using type = variadic<V1 ..., V2 ...>;
  };

  template <typename V, template <typename> typename Predicate>
  struct tt_filter;

  template <typename First, typename ... Rest, template <typename> typename Predicate>
  struct tt_filter<variadic<First, Rest ...>, Predicate>
  {
    using type = std::conditional_t<sizeof...(Rest) == 0,
      std::conditional_t<Predicate<First>::value, variadic<First>, variadic<> >,
      std::conditional_t<Predicate<First>::value, tt_join<variadic<First>, tt_filter<variadic<Rest ...>, Predicate> >, tt_filter<variadic<Rest ...>, Predicate> >
    >;
  };

  template <typename ... Types>
  struct variadic<Types ...>
  {
    template <typename V>
    using t_join = tt_join<variadic<Types ...>, V>;

    template <template <typename> typename Predicate>
    using t_filter = tt_filter<variadic<Types ...>, Predicate>;
  };

} } // ns just ex
