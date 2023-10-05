
export module just.std; // checked: 2023-10-03 #2

export import <string>;

import <cstddef>;
import <cstdint>;
import <type_traits>;
import <source_location>;

export namespace just {

  using t_int = std::intptr_t;

  using t_index = std::ptrdiff_t;
  using t_size = decltype( sizeof(bool) ); // std::size_t

  using t_align = std::align_val_t;
  using t_align_of = decltype( alignof(bool) ); // std::size_t

  using t_source = std::source_location;

  //

  template <typename Type>
  concept c_integral = std::is_integral_v<Type>;

  template <typename Type>
  concept c_enum = std::is_enum_v<Type>;

  template <typename Type, typename U>
  concept c_same_type = std::is_same_v<Type, U>;

  template <typename Type, typename ... U>
  concept c_any_of = (std::is_same_v<Type, U> || ...);

  //

  template <typename Type>
  concept c_not_ref = not std::is_reference_v<Type>;

  template <c_not_ref Type>
  using ref_to = Type &;

  template <c_not_ref Type>
  using right_ref_to = Type &&;

  template <typename Type>
  using pointer_to = Type *;

  //

  struct no_instance
  {
    no_instance() = delete;
    no_instance(const no_instance &) = delete;
    no_instance(no_instance &&) = delete;
  };

  struct nest { private: no_instance only_nest; };

} // ns just
