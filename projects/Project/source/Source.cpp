#include <cmath>
#include <iostream>
#include <random>
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
		// set edge values
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
		// m_centre = 'center of mass'
	}

public:
	Cell() {}

	virtual ~Cell() = default;

	t_point get_centre()
	{
		return this->m_centre;
	}

	double get_area() {} // current area

	virtual void next_pos() const = 0;

	// F_gravitation() attraction for each link
	// F_repulsion() repulsion for each link
	// F_restoring() elasticity for each node
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
	std::vector<Cell*> neighbors;

public:

	StableCell() {}

	// via links
	StableCell(const std::vector<Node>& points)
	{
		nodes = points;

		// creating links and setting left and right
		// normal area determination
	}

	State get_state()
	{
		return this->m_state;
	}

	void set_state(State state)
	{
		m_state = state;
	}

	auto next_state()
	{
		// cellular automaton
	}

	virtual void next_pos() const override
	{
		// function call for each node
	}

	std::vector<Cell*> get_neighbors() // search for stable neighbors for an automaton
	{
		// the centers of cells are checked, the distance to which is less than the radius of search
	}

	std::size_t count_ill_neighbors() // for cellular automaton
	{
		// ...
	}
	std::size_t count_recovered_neighbors() // for cellular automaton
	{
		// ...
	}

};

class ImmuneCell : public Cell
{
protected:

	double m_velocity;			// own velocity module
	t_point m_direction;	// direction of the own velocity

public:

	ImmuneCell() {}

	virtual ~ImmuneCell() = default;

	virtual void set_direction() const = 0;

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
		// normal area determination
	}

	bool sensor()
	{
		// checking cells for infected
			//if the distance to the center is less than the search radius & state: ill or recovered
	}

	virtual void set_direction() const override
	{
		// random direction
	}

	void set_velocity()
	{
		// establishment of speed in the presence of infected cells nearby
	}

	virtual void next_pos() const override
	{
		// function call for each node
			// + adding random vector of velocity
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
		// normal area determination
	}

	virtual void set_direction() const override
	{
		// access to the list of lymphocytes and their centers is needed
			// their center of mass is taken
	}

	virtual void next_pos() const override
	{
		// function call for each node
			// + adding vector of velocity
	}

	void kill(const Cell& cell)
	{
		// cell removal from the cell array
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

struct Virus
{
	double m_concentr
};

class Calculate
{
public:

	void calc_next_state() {}

	void calc_next_pos() {}
};

auto StableCell::next_state()
{
	int new_state;
	State next_state;

	std::default_random_engine dre;
	std::uniform_int_distribution<int> di(1, 4);
	new_state = di(dre);
	if (m_state = State::ill)
	{
		switch (new_State)
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
	get_neighbors();
	for_each(m_neighbors.cbegin(), coll.cend(),
		[](StableCell* neighbor) {
			if (neighbor->get_state() = State::ill)
				counter_ill += 1;
		});
	concentration = counter_ill / m_neighbors.size();

	if (m_state = State::healthy)
	{
		switch (new_State)
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
	if (m_state = State::healthy)
	{
		switch (new_State)
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