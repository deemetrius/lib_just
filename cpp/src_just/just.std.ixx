
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

  // conversion of types

  template <typename Result>
  struct t_cast;

  // operator |
  template <typename From, typename Result>
  constexpr Result
    operator | (From p_from, const t_cast<Result> p_cast)
  { return p_cast(p_from); }

  // global template object
  template <typename Result>
  constexpr inline t_cast<Result>
    as_type{};

  // iteration

  enum class t_direction : t_byte { n_forward, n_reverse };

  struct t_sentinel {};

  // type traits

  template <typename Type>
  struct is_simple : public std::bool_constant<
    not (std::is_reference_v<Type> or std::is_unbounded_array_v<Type> or std::is_function_v<Type>)
  > {};

  template <typename Type>
  inline constexpr bool is_simple_v = is_simple<Type>::value;

  // concepts

  template <typename Type>
  concept c_scalar = std::is_scalar_v<Type>;

  template <typename Type>
  concept c_not_ref = not std::is_reference_v<Type>;

  template <typename Type, typename U>
  concept c_same_type = std::is_same_v<Type, U>;

  template <typename Type, typename ... U>
  concept c_any_of = (std::is_same_v<Type, U> || ...);

  template <typename Type>
  concept c_allocatable = is_simple_v<Type> and (not std::is_abstract_v<Type>);

  //

  template <typename Type>
  using t_pointer = Type *;

  template <c_not_ref Type>
  using t_reference = Type &;

  template <c_not_ref Type>
  using t_right_ref = Type &&;

  //

  template <typename Type>
  concept c_bool_comparable_equal = requires(
    t_reference< const std::remove_reference_t<Type> > p1,
    t_reference< const std::remove_reference_t<Type> > p2
  ) {
    static_cast<bool>(p1 == p2);
  };

  template <typename Type>
  concept c_bool_comparable_not_equal = requires(
    t_reference< const std::remove_reference_t<Type> > p1,
    t_reference< const std::remove_reference_t<Type> > p2
  ) {
    static_cast<bool>(p1 != p2);
  };

  template <typename Type>
  concept c_bool_comparable_equal_both =
    c_bool_comparable_equal<Type> &&
    c_bool_comparable_not_equal<Type>;

  //

  template <c_allocatable Type>
  constexpr inline t_size
    entire_size(t_index p_count)
  {
    return sizeof(Type) * static_cast<t_size>(p_count);
  }

  //

  template <typename Value, typename Status>
  struct t_result
  {
    using value_type = Value;
    using status_type = Status;

    // data
    value_type
      value{};
    [[no_unique_address]]
    status_type
      status{};
  };

  namespace aux {

    // min
    template <typename First, typename Second, typename ... Rest>
    constexpr First
      min(First p_first, Second p_second, Rest ... p_rest)
    {
      if constexpr( sizeof...(Rest) == 0 )
      {
        return (p_first < p_second ? p_first : p_second);
      } else {
        return (p_first < p_second ? p_first : min(p_second, p_rest ...) );
      }
    }
  
  } // ns aux

} // ns
