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
		// и ребра задаются
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
		// m_centre = "центр масс"
	}

public:
	Cell() {}

	virtual ~Cell() = default;

	t_point get_centre()
	{
		return this->m_centre;
	}

	double get_area() {} // текущая площадь

	virtual void next_pos() const = 0;

	// F_gravitation() притяжение для каждого ребра
	// F_repulsion() отталкивание для каждого ребра
	// F_restoring() упругость для каждого узла
	// F_pressure() внутреннее давление для каждого узла


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

	// по узлам
	StableCell(const std::vector<Node>& points)
	{
		nodes = points;
		
		// создание ребер и выставление левых-правых
		// определение нормальной площади
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
		// клеточный автомат, алгоритм метрополиса 
	}

	virtual void next_pos() const override
	{
		// вызов функций для каждого узла
	}
	 
	std::vector<Cell*> get_neighbors() // поиск соседних неподвижных клеток для автомата
	{
		// проверяются центры клеток, расстояние до которых
			// меньше радиуса поиска
	}

	std::size_t count_ill_neighbors() // для клеточного автомата
	{
		// ...
	}
	std::size_t count_recovered_neighbors() // для клеточного автомата
	{
		// ...
	}

};

class ImmuneCell : public Cell
{
protected:

	double m_speed;			// модуль собственной скорости
	t_point m_direction;	// направление вектора собственной скорости

public:

	ImmuneCell() {}

	virtual ~ImmuneCell() = default;

	virtual void set_direction() const = 0;

	virtual void next_pos() const override = 0
	{
		// вызов функций для каждого узла
	}
};

class Lymphocyte : public ImmuneCell
{

public:
	 
	Lymphocyte() {}

	// по узлам и скорости
	Lymphocyte(const std::vector<Node>& points, double speed)
	{
		m_speed = speed;
		nodes = points;

		// создание ребер и выставление левых-правых
		// определение нормальной площади
	}

	bool sensor()
	{
		// проверка клеток на наличие зараженных
			//если расстояние до центра меньше радиуса поиска И состояние: ill или recovered
	}

	virtual void set_direction() const override
	{
		// случайное направление
	}

	void set_speed()
	{
		// установление скорости при наличии зараженных клеток рядом
	}

	virtual void next_pos() const override
	{
		// вызов функций для каждого узла
			// + добавление ненаправленного собственного вектора
	}
};

class Fagocyte : public ImmuneCell
{
public:

	Fagocyte() {}

	// по узлам и скорости
	Fagocyte(const std::vector<Node>& points, double speed)
	{
		m_speed = speed;
		nodes = points;

		// создание ребер и выставление левых-правых
		// определение нормальной площади
	}
	
	virtual void set_direction() const override
	{
		// нужен доступ к списку лимфоцитов и их центрам
			// берется их центр масс
	}

	virtual void next_pos() const override
	{
		// вызов функций для каждого узла
			// + добавление направленного собственного вектора
	}

	void kill(const Cell& cell)
	{
		// удаление клетки из массива клеток
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