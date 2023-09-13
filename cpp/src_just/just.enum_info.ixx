
export module just.enum_info;

export import just.static_text;
export import <functional>;
export import <map>;

export namespace just {

  struct nest_conversion_map
  {
    ~nest_conversion_map() = delete; // only nest

    template <
      typename Value,
      typename Target,
      typename Projection = std::identity
    >
    struct tpl
    {
      using value_type = Value;
      using target_type = Target;
      using projection_type = Projection;

      using type_to_target = std::map<value_type, target_type, std::ranges::less>;
      using type_from_target = std::map<target_type, value_type, std::ranges::less>;

      // data
      type_to_target
        m_to;
      type_from_target
        m_from;

      void add_to(value_type p_value, target_type p_target)
      { m_to.try_emplace(p_value, p_target); }

      void add_from(value_type p_value, target_type p_target, projection_type proj = {})
      { m_from.try_emplace(proj(p_target), p_value); }
    };
  };

  template <
    typename Enum,
    typename Text,
    typename Projection = std::identity,
    typename Conversion_nest = nest_conversion_map
  >
  struct nest_enum_info
  {
    ~nest_enum_info() = delete; // only nest

    using value_type = Enum;
    using text_type = Text;

    using projection_type = Projection;
    using storage_type = Conversion_nest::template tpl<value_type, text_type, projection_type>;

    template <t_static_text ... Names>
    struct t_alias
    {
      static void add(storage_type & storage, value_type value, projection_type proj = {})
      {
        ( storage.add_from(value, Names.data, proj), ... );
      }
    };

    template <value_type V, t_static_text Name, typename Alias>
    struct t_element
    {
      static void add(storage_type & storage, projection_type proj)
      {
        storage.add_to(V, Name.data);
        Alias::add(storage, V, proj);
      }
    };

    template <typename ... Items>
    struct t_info
    {
      // data
      storage_type
        storage;

      t_info(projection_type proj = {})
      {
        ( Items::add(this->storage, proj), ... );
      }
    };
  };

} // ns
