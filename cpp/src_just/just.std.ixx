
export module just.std;

export import <new>;
export import <string>;

import <cstddef>;
import <cstdint>;
import <type_traits>;
import <source_location>;

export namespace just {

  using t_size = std::size_t;
  using t_index = std::ptrdiff_t;

  using t_int = std::intptr_t;
  using t_byte = std::uint8_t;

  using t_align = std::align_val_t;
  using t_align_of = std::underlying_type_t<t_align>;

  using t_source = std::source_location;
  using t_text = std::string;

  //

  template <typename Type>
  concept c_not_ref = not std::is_reference_v<Type>;

  //

  template <typename Type>
  using t_pointer = Type *;

  template <c_not_ref Type>
  using t_reference = Type &;

  template <c_not_ref Type>
  using t_right_ref = Type &&;

  template <typename Type, typename ... U>
  concept c_any_of = (std::is_same_v<Type, U> || ...);

  template <typename Type, typename U>
  concept c_same_type = std::is_same_v<Type, U>;

  // concepts

  template <typename Type, typename Result>
  concept c_convertible_from = requires
  {
    static_cast<Result>( std::declval<Type>() );
  };

  template <typename Type>
  concept c_simple = not (
    std::is_reference_v<Type> or
    std::is_unbounded_array_v<Type> or
    std::is_function_v<Type>
    );

  template <typename Type>
  concept c_scalar = std::is_scalar_v<Type>;

  template <typename Type>
  concept c_allocatable = c_simple<Type> and (not std::is_abstract_v<Type>);

  //

  struct no_instance
  {
    no_instance() = delete;
    no_instance(const no_instance &) = delete;
    no_instance(no_instance &&) = delete;
  };

  struct nest { private: no_instance only_nest; };

} // ns just
