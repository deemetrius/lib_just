
export module just.dyn_place;

export import just.std;
import <utility>;

export namespace just {

  // dynamic space nest

  template <typename Memory, bool Leak = false>
  struct nest_dynamic_space
    final
    : protected Memory
  {
    ~nest_dynamic_space() = delete; // nest

    using memory_type = Memory;

    static constexpr inline bool
      s_leak{Leak};

    // single space

    template <c_allocatable T>
    struct t_single
      final
    {
      using value_type = T;
      using pointer = t_pointer<value_type>;
      using reference = t_reference<value_type>;

    private:

      // data
      pointer
        m_handle;

      using t_self = t_single;

    public:

      ~t_single() noexcept
        requires( not s_leak )
      { reset(); }

      t_single(t_reference<const t_self>) = delete; // no copy
      t_reference<t_self> operator = (t_reference<const t_self>) = delete; // no copy assign

      t_single() noexcept
        : m_handle{memory_type::template allocate_single<value_type>()}
      {}

      template <typename Agent>
      t_single(
        Agent && p_agent,
        const t_source p_source = t_source::current()
      )
        : m_handle{memory_type::template allocate_single<value_type>(p_agent, p_source)}
      {}

      // move
      t_single(t_right_ref<t_self> p_other) noexcept :
        m_handle{ std::exchange(p_other.m_handle, nullptr) }
      {}

      // move assign
      t_reference<t_self>
        operator = (t_right_ref<t_self> p_other) noexcept
      {
        memory_type::deallocate(std::exchange(
          m_handle, std::exchange(p_other.m_handle, nullptr)
        ));
      }

      void
        reset() noexcept
      { memory_type::deallocate( std::exchange(m_handle, nullptr) ); }

      inline pointer
        get() const noexcept
      { return m_handle; }

      inline pointer
        operator -> () const noexcept
      { return m_handle; }

      inline reference
        operator * () const noexcept
      { return *m_handle; }

    }; // t_single

    // fixed size space

    template <c_allocatable T, t_index N>
      requires( N > 0 )
    struct t_fixed
      final
    {
      using value_type = T;
      using pointer = t_pointer<value_type>;
      using reference = t_reference<value_type>;
      using size_type = decltype(N);

      static constexpr inline const size_type
        s_count = N;

      static constexpr inline size_type
        size()
      { return s_count; }

    private:

      // data
      pointer
        m_handle;

      using t_self = t_fixed;

    public:

      ~t_fixed() noexcept
        requires( not s_leak )
      { reset(); }

      t_fixed(t_reference<const t_self>) = delete; // no copy
      t_reference<t_self> operator = (t_reference<const t_self>) = delete; // no copy assign

      t_fixed()
        : m_handle{memory_type::template allocate_multiple<value_type>(s_count)}
      {}

      template <typename Agent>
      t_fixed(
        Agent && p_agent,
        const t_source p_source = t_source::current()
      )
        : m_handle{memory_type::template allocate_multiple<value_type>(
          s_count, p_agent, p_source
        )}
      {}

      // move
      t_fixed(t_right_ref<t_self> p_other) noexcept :
        m_handle{ std::exchange(p_other.m_handle, nullptr) }
      {}

      // move assign
      t_reference<t_self>
        operator = (t_right_ref<t_self> p_other) noexcept
      {
        memory_type::deallocate(std::exchange(
          m_handle, std::exchange(p_other.m_handle, nullptr)
        ));
      }

      void
        reset() noexcept
      { memory_type::deallocate( std::exchange(m_handle, nullptr) ); }

      inline pointer
        get() const noexcept
      { return m_handle; }

      inline reference
        operator [] (size_type p_index) const
      { return m_handle[p_index]; }

    }; // t_fixed

    // any sized space

    template <c_allocatable T>
    struct t_multiple
      final
    {
      using value_type = T;
      using pointer = t_pointer<value_type>;
      using reference = t_reference<value_type>;
      using size_type = t_index;

    private:

      struct t_data
      {
        // data
        pointer
          handle{};
        size_type
          size{};
      };

      // data
      t_data
        m_data;

      using t_self = t_multiple;

    public:

      ~t_multiple() noexcept
        requires( not s_leak )
      { reset(); }

      t_multiple(t_reference<const t_self>) = delete; // no copy
      t_reference<t_self> operator = (t_reference<const t_self>) = delete; // no copy assign

      t_multiple() = default;

      t_multiple(size_type p_size)
        : m_data{memory_type::template allocate_multiple<value_type>(p_size)}
      {
        if( m_data.handle != nullptr ) { m_data.size = p_size; }
      }

      template <typename Agent>
      t_multiple(
        size_type p_size,
        Agent && p_agent,
        const t_source p_source = t_source::current()
      )
        : m_data{memory_type::template allocate_multiple<value_type>(
          p_size, p_agent, p_source
        )}
      { m_data.size = p_size; }

      // move
      t_multiple(t_right_ref<t_self> p_other) noexcept :
        m_data{ std::exchange(p_other.m_data, {}) }
      {}

      // move assign
      t_reference<t_self>
        operator = (t_right_ref<t_self> p_other) noexcept
      {
        memory_type::deallocate(std::exchange(
          m_data, std::exchange(p_other.m_data, {})
        ).handle);
      }

      inline size_type
        size() const
      { return m_data.size; }

      void
        reset() noexcept
      { memory_type::deallocate(std::exchange(m_data, {}).handle); }

      inline pointer
        get() const noexcept
      { return m_data.handle; }

      inline reference
        operator [] (size_type p_index) const
      { return m_data.handle[p_index]; }

    }; // t_multiple

  }; // t_cross_dyn_place

  /*
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
      if constexpr( s_direction == t_direction::forward )
      { ++m_current; } else { --m_current; }
      return *this;
    }

    t_reference<t_iterator>
      operator -- ()
    {
      if constexpr( s_direction == t_direction::forward )
      { --m_current; } else { ++m_current; }
      return *this;
    }

    t_reference<t_iterator>
      operator += (difference_type p_delta)
    {
      if constexpr( s_direction == t_direction::forward )
      { m_current += p_delta; } else { m_current -= p_delta; }
      return *this;
    }

    t_reference<t_iterator>
      operator -= (difference_type p_delta)
    {
      if constexpr( s_direction == t_direction::forward )
      { m_current -= p_delta; } else { m_current += p_delta; }
      return *this;
    }

    friend t_iterator
      operator + (t_reference<const t_iterator> p, difference_type p_delta)
    {
      if constexpr( s_direction == t_direction::forward )
      { return {p.m_current + p_delta, p.m_end}; } else
      { return {p.m_current - p_delta, p.m_end}; }
    }

    friend t_iterator
      operator - (t_reference<const t_iterator> p, difference_type p_delta)
    {
      if constexpr( s_direction == t_direction::forward )
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
      if constexpr( s_direction == t_direction::forward )
      { return p1.m_current <=> p2.m_current; } else
      { return p2.m_current <=> p1.m_current; }
    }

    friend std::strong_ordering
      operator <=> (t_reference<const t_iterator> p, sentinel_type)
    {
      if constexpr( s_direction == t_direction::forward )
      { return p.m_current <=> p.m_end; } else
      { return p.m_end <=> p.m_current; }
    }
  };

  using iterator = t_iterator<t_direction::forward>;
  using reverse_iterator = t_iterator<t_direction::reverse>;

  iterator
    begin()
  { return {m_handle, m_handle + s_count}; }

  sentinel_type
    end()
  { return {}; }
  */

} // ns
