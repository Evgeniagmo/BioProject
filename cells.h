#pragma once

#include <cmath>
#include <chrono>
#include <memory>
#include <vector>

#include "node.h"
#include "link.h"
#include "functions.h"

class StableCell;
class Cell
{
public:
	using point_t = std::pair <double, double>;
	using node_t = std::shared_ptr < Node >;
	using stable_cell_t = std::shared_ptr < StableCell >;

protected:
	std::vector <node_t> m_nodes;
	std::vector <Link> m_links;

	double m_area = 0.0;

public:
	Cell() noexcept = default;

	virtual ~Cell() noexcept = default;

private:

	double count_area() const noexcept;

public:

	point_t count_centre() const noexcept;

	const std::vector <node_t> get_nodes() const noexcept
	{
		return m_nodes;
	}

	// f_repulsion() repulsion and gravitation for each link - equivalent for molecular connections between cells
	// f_restoring() restoring force for each link 
	// F_pressure() internal pressure for each node

	void f_repulsion(const std::vector< stable_cell_t >& neighbors, double search_radius) noexcept;

	void f_restoring() noexcept;

	void f_pressure() noexcept;

	void move() noexcept;

	// check intersections between two cells and between cell and borders
	void check_intersection(const std::vector< node_t >& nodes) noexcept;

	void check_borders(const std::vector <point_t>& borders) noexcept;

};

class StableCell : public Cell
{
public:
	using stable_cell_t = std::shared_ptr < StableCell >;


	enum class State
	{
		healthy,
		dead,
		ill,
		recovered
	};
private:
	double m_cell_size; // normal length of link
	State m_state;
	State new_state;
	//std::vector<Cell*> m_neighbors;
	//bool m_flag = false;

public:

	StableCell() noexcept = default;

	explicit StableCell(double cell_size, const point_t left_top) noexcept :
		m_cell_size(cell_size), m_state(State::healthy)
	{
		initialize(left_top);
	}

private:

	void initialize(const point_t left_top);

public:

	State get_state() const noexcept
	{
		return this->m_state;
	}

	void set_new_state(const State& state)
	{
		new_state = state;
	}

	void change_state()
	{
		m_state = new_state;
	}

	// cellular automation
	State next_state(
		const std::vector< stable_cell_t > neighbors, bool virus_traits) const noexcept;

public:

	// making vector for client
	// {x1, y1, ... y4, state_num}
	// healthy - 0
	// ill - 1
	// recovered - 2
	std::vector <double> pos_state_to_numbers() const noexcept;

};

//class ImmuneCell : public Cell
//{
//protected:
//
//	double m_velocity;			// absolute value for velocity
//	point_t m_direction;	// direction for velocity
//
//public:
//
//	ImmuneCell() noexcept = default;
//
//	virtual ~ImmuneCell() noexcept = default;
//
//	virtual point_t set_direction(const WorkingSpace& tissue) const = 0;
//
//	virtual void next_pos() const override = 0;
//};
//
//class Lymphocyte : public ImmuneCell
//{
//
//public:
//
//	Lymphocyte() noexcept = default;
//
//	// via nodes and velocity
//	explicit Lymphocyte(const std::vector<Node>& points, double velocity)  noexcept
//	{
//		m_velocity = velocity;
//		nodes = points;
//
//		// creating links and setting left and right
//		// m_area
//	}
//
//	// random direction
//	virtual point_t set_direction(const WorkingSpace& tissue) const override;
//
//	// changing velocity in case of ill cells nearby
//	void update_velocity() noexcept;
//
//	// searching for ill or recovered cells
//	// distance(m_centre, cell_centre) < search_radius
//	void sensor(const WorkingSpace& tissue) noexcept;
//
//	// function call for each node
//	// add m_velosity
//	virtual void next_pos() const noexcept override;
//};
//
//class Fagocyte : public ImmuneCell
//{
//public:
//
//	Fagocyte() noexcept = default;
//
//	// via nodes and velocity
//	explicit Fagocyte(const std::vector<Node>& points, double velocity) noexcept
//	{
//		m_velocity = velocity;
//		nodes = points;
//
//		// creating links and setting left and right
//		// m_area
//	}
//
//	virtual point_t set_direction(const WorkingSpace& tissue) const noexcept override;
//
//	// delete ill or recovered cells
//	void kill(const WorkingSpace& tissue) noexcept;
//
//	// function call for each node
//	// add m_velosity
//	virtual void next_pos() const noexcept override;
//};