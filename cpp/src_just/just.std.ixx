
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

  enum class t_direction : t_byte { forward, reverse };

  struct t_sentinel {};

  //

  template <typename T>
  struct is_simple : public std::bool_constant<
    not (std::is_reference_v<T> or std::is_unbounded_array_v<T> or std::is_function_v<T>)
  > {};

  template <typename T>
  inline constexpr bool is_simple_v = is_simple<T>::value;

  // concepts

  template <typename T>
  concept c_scalar = std::is_scalar_v<T>;

  template <typename T>
  concept c_not_ref = not std::is_reference_v<T>;

  template <typename T, typename U>
  concept c_same_type = std::is_same_v<T, U>;

  template <typename T, typename ... U>
  concept c_any_of = (std::is_same_v<T, U> || ...);

  template <typename T>
  concept c_allocatable = is_simple_v<T> and (not std::is_abstract_v<T>);

  //

  template <c_allocatable T>
  constexpr inline t_size
    entire_size(t_index p_count)
  {
    return sizeof(T) * static_cast<t_size>(p_count);
  }

  //

  template <typename T>
  using t_pointer = T *;

  template <c_not_ref T>
  using t_reference = T &;

  template <c_not_ref T>
  using t_right_ref = T &&;

  //

  template <typename Value, typename Status>
  struct t_result
  {
    using t_value = Value;
    using t_status = Status;

    // data
    t_value
      value{};
    [[no_unique_address]]
    t_status
      status{};
  };

} // ns
