
export module just.errors;

export import just.std;
export import <concepts>;
export import <utility>;
export import <typeinfo>;

export namespace just {

  //using namespace std::string_literals;

  //

  struct t_error
  {
    // data
    t_source
      source{t_source::current()};
  };

  // out of range

  struct t_info_range
  {
    enum t_status { n_unset, n_low, n_high };

    // data
    t_status
      status{n_unset};
  };

  struct t_error_range
    : public t_info_range
    , public t_error
  {};

  // unready

  struct t_error_unready
    : public t_error
  {};

  // no memory

  struct t_type_info
  {
    using const_pointer = t_pointer<const t_type_info>;

    // data
    t_align_of
      align{};
    t_size
      size{};
    t_text
      name{};
  };

  template <typename T>
  const t_type_info
    g_type_info{alignof(T), sizeof(T), typeid(T).name()};

  struct t_error_allocate
    : public t_error
    , public std::bad_alloc
  {
    // data
    t_type_info::const_pointer
      target_type;
    t_index
      count{};

    template <typename T>
    t_error_allocate(
      std::in_place_type_t<T> p_type,
      t_index p_count = 1,
      t_source p_source = t_source::current()
    )
      : target_type{& g_type_info<T>}
      , count{p_count}
      , t_error{p_source}
    {}
  };

} // ns
