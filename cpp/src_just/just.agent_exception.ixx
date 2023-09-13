
export module just.agent_exception;

export import <utility>;

export namespace just {

#ifdef __cpp_exceptions

  struct agent_exception
  {
    template <typename T, typename ... Args>
    void operator () (std::in_place_type_t<T>, Args ... p_args) const
    { throw T{p_args ...}; }
  };

#endif

} // ns
