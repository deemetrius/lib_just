
export module just.conversion;

export namespace just {

  // conversion of types

  template <typename Result>
  struct t_cast;

  // operator |
  template <typename From, typename Result>
  constexpr Result
    operator | (From p_from, const t_cast<Result> p_cast)
  { return p_cast(p_from); }

  // global template object
  template <typename Result>
  constexpr inline t_cast<Result>
    as_type{};

} // ns just
