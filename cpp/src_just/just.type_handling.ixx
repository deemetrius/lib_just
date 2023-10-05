
export module just.type_hadling;

export import just.std;
import <type_traits>;

export namespace just {

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

  // c_comparable_equal_with
  template <typename Left, typename Right>
  concept c_comparable_equal_with = requires(
    ref_to< const std::remove_reference_t<Left> > p_left,
    ref_to< const std::remove_reference_t<Right> > p_right
    )
  { (p_left == p_right); };

  // c_comparable_equal
  template <typename Type>
  concept c_comparable_equal = c_comparable_equal_with<Type, Type>;

  // t_result_of_compare_equal
  template <typename Left, typename Right = Left>
    requires( c_comparable_equal_with<Left, Right> )
  using t_result_of_compare_equal = decltype(
    std::declval<Left>() == std::declval<Right>()
    );

  // c_comparable_equal_resulted_is
  template <typename Type, typename Result>
  concept c_comparable_equal_resulted_is =
    c_comparable_equal_with<Type, Type> &&
    std::is_same_v<t_result_of_compare_equal<Type>, Result>;

  // c_comparable_equal_resulted_as
  template <typename Type, typename Result>
  concept c_comparable_equal_resulted_as =
    c_comparable_equal_with<Type, Type> &&
    c_convertible_from<Result, t_result_of_compare_equal<Type> >;

} // ns just
