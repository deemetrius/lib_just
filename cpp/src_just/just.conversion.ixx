
export module just.conversion;

// conversion of types
// 
// usage: value |as_type<Result>
// definition:
// * specialize: template <> struct cast_functor<Desired_result> {};
// * add overloading for member: constexpr Desired_result operator () (From param) const

export namespace just {

  template <typename Result>
  struct cast_functor;

  // operator |
  template <typename From, typename Result>
  constexpr Result
    operator | (From p_from, const cast_functor<Result> p_cast)
  { return p_cast(p_from); }

  // as_type<Result>
  template <typename Result>
  constexpr inline const cast_functor<Result>
    as_type{ };

} // ns just
