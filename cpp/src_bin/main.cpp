
#if 1
import just.enum_info;
import <iostream>;

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
{
  using namespace just::literals_static_text;
  static constexpr bool b{ "123"_st.span() == "123"_st.span()};
  std::cout
    << b << '\n'
    << nest_align_horz::s_info->to_target(t_align_horz::n_left) << '\n'
    << std::to_underlying( nest_align_horz::s_info->to_value("align-left") ) << '\n'
  ;
  return 0;
}
#endif

//

#if 0

import just.static_text;
import <iostream>;

int main()
{
  constexpr static just::t_static_text
    res1{just::static_concat<"1", "22", "333">()};
  constexpr static just::t_static_text
    res2{just::static_concat_separated<":", "1", "22", "333">()};
  std::cout << res1.data << ' ' << res1.length << '\n';
  std::cout << res2.data << ' ' << res2.length << '\n';
  return 0;
}

#endif

//

#if 0

import just.compare;
import just.std;
import <iostream>;
#if 0
template <typename Type>
struct t_nest_of;

template <typename Nest, typename Type>
struct t_nest_of<Type Nest::*>
{
  using type = Nest;
};

struct base
{
  // data
  bool value{};

  friend just::t_compare::param_type operator <=> (const base &, const base &) = default;

  const base & bb() const { return *this; } 
};

struct t_wrap
  : public base
{
  //using base::base;

  friend just::t_compare operator <=> (const t_wrap & p1, const t_wrap & p2)
  { return p1.bb() <=> p2.bb(); }
};


#ifdef _MSC_VER
#define NO_INLINE __declspec(noinline)
#elif defined(__GNUC__)
#define NO_INLINE __attribute__ ((noinline))
#else
#define NO_INLINE
#endif

bool NO_INLINE go1(const t_wrap & p1, const t_wrap & p2)
{ return p1 >= p2; }

bool NO_INLINE go2(const t_wrap & p1, const t_wrap & p2)
{ return p1.value >= p2.value; }
#endif

int main()
{
  //t_wrap v1{true}, v2{false};
  //bool r1 = go1(v1, v2);
  //bool r2 = go2(v1, v2);
  just::t_compare
    v{1.0 <=> 0.0 | just::as_compare},
    w{1.0/0.0 <=> 1.0/0.0 | just::as_compare};
  std::cout << std::to_underlying(v) << '\n';
  std::cout << std::to_underlying(w) << '\n';
  std::cout << (v < w) << '\n';
  std::cout << (0.0/0.0 <=> 1.0/0.0 == just::t_compare::n_unordered) << '\n';
  //just::t_compare_partial w{1.0 <=> 0.0};
  //std::cout << (v == w) << '\n';
  //std::cout << (v == 0) << '\n';
  switch( w )
  {
  case just::t_compare::n_none:
    std::cout << "none\n";
    break;
  case just::t_compare::n_less:
    std::cout << "less\n";
    break;
  case just::t_compare::n_equal:
    std::cout << "equal\n";
    break;
  case just::t_compare::n_greater:
    std::cout << "greater\n";
    break;
  case just::t_compare::n_unordered:
    std::cout << "unordered\n";
    break;
  //default:
    //std::cout << "wrong\n";
  }
  return 0;
}

#endif

//

#if 0

import just.cross_memory;
import just.dyn_place;
import just.t_agent_exception;
import <iostream>;
//import just.flags;
//import <typeinfo>;
//import <typeindex>;
//import <string_view>;
//import <array>;
//import <algorithm>;

//using namespace std::string_literals;

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
    using t_multiple = t_nest::t_multiple_exact<just::t_byte[0x7fffffff], 3>;
    using t_agent = just::t_agent_exception;
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
