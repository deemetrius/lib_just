
export module just.iteration;

export namespace just {

  enum class t_direction { n_forward, n_reverse };

  struct t_sentinel {};

#if 0
  template <t_direction Direction>
  struct t_iterator
  {
    using iterator_category =	std::contiguous_iterator_tag;

    using value_type = t_self::value_type;
    using difference_type =	t_self::size_type;
    using pointer	= t_self::pointer;
    using reference	= t_self::reference;

    static constexpr inline const t_direction
      s_direction{Direction};

    // data
    pointer
      m_current, m_end;

    inline reference
      operator * ()
    { return *m_current; }

    inline pointer
      operator -> ()
    { return m_current; }

    t_reference<t_iterator>
      operator ++ ()
    {
      if constexpr( s_direction == t_direction::n_forward )
      { ++m_current; } else { --m_current; }
      return *this;
    }

    t_reference<t_iterator>
      operator -- ()
    {
      if constexpr( s_direction == t_direction::n_forward )
      { --m_current; } else { ++m_current; }
      return *this;
    }

    t_reference<t_iterator>
      operator += (difference_type p_delta)
    {
      if constexpr( s_direction == t_direction::n_forward )
      { m_current += p_delta; } else { m_current -= p_delta; }
      return *this;
    }

    t_reference<t_iterator>
      operator -= (difference_type p_delta)
    {
      if constexpr( s_direction == t_direction::n_forward )
      { m_current -= p_delta; } else { m_current += p_delta; }
      return *this;
    }

    friend t_iterator
      operator + (t_reference<const t_iterator> p, difference_type p_delta)
    {
      if constexpr( s_direction == t_direction::n_forward )
      { return {p.m_current + p_delta, p.m_end}; } else
      { return {p.m_current - p_delta, p.m_end}; }
    }

    friend t_iterator
      operator - (t_reference<const t_iterator> p, difference_type p_delta)
    {
      if constexpr( s_direction == t_direction::n_forward )
      { return {p.m_current - p_delta, p.m_end}; } else
      { return {p.m_current + p_delta, p.m_end}; }
    }

    friend bool
      operator == (
        t_reference<const t_iterator> p1,
        t_reference<const t_iterator> p2
        )
    { return p1.m_current == p2.m_current; }

    friend bool
      operator == (t_reference<const t_iterator> p, sentinel_type)
    { return p.m_current == p.m_end; }

    friend std::strong_ordering
      operator <=> (
        t_reference<const t_iterator> p1,
        t_reference<const t_iterator> p2
        )
    {
      if constexpr( s_direction == t_direction::n_forward )
      { return p1.m_current <=> p2.m_current; } else
      { return p2.m_current <=> p1.m_current; }
    }

    friend std::strong_ordering
      operator <=> (t_reference<const t_iterator> p, sentinel_type)
    {
      if constexpr( s_direction == t_direction::n_forward )
      { return p.m_current <=> p.m_end; } else
      { return p.m_end <=> p.m_current; }
    }
  };

  using iterator = t_iterator<t_direction::n_forward>;
  using reverse_iterator = t_iterator<t_direction::n_reverse>;

  iterator
    begin()
  { return {m_handle, m_handle + s_count}; }

  sentinel_type
    end()
  { return {}; }
#endif

} // ns just
