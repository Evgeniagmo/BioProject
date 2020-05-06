#pragma once

using t_point = std::pair <double, double>;

class Node
{
private:

	t_point m_pos;
	Link l_link, r_link;
	t_point m_displacement; // for movement on each iteraction

public:

	Node() noexcept = default;

	//Node(const t_point& pos) : m_pos(pos)
	//{
	//	// и ребра задаются
	//}

	explicit Node(double first, double second) noexcept :
		m_pos(std::make_pair(first, second));
	{}

	~Node() noexcept = default;

public:

	const t_point get_pos() const noexcept
	{
		return m_pos;
	}

	void set_pos(const t_point& pos)
	{
		m_pos = pos;
	}

	const Link get_left_link() const noexcept
	{
		return l_link;
	}

	void set_left_link(const Link& new_left)
	{
		l_link = new_left;
	}

	const Link get_right_link() const noexcept
	{
		return r_link;
	}

	void set_right_link(const Link& new_right)
	{
		r_link = new_right;
	}

	const t_point get_displacement() const noexcept
	{
		return m_displacement;
	}

	// displacement updates on each iteraction
	void add_displacement(const t_point& displacement) noexcept;
}