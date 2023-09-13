
#if 1
import just.enum_info;

//using namespace just::literals_static_text;

enum class t_align_horz { n_left, n_center, n_right };

struct nest_align_horz
{
  using enum_type = t_align_horz;
  using nest = just::nest_enum_info<enum_type, just::t_text>;
  using info_type = nest::t_info<
    nest::t_element<enum_type::n_left  , "Left"  , nest::t_alias<"left"  , "align-left"  > >,
    nest::t_element<enum_type::n_center, "Center", nest::t_alias<"center", "align-center"> >,
    nest::t_element<enum_type::n_right , "Right" , nest::t_alias<"right" , "align-right" > >
  >;

  static inline info_type
    s_info{};
};

int main()
{ return 0; }
#endif

//

#if 0

import just.cross_memory;
import just.dyn_place;
import just.agent_exception;
import <iostream>;
//import just.flags;
//import <typeinfo>;
//import <typeindex>;
//import <string_view>;
//import <array>;
//import <algorithm>;

std::ostream & operator << (std::ostream & o, const just::t_source & s)
{
  o
    << '[' << s.line()
    << ':' << s.column()
    << "] " << s.file_name()
    << " ~ " << s.function_name();
  return o;
}

const just::nest_cross_memory::t_global
g_just_cross_object{};

int main() {
  try {
    using t_nest = just::nest_dynamic_space<just::nest_cross_memory>;
    using t_multiple = t_nest::t_fixed<just::t_byte[0x7fffffff], 3>;
    using t_agent = just::agent_exception;
    t_agent a{};
    t_multiple v{a};
    std::cout << static_cast<just::t_int>(v[0][0]) << '\n';
    std::cout << typeid(t_multiple).name() << '\n';
  } catch( const just::t_error_allocate & e ) {
    std::cout
      << "catch " << e.source
      << '\n' << e.target_type->name
      << '\n' << e.count
      << '\n';
  } catch( ... ) {
    std::cout << "catch ...\n";
  }
  using namespace just::literals_static_text;
  just::t_static_text vt{"hello"_st};
  std::cout << typeid(decltype(vt)::size_type).name() << '\n';
#if 0
  bool b;
  t_tpl< just::g_flags<t_omg::none>(t_omg::last) > vt;
  t_tpl< just::g_make_flags(t_omg::last) > vt2;
  try {
    just::t_mem_place::t_single<t_test> v;
    std::cin >> b;
  } catch ( ... ) {
    std::cout << "catch\n";
  }
  std::cout << "done\n" << sizeof(just::t_index);
  std::cin >> b;
  //using namespace std::literals::string_view_literals;
#endif
  return 0;
}

#endif
