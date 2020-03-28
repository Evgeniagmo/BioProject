#include <cmath>
#include <iostream>
#include <string>
#include <vector>

using t_point = std::pair <double, double>;

//double distance(Node left, Node right)
//{
//	return sqrt(pow(left.get_pos().first - right.get_pos().first)
//		+ pow(left.get_pos().second - right.get_pos().second));
//}

class Link
{
private:

	double m_length;

	Node l_node, r_node;

public:
	Link() {}

	Link(const Node& left, const Node& right) : l_node(left), r_node(right)
	{
		this->set_length();
	}

	~Link() {};

	double get_length() const
	{
		return m_length;
	}

	void set_length()
	{
		m_length = distance(l_node, r_node);
	}

	Node get_left_node()
	{
		return l_node;
	}

	void set_left_node(Node new_left)
	{
		l_node = new_left;
	}

	Node get_right_node()
	{
		return r_node;
	}

	void set_right_node(Node new_right)
	{
		r_node = new_right;
	}

};

class Node
{
private:

	t_point m_pos;
	Link l_link, r_link;

public:

	Node() {}

	Node(const t_point& pos) : m_pos(pos)
	{
		// � ����� ��������
	}

	~Node() {};

	t_point get_pos()
	{
		return m_pos;
	}

	void set_pos(t_point pos)
	{
		m_pos = pos;
	}

	Link get_left_link()
	{
		return l_link;
	}

	void set_left_link(Link new_left)
	{
		l_link = new_left;
	}

	Link get_right_link()
	{
		return r_link;
	}

	void set_right_link(Link new_right)
	{
		r_link = new_right;
	}
};

class Cell
{

protected:
	std::vector <Node> nodes;
	std::vector <Link> links;

	double m_area;
	t_point m_centre;

private:
	void set_centre()
	{
		// m_centre = "����� ����"
	}

public:
	Cell() {}

	virtual ~Cell() = default;

	t_point get_centre()
	{
		return this->m_centre;
	}

	double get_area() {} // ������� �������

	virtual void next_pos() const = 0;

	// F_gravitation() ���������� ��� ������� �����
	// F_repulsion() ������������ ��� ������� �����
	// F_restoring() ��������� ��� ������� ����
	// F_pressure() ���������� �������� ��� ������� ����


};

class StableCell : public Cell
{
public:
	enum class State
	{
		healthy,
		dead,
		ill,
		recovered
	};
private:
	State m_state = State::healthy;
	std::vector<Cell*> neighbors;

public:
	
	StableCell() {}

	// �� �����
	StableCell(const std::vector<Node>& points)
	{
		nodes = points;
		
		// �������� ����� � ����������� �����-������
		// ����������� ���������� �������
	}

	State get_state()
	{
		return this->m_state;
	}

	void set_state(State state)
	{
		m_state = state;
	}

	void next_state()
	{
		// ��������� �������, �������� ����������� 
	}

	virtual void next_pos() const override
	{
		// ����� ������� ��� ������� ����
	}
	 
	std::vector<Cell*> get_neighbors() // ����� �������� ����������� ������ ��� ��������
	{
		// ����������� ������ ������, ���������� �� �������
			// ������ ������� ������
	}

	std::size_t count_ill_neighbors() // ��� ���������� ��������
	{
		// ...
	}
	std::size_t count_recovered_neighbors() // ��� ���������� ��������
	{
		// ...
	}

};

class ImmuneCell : public Cell
{
protected:

	double m_speed;			// ������ ����������� ��������
	t_point m_direction;	// ����������� ������� ����������� ��������

public:

	ImmuneCell() {}

	virtual ~ImmuneCell() = default;

	virtual void set_direction() const = 0;

	virtual void next_pos() const override = 0
	{
		// ����� ������� ��� ������� ����
	}
};

class Lymphocyte : public ImmuneCell
{

public:
	 
	Lymphocyte() {}

	// �� ����� � ��������
	Lymphocyte(const std::vector<Node>& points, double speed)
	{
		m_speed = speed;
		nodes = points;

		// �������� ����� � ����������� �����-������
		// ����������� ���������� �������
	}

	bool sensor()
	{
		// �������� ������ �� ������� ����������
			//���� ���������� �� ������ ������ ������� ������ � ���������: ill ��� recovered
	}

	virtual void set_direction() const override
	{
		// ��������� �����������
	}

	void set_speed()
	{
		// ������������ �������� ��� ������� ���������� ������ �����
	}

	virtual void next_pos() const override
	{
		// ����� ������� ��� ������� ����
			// + ���������� ��������������� ������������ �������
	}
};

class Fagocyte : public ImmuneCell
{
public:

	Fagocyte() {}

	// �� ����� � ��������
	Fagocyte(const std::vector<Node>& points, double speed)
	{
		m_speed = speed;
		nodes = points;

		// �������� ����� � ����������� �����-������
		// ����������� ���������� �������
	}
	
	virtual void set_direction() const override
	{
		// ����� ������ � ������ ���������� � �� �������
			// ������� �� ����� ����
	}

	virtual void next_pos() const override
	{
		// ����� ������� ��� ������� ����
			// + ���������� ������������� ������������ �������
	}

	void kill(const Cell& cell)
	{
		// �������� ������ �� ������� ������
	}

};

class WorkingSpace
{
public:

	std::vector<Cell*> epithelium, basale, fibroblasts,
		lymphocytes, fagocytes;
	
	std::vector< std::vector< Cell* > > all_types = { epithelium, basale,
					fibroblasts, lymphocytes, fagocytes };
	
	
	// time control

};

class Calculate
{
public:

	void calc_next_state() {}

	void calc_next_pos() {}
};