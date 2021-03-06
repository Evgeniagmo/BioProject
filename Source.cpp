#include <cmath>
#include <iostream>
#include <random>
#include <string>
#include <vector>

using t_point = std::pair <double, double>;

double distance(t_point left, t_point right)
{
	return std::hypot((left.first - right.first), (left.second - right.second));
}

t_point make_vector(t_point init, t_point final) //creating vector directed from initial point to final point
{
	return std::make_pair((final.first - init.first), (final.second - init.second));
}

class Node;

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
		m_length = distance(l_node.get_pos(), r_node.get_pos());
	}

	Node get_left_node() const
	{
		return l_node;
	}

	void set_left_node(Node new_left)
	{
		l_node = new_left;
	}

	Node get_right_node() const
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
	t_point m_displacement; // for movement on each iteraction

public:

	Node() {}

	Node(const t_point& pos) : m_pos(pos)
	{
		// � ����� ��������
	}

	Node(double first, double second)
	{
		m_pos = std::make_pair(first, second);
	}

	~Node() {};

	t_point get_pos() const
	{
		return m_pos;
	}

	void set_pos(t_point pos)
	{
		m_pos = pos;
	}

	Link get_left_link() const
	{
		return l_link;
	}

	void set_left_link(Link new_left)
	{
		l_link = new_left;
	}

	Link get_right_link() const
	{
		return r_link;
	}

	void set_right_link(Link new_right)
	{
		r_link = new_right;
	}

	t_point get_displacement() const
	{
		return m_displacement;
	}

	void add_displacement(t_point displacement)
	{
		m_displacement.first += displacement.first;
		m_displacement.second += displacement.second;
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
		for (const Node& node : nodes)
		{
			m_centre.first += node.get_pos().first / nodes.size();
			m_centre.second += node.get_pos().second / nodes.size();
		}
	}

public:
	Cell() {}

	virtual ~Cell() = default;

	std::vector <Node> get_nodes() const
	{
		return nodes;
	}

	t_point get_centre() const
	{
		return this->m_centre;
	}

	double get_area()
	{
		double area = 0.0;

		std::size_t j = nodes.size() - 1;
		for (std::size_t i = 0; i < nodes.size(); i++)
		{
			area += (nodes[j].get_pos().first + nodes[i].get_pos().first) *
				(nodes[j].get_pos().second + nodes[i].get_pos().second);
			j = i;
		}

		return abs(area / 2.0);
	}

	virtual void next_pos() const = 0;

	void f_pressure()
	{
		for_each(get_nodes().begin(), get_nodes().cend(),
			[this](Node node) {
				// coefficient of compression
				double coef = get_area() / m_area;

				// current vector directed from the node
				t_point vect_end = make_vector(node.get_pos(), m_centre);

				// displacement
				vect_end.first -= vect_end.first / coef;
				vect_end.second -= vect_end.second / coef;
				node.add_displacement(vect_end);
			});
	}

	// F_gravitation() gravitation for each link
	// F_repulsion() repulsion for each link
	// f_restoring() restoring force for each node
	// F_pressure() internal pressure for each node


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
	std::vector<Cell*> m_neighbors;
	bool m_flag = false;

public:

	StableCell() {}

	// via nodes
	StableCell(const std::vector<Node>& points)
	{
		nodes = points;

		// creating links and setting left and right
		// m_area
	}

	StableCell(const Node& right_top)
	{
		nodes.push_back(right_top);

		Node left_top = Node((right_top.get_pos().first - 12.7), right_top.get_pos().second);
		nodes.push_back(left_top);
		Node left_bottom = Node(left_top.get_pos().first, (left_top.get_pos().second - 12.7));
		nodes.push_back(left_bottom);
		Node right_bottom = Node(right_top.get_pos().first, left_bottom.get_pos().second);
		nodes.push_back(right_bottom);

		// creating links and setting left and right
		// m_area
	}

	State get_state()
	{
		return this->m_state;
	}

	void set_state(State state)
	{
		m_state = state;
	}

	auto next_state(const WorkingSpace& tissue)
	{
		// cellular automation
	}

	bool get_flag()
	{
		return m_flag;
	}

	void set_flag(bool flag)
	{
		m_flag = flag;
	}

	virtual void next_pos() const override
	{
		// function call for each node
	}

	void get_neighbors(const WorkingSpace& tissue); // searching for stablecells for automation
								// distance(m_centre, cell_centre) < search_radius

};

class ImmuneCell : public Cell
{
protected:

	double m_velocity;			// the absolute value for velocity
	t_point m_direction;	// direction for velocity

public:

	ImmuneCell() {}

	virtual ~ImmuneCell() = default;

	virtual t_point set_direction(const WorkingSpace& tissue) const = 0;

	virtual void next_pos() const override = 0
	{
		// function call for each node
	}
};

class Lymphocyte : public ImmuneCell
{

public:

	Lymphocyte() {}

	// via nodes and velocity
	Lymphocyte(const std::vector<Node>& points, double velocity)
	{
		m_velocity = velocity;
		nodes = points;

		// creating links and setting left and right
		// m_area
	}

