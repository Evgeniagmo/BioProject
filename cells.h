#pragma once

#include <cmath>
#include <vector>

#include "node.h"
#include "link.h"
#include "functions.h"

class Cell
{

protected:
	std::vector <Node> nodes;
	std::vector <Link> links;

	double m_area;
	t_point m_centre;

private:
	void set_centre() noexcept;

public:
	Cell() noexcept = default;

	virtual ~Cell() noexcept = default;

	const std::vector <Node> get_nodes() const noexcept
	{
		return nodes;
	}

	const t_point get_centre() const noexcept
	{
		return this->m_centre;
	}

	double get_area() noexcept;

	virtual void next_pos() const noexcept = 0;

	void f_gravitation() noexcept;

	void f_repulsion() noexcept;

	void f_restoring() noexcept;

	void f_pressure() noexcept;

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

	StableCell() noexcept = default;

	// via nodes
	//StableCell(const std::vector<Node>& points)
	//{
	//	nodes = points;

	//	// creating links and setting left and right
	//	// m_area
	//}

	StableCell(const Node& right_top)
	{
		// ...
	}

	State get_state()
	{
		return this->m_state;
	}

	void set_state(State state)
	{
		m_state = state;
	}

	bool get_flag()
	{
		return m_flag;
	}

	void set_flag(bool flag)
	{
		m_flag = flag;
	}

	// searching for stablecells for automation
	// distance(m_centre, cell_centre) < search_radius
	void get_neighbors(const WorkingSpace& tissue) noexcept;

	// cellular automation
	auto next_state(const WorkingSpace& tissue)  noexcept;
	
	// function call for each node
	virtual void next_pos() const noexcept override

};

class ImmuneCell : public Cell
{
protected:

	double m_velocity;			// absolute value for velocity
	t_point m_direction;	// direction for velocity

public:

	ImmuneCell() noexcept = default;

	virtual ~ImmuneCell() noexcept = default;

	virtual t_point set_direction(const WorkingSpace& tissue) const = 0;

	virtual void next_pos() const override = 0;
};

class Lymphocyte : public ImmuneCell
{

public:

	Lymphocyte() noexcept = default;

	// via nodes and velocity
	explicit Lymphocyte(const std::vector<Node>& points, double velocity)  noexcept
	{
		m_velocity = velocity;
		nodes = points;

		// creating links and setting left and right
		// m_area
	}

	// random direction
	virtual t_point set_direction(const WorkingSpace& tissue) const override;

	// changing velocity in case of ill cells nearby
	void update_velocity() noexcept;

	// searching for ill or recovered cells
	// distance(m_centre, cell_centre) < search_radius
	void sensor(const WorkingSpace& tissue) noexcept = default;

	// function call for each node
	// add m_velosity
	virtual void next_pos() const noexcept override;
};

class Fagocyte : public ImmuneCell
{
public:

	Fagocyte() noexcept = default;

	// via nodes and velocity
	explicit Fagocyte(const std::vector<Node>& points, double velocity) noexcept
	{
		m_velocity = velocity;
		nodes = points;

		// creating links and setting left and right
		// m_area
	}

	virtual t_point set_direction(const WorkingSpace& tissue) const noexcept override;

	// delete ill or recovered cells
	void kill(const WorkingSpace& tissue) noexcept;

	// function call for each node
	// add m_velosity
	virtual void next_pos() const noexcept override;
};