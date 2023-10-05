
export module just.enum_info;

export import just.text;
export import <functional>;
export import <map>;

export namespace just {

  template <c_enum Enum, typename Text, typename Aliases>
  struct t_enum_info
  {
    using value_type = Enum;
    using text_type = Text;
    using aliases_type = Aliases;

    // data
    value_type
      value;
    text_type
      name;
    aliases_type
      aliases;
  };

  struct nest_conversion_map : public nest
  {
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

      template <typename Behavior = t_behavior_default>
      target_type to_target(value_type value, Behavior && behavior = {}) const
      {
        typename type_to_target::const_iterator it = m_to.find(value);
        return (it == m_to.end()) ? behavior( m_from.begin() )->first : it->second;
      }

      template <typename Behavior = t_behavior_default>
      value_type to_value(target_type target, Behavior && behavior = {}) const
      {
        typename type_from_target::const_iterator it = m_from.find(target);
        return (it == m_from.end()) ? behavior( m_to.begin() )->first : it->second;
      }
    };
  }; // nest_conversion_map

  template <
    typename Enum,
    typename Text,
    typename Projection = std::identity,
    typename Storage_nest = nest_conversion_map
  >
  struct nest_enum_info : public nest
  {
    using value_type = Enum;
    using text_type = Text;
    using text_view_type = std::basic_string_view<typename text_type::value_type>;

    using projection_type = Projection;
    using storage_type = Storage_nest::template tpl<value_type, text_type, projection_type>;

    struct t_item
    {
      // data
      value_type
        value;
      text_type
        name;
    };

    template <text ... Names>
      requires( sizeof...(Names) > 0 )
    struct t_alias
    {
      static void add(storage_type & storage, value_type value, projection_type proj = {})
      {
        ( storage.add_from(value, Names.data, proj), ... );
      }
    };

    template <value_type Value, text Name, typename Alias>
    struct t_element
    {
      //static inline const t_item
      //  s_item{ Value, {Name.data, Name.t_size} };

      static void add(storage_type & storage, projection_type proj)
      {
        storage.add_to(Value, {Name.data, Name.size});
        Alias::add(storage, Value, proj);
      }
    };

    template <typename ... Items>
      requires( sizeof...(Items) > 0 )
    struct t_info
    {
      // data
      storage_type
        storage;

      t_info(projection_type proj = {})
      {
        ( Items::add(this->storage, proj), ... );
      }

      pointer_to<storage_type> operator -> ()
      { return &storage; }
    };
  }; // nest_enum_info

  struct t_behavior_default
  {
    template <typename Iterator>
    Iterator operator () (Iterator it) const
    { return it; }
  };

} // ns