	void sensor(const WorkingSpace& tissue);	// searching for ill or recovered cells
												// distance(m_centre, cell_centre) < search_radius

	virtual t_point set_direction(const WorkingSpace& tissue) const override;		// random direction

	void set_velocity()
	{
		// changing velocity in case of ill cells nearby
	}

	virtual void next_pos() const override
	{
		// function call for each node
			// add m_velosity
	}
};

class Fagocyte : public ImmuneCell
{
public:

	Fagocyte() {}

	// via nodes and velocity
	Fagocyte(const std::vector<Node>& points, double velocity)
	{
		m_velocity = velocity;
		nodes = points;

		// creating links and setting left and right
		// m_area
	}

	virtual t_point set_direction(const WorkingSpace& tissue) const override;

	virtual void next_pos() const override
	{
		// function call for each node
			// add m_velosity
	}

	void kill(const WorkingSpace& tissue);		// delete ill or recovered cells

};

class WorkingSpace
{
public:

	std::vector<Cell*> all_stable, all_lymphocytes, all_fagocytes;

	/*std::vector< std::vector< Cell* > > all_types = { epithelium, basale,
					fibroblasts, lymphocytes, fagocytes };*/

	const double search_radius = 0; // 1.5 link length

	double epithelial_cell_size; // <L> = 12,7 microns

	t_point left_bottom, right_top;


	// time control

};

class Calculate
{
public:

	void calc_next_state() {}

	void calc_next_pos() {}
};

void StableCell::get_neighbors(const WorkingSpace& tissue)
{
	std::vector<Cell*> neighbors;
	for_each(tissue.all_stable.cbegin(), tissue.all_stable.cend(),
		[neighbors, tissue, this](Cell* cell) {
			if (distance(m_centre, cell->get_centre()) < tissue.search_radius)
				neighbors.push_back(cell);
		});
	m_neighbors = neighbors;
}

void Lymphocyte::sensor(const WorkingSpace& tissue)
{
	for_each(tissue.all_stable.cbegin(), tissue.all_stable.cend(),
		[tissue, this](StableCell* cell) {
			if ((distance(m_centre, cell->get_centre()) < tissue.search_radius) &&
				((cell->get_state() == StableCell::State::ill) || (cell->get_state() == StableCell::State::recovered)))
				cell->set_flag(true);
		});
}

t_point Lymphocyte::set_direction(const WorkingSpace& tissue) const
{
	t_point new_direction = std::make_pair(0, 0);
	std::default_random_engine dre;
	std::uniform_int_distribution<double> di_first(tissue.left_bottom.first, tissue.right_top.first);
	std::uniform_int_distribution<double> di_second(tissue.left_bottom.second, tissue.right_top.second);
	new_direction.first = di_first(dre);
	new_direction.second = di_second(dre);

	return new_direction;
}

t_point Fagocyte::set_direction(const WorkingSpace& tissue) const
{
	t_point center_of_mass = std::make_pair(0, 0);
	double quantity = tissue.all_fagocytes.size();
	for (Cell* fagocyte : tissue.all_fagocytes)
	{
		center_of_mass.first += fagocyte->get_centre().first / quantity;
		center_of_mass.second += fagocyte->get_centre().second / quantity;
	}
	return center_of_mass;
}

void Fagocyte::kill(const WorkingSpace& tissue)
{
	tissue.all_stable.erase(remove_if(tissue.all_stable.begin(), tissue.all_stable.end(),
		[tissue, this](StableCell* cell) {
			return ((distance(m_centre, cell->get_centre) < tissue.search_radius) &&
				(cell->get_flag() == true));
		}),
		tissue.all_stable.end());
}

auto StableCell::next_state(const WorkingSpace& tissue)
{
	int new_state;
	State next_state;

	std::default_random_engine dre;
	std::uniform_int_distribution<int> di(1, 4);
	new_state = di(dre);
	if (m_state == State::ill)
	{
		switch (new_state)
		{
		case 1:
			return m_state;
		case 3:
			// depends on the virus traits
		case 4:
			// depends on the virus traits and time
		case 2:
			// 
		}
	}

	int counter_ill = 0;
	double concentration = 0;
	get_neighbors(tissue);
	for_each(m_neighbors.cbegin(), m_neighbors.cend(),
		[this, counter_ill](StableCell* neighbor) {
			if (neighbor->get_state() == State::ill)
				counter_ill += 1;
		});
	concentration = counter_ill / m_neighbors.size();

	if (m_state == State::healthy)
	{
		switch (new_state)
		{
		case 2:
			if (concentration > 0, 5)
				// depends on the virus traits, its concentration, number of ill
		case 3:
			return m_state;
		case 4:
			// depends on time
		case 1:
			// 
		}
	}
	if (m_state == State::healthy)
	{
		switch (new_state)
		{
		case 1:
			return m_state;
		case 2:
			// depends on the virus traits, its concentration, number of ill
		case 4:
			// depends on the virus traits and time
		case 3:
			// 
		}
	}


}