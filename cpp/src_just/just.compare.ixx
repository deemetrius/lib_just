
export module just.compare;

export import just.std;
export import just.conversion;
export import <compare>;

export namespace just {

  enum class t_compare
  {
    n_none,
    n_less,
    n_equal_partial,
    n_equal_weak,
    n_equal,
    n_greater,
    n_unordered
  };

  // conversion
  template <>
  struct cast_functor<t_compare>
  {
    using result_type = t_compare;

    constexpr result_type
      operator () (std::strong_ordering p) const
    {
      return
        p < 0 ? result_type::n_less :
        p > 0 ? result_type::n_greater : result_type::n_equal;
    }

    constexpr result_type
      operator () (std::weak_ordering p) const
    {
      return
        p < 0 ? result_type::n_less :
        p > 0 ? result_type::n_greater : result_type::n_equal_weak;
    }

    constexpr result_type
      operator () (std::partial_ordering p) const
    {
      return
        p < 0 ? result_type::n_less :
        p > 0 ? result_type::n_greater :
        p == 0 ? result_type::n_equal_partial : result_type::n_unordered;
    }
  };

  // global object
  constexpr inline cast_functor<t_compare>
    as_compare{};

  // operator ==
  template <c_any_of<std::strong_ordering, std::weak_ordering, std::partial_ordering> Order>
  constexpr bool
    operator == (Order p_order, t_compare p_compare)
  { return as_compare(p_order) == p_compare; }

  //

#if 0
  struct t_compare_flags {
    enum value_type : t_size
    {
      n_none      = 0b1000'0000,

      n_less      = 0b0000'0001,
      n_equal     = 0b0000'0010,
      n_greater   = 0b0000'0100,
      n_unordered = 0b0100'1000,

      n_strong    = 0b0001'0000,
      n_equal_weak      = 0b0010'0000,
      n_equal_partial   = 0b0100'0000,
    };

    // data
    value_type
      value;

    t_compare_flags() : value{n_none} {}
    t_compare_flags(value_type o, value_type p) : value{o | p} {}
  };
#endif

#if 0
  struct t_compare {
    using param_type = std::strong_ordering;

    static constexpr inline const int
      n_less{-1},
      n_equal{0},
      n_greater{+1};

    // data
    int
      value{};

    constexpr
      t_compare(param_type p)
      : value{p < 0 ? n_less : (p > 0 ? n_greater : n_equal)}
    {}

    constexpr
      t_compare(int p)
      : value{p < 0 ? n_less : (p > 0 ? n_greater : n_equal)}
    {}

    constexpr
      operator int () const
    { return this->value; }
  };

  struct t_compare_partial {
    using param_type = std::partial_ordering;

    static constexpr inline const int
      n_less{-1},
      n_equal{0},
      n_greater{+1};

    // data
    int
      value{};

    constexpr
      t_compare_partial(param_type p)
      : value{p < 0 ? n_less : (p > 0 ? n_greater : n_equal)}
    {}

    constexpr
      t_compare_partial(int p)
      : value{p < 0 ? n_less : (p > 0 ? n_greater : n_equal)}
    {}

    constexpr
      operator int () const
    { return this->value; }
  };

  bool operator == (const t_compare &, const t_compare_partial &) = delete;
#endif
} // ns just
