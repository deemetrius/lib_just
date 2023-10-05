
export module just.ui; // experimental

export import just.variadic;

import <type_traits>;

export namespace just { namespace ex {

  template <typename Type, typename Void = void>
  constexpr bool
    g_has_on_click{};

  template <typename Type>
  constexpr bool
    g_has_on_click< Type, std::void_t<decltype(Type::on_click)> >{true};

  template <typename Type>
  using t_has_on_click = std::bool_constant< g_has_on_click<Type> >;

  template <auto Id>
  struct ui_button
  {
    static void
      on_click()
    {}
  };

  template <typename Element>
  constexpr bool
    g_is_control{};

  template <typename Enum, Enum Id, template <auto> typename Pat>
  constexpr bool
    g_is_control< Pat<Id> >{true};

  template <typename Type, typename Enum>
  concept c_ui_control = g_is_control<Type>;

  template <typename Enum, c_ui_control<Enum> ... Elements>
  struct ui_form
  {
    using tv_with_on_click = variadic<Elements ...>::template t_filter<t_has_on_click>;
  };

  //

  enum class main_window {
    n_button_1
  };

  using t_main = ui_form<main_window, ui_button<main_window::n_button_1> >;

} } // ns just ex
