
export module just.t_agent_exception;

export import <utility>;

export namespace just {

#ifdef __cpp_exceptions

  struct t_agent_exception
  {
    template <typename Type, typename ... Args>
    void operator () (std::in_place_type_t<Type>, Args ... p_args) const
    { throw Type{p_args ...}; }
  };

#endif

} // ns
