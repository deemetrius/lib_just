
export module just.span;

export import just.std;

export namespace just {

  // � �� ������� ������������� � t_span ��� ������ �����
  // ��� ��� ������, ��� �� ���� ����� ���� ������ ��� ����������� t_span
  // ��� ������������ ������������� �������� ������

  template <typename Type>
  struct t_span
  {
    using value_type = Type;
    using size_type = t_index;
    using pointer = t_pointer<value_type>;

    // data
    pointer
      data{};
    size_type
      size{};
  };

  template <typename Type, t_index Size>
  struct t_span_exact
  {
    using value_type = Type;
    using size_type = decltype(Size);
    using pointer = t_pointer<value_type>;

    static constexpr inline const size_type
      size{Size};

    // data
    pointer
      data{};
  };

} // ns just
