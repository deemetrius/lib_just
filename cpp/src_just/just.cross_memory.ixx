
export module just.cross_memory;

export import just.errors;
export import <new>;
export import <utility>;

export namespace just {

  struct nest_cross_memory
  {
    ~nest_cross_memory() = delete; // nest

  private:

    struct nest_hidden
    {
      ~nest_hidden() = delete; // nest

      enum class t_status : bool { n_ready = false, n_unready = true };

      using t_void_pointer = t_pointer<void>;
      using t_ret = t_result<t_void_pointer, t_status>;

      static t_ret
        allocate_unready(t_size p_size, t_align p_align) noexcept
      {
        return { .status = t_status::n_unready };
      }

      static t_ret
        allocate(t_size p_size, t_align p_align) noexcept
      {
        return { ::operator new (p_size, p_align, std::nothrow) };
      }

      static void
        deallocate(t_void_pointer p_handle, t_align p_align) noexcept
      {
        ::operator delete (p_handle, p_align, std::nothrow);
      }

      using tfn_allocate = decltype(& allocate);
      using tfn_deallocate = decltype(& deallocate);

      struct t_core
      {
        // data
        tfn_allocate
          m_allocate = & allocate;
        tfn_deallocate
          m_deallocate = & deallocate;
      };

      static constexpr inline const t_core
        s_core_unready{& allocate_unready};

      static inline t_pointer<const nest_hidden::t_core>
        s_core = & nest_hidden::s_core_unready;
    }; // nest_hidden
  
    template <typename T, typename Agent>
    static void
      validate(
        t_reference<const nest_hidden::t_ret> p_ret,
        t_index p_count,
        t_reference<Agent> p_agent,
        const t_source p_source
      )
    {
      if( p_ret.value == nullptr )
      {
        if( p_ret.status == nest_hidden::t_status::n_unready )
        { p_agent(std::in_place_type<t_error_unready>, p_source); }
        p_agent(std::in_place_type<t_error_allocate>, std::in_place_type<T>, p_count, p_source);
      }
    }

  protected:

    // single

    template <c_allocatable T>
    static t_pointer<T>
      allocate_single() noexcept
    {
      nest_hidden::t_ret v_result = nest_hidden::s_core->m_allocate(
        sizeof(T), t_align{alignof(T)}
      );
      return static_cast< t_pointer<T> >(v_result.value);
    }

    // single validated

    template <c_allocatable T, typename Agent>
    static t_pointer<T>
      allocate_single(Agent && p_agent, const t_source p_source)
    {
      nest_hidden::t_ret ret = nest_hidden::s_core->m_allocate(
        sizeof(T), t_align{alignof(T)}
      );
      validate<T>(ret, 1, p_agent, p_source);
      return static_cast< t_pointer<T> >(ret.value);
    }

    // multiple

    template <c_allocatable T>
    static t_pointer<T>
      allocate_multiple(t_index p_count) noexcept
    {
      nest_hidden::t_ret v_result = nest_hidden::s_core->m_allocate(
        entire_size<T>(p_count), t_align{alignof(T)}
      );
      return static_cast< t_pointer<T> >(v_result.value);
    }

    // multiple validated

    template <c_allocatable T, typename Agent>
    static t_pointer<T>
      allocate_multiple(t_index p_count, Agent && p_agent, const t_source p_source)
    {
      nest_hidden::t_ret ret = nest_hidden::s_core->m_allocate(
        entire_size<T>(p_count), t_align{alignof(T)}
      );
      validate<T>(ret, p_count, p_agent, p_source);
      return static_cast< t_pointer<T> >(ret.value);
    }

    // close

    template <c_allocatable T>
    static void
      deallocate(t_pointer<T> p_handle) noexcept
    {
      nest_hidden::s_core->m_deallocate(p_handle, t_align{alignof(T)});
    }

  public:

    struct t_global
    {
    private:
      // data
      const nest_hidden::t_core
        m_core;

    public:
      t_global() noexcept { nest_hidden::s_core = & m_core; }
      void set() const noexcept { nest_hidden::s_core = & m_core; }
    }; // t_global

  }; // nest_cross_memory

} // ns

extern "C" {

  __declspec(dllexport)
  void
    just_init_cross_memory(const just::nest_cross_memory::t_global & p_global)
  {
    p_global.set();
  }

} // extern
