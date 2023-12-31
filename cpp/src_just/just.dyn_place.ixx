
export module just.dyn_place;

export import just.type_hadling;
import <utility>;

export namespace just {

  // dynamic space nest

  template <typename Memory_nest, bool Leak = false>
  struct nest_dynamic_space
    final
    : protected Memory_nest
    , nest

  {
    using memory_type = Memory_nest;

    static constexpr inline bool
      s_leak{Leak};

    // single space

    template <c_allocatable Type>
    struct t_single
      final
    {
      using value_type = Type;
      using pointer = pointer_to<value_type>;
      using reference = ref_to<value_type>;

    private:

      // data
      pointer
        m_handle;

      using t_self = t_single;

    public:

      ~t_single() noexcept
        requires( not s_leak )
      { reset(); }

      t_single(ref_to<const t_self>) = delete; // no copy
      ref_to<t_self> operator = (ref_to<const t_self>) = delete; // no copy assign

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
      t_single(right_ref_to<t_self> p_other) noexcept :
        m_handle{ std::exchange(p_other.m_handle, nullptr) }
      {}

      // move assign
      ref_to<t_self>
        operator = (right_ref_to<t_self> p_other) noexcept
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

    // fixed t_size space

    template <c_allocatable Type, t_index N>
      requires( N > 0 )
    struct t_multiple_exact
      final
    {
      using value_type = Type;
      using pointer = pointer_to<value_type>;
      using reference = ref_to<value_type>;
      using size_type = decltype(N);

      static constexpr inline const size_type
        s_count = N;

      static constexpr inline size_type
        t_size()
      { return s_count; }

    private:

      // data
      pointer
        m_handle;

      using t_self = t_multiple_exact;

    public:

      ~t_multiple_exact() noexcept
        requires( not s_leak )
      { reset(); }

      t_multiple_exact(ref_to<const t_self>) = delete; // no copy
      ref_to<t_self> operator = (ref_to<const t_self>) = delete; // no copy assign

      t_multiple_exact()
        : m_handle{memory_type::template allocate_multiple<value_type>(s_count)}
      {}

      template <typename Agent>
      t_multiple_exact(
        Agent && p_agent,
        const t_source p_source = t_source::current()
      )
        : m_handle{memory_type::template allocate_multiple<value_type>(
          s_count, p_agent, p_source
        )}
      {}

      // move
      t_multiple_exact(right_ref_to<t_self> p_other) noexcept :
        m_handle{ std::exchange(p_other.m_handle, nullptr) }
      {}

      // move assign
      ref_to<t_self>
        operator = (right_ref_to<t_self> p_other) noexcept
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

    }; // t_multiple_exact

    // any sized space

    template <c_allocatable Type>
    struct t_multiple
      final
    {
      using value_type = Type;
      using pointer = pointer_to<value_type>;
      using reference = ref_to<value_type>;
      using size_type = t_index;

    private:

      struct t_data
      {
        // data
        pointer
          handle{};
        size_type
          t_size{};
      };

      // data
      t_data
        m_data;

      using t_self = t_multiple;

    public:

      ~t_multiple() noexcept
        requires( not s_leak )
      { reset(); }

      t_multiple(ref_to<const t_self>) = delete; // no copy
      ref_to<t_self> operator = (ref_to<const t_self>) = delete; // no copy assign

      t_multiple() = default;

      t_multiple(size_type p_size)
        : m_data{memory_type::template allocate_multiple<value_type>(p_size)}
      {
        if( m_data.handle != nullptr ) { m_data.t_size = p_size; }
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
      { m_data.t_size = p_size; }

      // move
      t_multiple(right_ref_to<t_self> p_other) noexcept :
        m_data{ std::exchange(p_other.m_data, {}) }
      {}

      // move assign
      ref_to<t_self>
        operator = (right_ref_to<t_self> p_other) noexcept
      {
        memory_type::deallocate(std::exchange(
          m_data, std::exchange(p_other.m_data, {})
        ).handle);
      }

      inline size_type
        t_size() const
      { return m_data.t_size; }

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

} // ns
