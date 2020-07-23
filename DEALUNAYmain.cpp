#include <vector>
#include <algorithm>
#include <cmath>
#include <limits>
#include <iostream>
#include <iterator>
#include <array>
#include <random>
#include <chrono>
struct Vector2
{
	Vector2() = default;  // constructor
	Vector2(const Vector2& v) = default; // copy constructor
	Vector2(Vector2&&) = default;  //DOUBT
	Vector2(const double vx, const double vy);  // paramatrized constructor

	double dist2(const Vector2& v) const;
	double dist(const Vector2& v) const;
	double norm2() const;

	Vector2& operator=(const Vector2&) = default; //DOUBT
	Vector2& operator=(Vector2&&) = default;
	bool operator ==(const Vector2& v) const;
	friend std::ostream& operator <<(std::ostream& str, const Vector2& v);
	//  private:
	double x;
	double y;
};
bool almost_equal(const Vector2& v1, const Vector2& v2, int ulp = 2);
struct Edge
{
	using VertexType = Vector2;

	Edge() = default;
	Edge(const Edge&) = default;
	Edge(Edge&&) = default;
	Edge(const VertexType& v1, const VertexType& v2);

	Edge& operator=(const Edge&) = default;
	Edge& operator=(Edge&&) = default;
	bool operator ==(const Edge& e) const;
	friend std::ostream& operator <<(std::ostream& str, const Edge& e);

	const VertexType* v;
	const VertexType* w;
	bool isBad = false;
};
bool almost_equal(const Edge& e1, const Edge& e2);
struct Triangle
{
	// public:
	using EdgeType = Edge;
	using VertexType = Vector2;

	Triangle() = default;
	Triangle(const Triangle&) = default;
	Triangle(Triangle&&) = default; // DOUBT
	Triangle(const VertexType& v1, const VertexType& v2, const VertexType& v3);

	bool containsVertex(const VertexType& v) const;
	bool circumCircleContains(const VertexType& v) const;

	Triangle& operator=(const Triangle&) = default;
	Triangle& operator=(Triangle&&) = default;
	bool operator ==(const Triangle& t) const;
	friend std::ostream& operator <<(std::ostream& str, const Triangle& t);
	// private:
	const VertexType* a;
	const VertexType* b;
	const VertexType* c;
	bool isBad = false;
};
bool almost_equal(const Triangle& t1, const Triangle& t2);
class Delaunay
{
public:
	using TriangleType = Triangle;  // triangle type is actually a datatype which corresponds to the 
									// private members of the triangle class
	using EdgeType = Edge;
	using VertexType = Vector2;     // similar goes for edge type and vertex type 
	Delaunay() = default;  // default constrcutor
	Delaunay(const Delaunay&) = delete;   // destructor the constructor
	Delaunay(Delaunay&&) = delete;  // detecting the entire class (DOUBTFULL)
	const std::vector<TriangleType>& triangulate(std::vector<VertexType>& vertices);
	const std::vector<TriangleType>& getTriangles() const; // get triangles function will return a vector 
														   //that is why we are passing a reference  

	const std::vector<EdgeType>& getEdges() const;     /* If you return the vector by using return statement in
														function. Whenever you return the obejct, the object copying or copy constructor
														will be called , you can yourself visualize the performance
														hit.
														Though based on project or programming need you can return
														vector using return statement, however always try to send
														vector object as reference if you wish to add or delete
														elements inside the vector container or const reference
														in case you want to read data not to modify it.*/
	const std::vector<VertexType>& getVertices() const;  // const is used since we are not modifying any values in that function 
	Delaunay& operator=(const Delaunay&) = delete;
	Delaunay& operator=(Delaunay&&) = delete;  // DOUBT

private:
	std::vector<TriangleType> _triangles;
	std::vector<EdgeType> _edges;
	std::vector<VertexType> _vertices;
};

class numeric
{
	bool almost_equal(const double x, const double y, int ulp = 2);

	double half(const double x);
};
bool almost_equal(const double x, const double y, int ulp)
{
	// the machine epsilon has to be scaled to the magnitude of the values used
	// and multiplied by the desired precision in ULPs (units in the last place)
	return std::abs(x - y) <= std::numeric_limits<double>::epsilon() * std::abs(x + y) * static_cast<double>(ulp)
		// unless the result is subnormal
		|| std::abs(x - y) < std::numeric_limits<double>::min();
}
double half(const double x)
{
	return 0.5 * x;
}

const std::vector<Delaunay::TriangleType>& // this is the return type of the triangulate function in the dealunay class
Delaunay::triangulate(std::vector<VertexType>& vertices)
{
	// Store the vertices locally
	_vertices = vertices;

	// Determinate the super triangle
	double minX = vertices[0].x;
	double minY = vertices[0].y;
	double maxX = minX;
	double maxY = minY;

	for (std::size_t i = 0; i < vertices.size(); ++i)
	{
		if (vertices[i].x < minX) minX = vertices[i].x;
		if (vertices[i].y < minY) minY = vertices[i].y;
		if (vertices[i].x > maxX) maxX = vertices[i].x;
		if (vertices[i].y > maxY) maxY = vertices[i].y;
	}

	const double dx = maxX - minX;
	const double dy = maxY - minY;
	const double deltaMax = std::max(dx, dy);
	const double midx = half(minX + maxX);
	const double midy = half(minY + maxY);

	const VertexType p1(midx - 20 * deltaMax, midy - deltaMax); // might be an algorithm
	const VertexType p2(midx, midy + 20 * deltaMax);
	const VertexType p3(midx + 20 * deltaMax, midy - deltaMax);

	std::cout << p1 << "\n" << p2 << "\n" << p3 << "\n";

	// Create a list of triangles, and add the supertriangle in it
	_triangles.push_back(TriangleType(p1, p2, p3));

	for (auto p = begin(vertices); p != end(vertices); p++)
	{
		std::vector<EdgeType> polygon;
		for (auto& t : _triangles)
		{
			if (t.circumCircleContains(*p))
			{
				t.isBad = true;
				polygon.push_back(Edge{ *t.a, *t.b });
				polygon.push_back(Edge{ *t.b, *t.c });
				polygon.push_back(Edge{ *t.c, *t.a });
			}
		}

		_triangles.erase(std::remove_if(begin(_triangles), end(_triangles), [](TriangleType& t) {
			return t.isBad;
			}), end(_triangles));

		for (auto e1 = begin(polygon); e1 != end(polygon); ++e1)
		{
			for (auto e2 = e1 + 1; e2 != end(polygon); ++e2)
			{
				if (almost_equal(*e1, *e2))
				{
					e1->isBad = true;
					e2->isBad = true;
				}
			}
		}

		polygon.erase(std::remove_if(begin(polygon), end(polygon), [](EdgeType& e) {
			return e.isBad;
			}), end(polygon));

		for (const auto e : polygon)
			_triangles.push_back(TriangleType(*e.v, *e.w, *p));

	}

	_triangles.erase(std::remove_if(begin(_triangles), end(_triangles), [p1, p2, p3](TriangleType& t) {
		return t.containsVertex(p1) || t.containsVertex(p2) || t.containsVertex(p3);
		}), end(_triangles));

	for (const auto t : _triangles)
	{
		_edges.push_back(Edge{ *t.a, *t.b });
		_edges.push_back(Edge{ *t.b, *t.c });
		_edges.push_back(Edge{ *t.c, *t.a });
	}

	//for (auto x : _triangles) std::cout << x << " ";
	//for (auto x : _vertices) std::cout << x << "\n";
	return _triangles;
}

const std::vector<Delaunay::TriangleType>&
Delaunay::getTriangles() const
{
	return _triangles;
}

const std::vector<Delaunay::EdgeType>&
Delaunay::getEdges() const
{
	return _edges;
}

const std::vector<Delaunay::VertexType>&
Delaunay::getVertices() const
{
	return _vertices;
}


// defining the functions of the triangle class


Triangle::Triangle(const VertexType& v1, const VertexType& v2, const VertexType& v3) :
	a(&v1), b(&v2), c(&v3), isBad(false)
{}

bool
Triangle::containsVertex(const VertexType& v) const
{
	// return p1 == v || p2 == v || p3 == v;
	return almost_equal(*a, v) || almost_equal(*b, v) || almost_equal(*c, v);
}

bool
Triangle::circumCircleContains(const VertexType& v) const
{
	const double ab = a->norm2();
	const double cd = b->norm2();
	const double ef = c->norm2();

	const double ax = a->x;
	const double ay = a->y;
	const double bx = b->x;
	const double by = b->y;
	const double cx = c->x;
	const double cy = c->y;

	const double circum_x = (ab * (cy - by) + cd * (ay - cy) + ef * (by - ay)) / (ax * (cy - by) + bx * (ay - cy) + cx * (by - ay));
	const double circum_y = (ab * (cx - bx) + cd * (ax - cx) + ef * (bx - ax)) / (ay * (cx - bx) + by * (ax - cx) + cy * (bx - ax));

	const VertexType circum(half(circum_x), half(circum_y));
	const double circum_radius = a->dist2(circum);
	const double dist = v.dist2(circum);
	return dist <= circum_radius;
}

bool
Triangle::operator ==(const Triangle& t) const
{
	return	(*this->a == *t.a || *this->a == *t.b || *this->a == *t.c) &&
		(*this->b == *t.a || *this->b == *t.b || *this->b == *t.c) &&
		(*this->c == *t.a || *this->c == *t.b || *this->c == *t.c);
}

std::ostream&
operator <<(std::ostream& str, const Triangle& t)
{
	return str << "Triangle:" << "\n\t" <<
		*t.a << "\n\t" <<
		*t.b << "\n\t" <<
		*t.c << '\n';
}

bool almost_equal(const Triangle& t1, const Triangle& t2)
{
	return	(almost_equal(*t1.a, *t2.a) || almost_equal(*t1.a, *t2.b) || almost_equal(*t1.a, *t2.c)) &&
		(almost_equal(*t1.b, *t2.a) || almost_equal(*t1.b, *t2.b) || almost_equal(*t1.b, *t2.c)) &&
		(almost_equal(*t1.c, *t2.a) || almost_equal(*t1.c, *t2.b) || almost_equal(*t1.c, *t2.c));
}


// DEFINING THE FUNCTIONS OF VECTOR2 CLASS


Vector2::Vector2(const double vx, const double vy) :
	x(vx), y(vy)
{}

double
Vector2::dist2(const Vector2& v) const
{
	const double dx = x - v.x;
	const double dy = y - v.y;
	return dx * dx + dy * dy;
}

double
Vector2::dist(const Vector2& v) const
{
	return hypot(x - v.x, y - v.y);
}

double
Vector2::norm2() const
{
	return x * x + y * y;
}

bool
Vector2::operator ==(const Vector2& v) const
{
	return (this->x == v.x) && (this->y == v.y);
}

std::ostream&
operator <<(std::ostream& str, const Vector2& v)
{
	return str << "Point x: " << v.x << " y: " << v.y;
}

bool almost_equal(const Vector2& v1, const Vector2& v2, int ulp)
{
	return almost_equal(v1.x, v2.x, ulp) && almost_equal(v1.y, v2.y, ulp);
}

// DEFINING THE FUNCTIONS OF EDGE CLASS


Edge::Edge(const VertexType& v1, const VertexType& v2) :
	v(&v1), w(&v2)
{}

bool
Edge::operator ==(const Edge& e) const
{
	return (*(this->v) == *e.v && *(this->w) == *e.w) ||
		(*(this->v) == *e.w && *(this->w) == *e.v);
}

std::ostream&
operator <<(std::ostream& str, const Edge& e)
{
	return str << "Edge " << *e.v << ", " << *e.w;
}

bool almost_equal(const Edge& e1, const Edge& e2)
{
	return	(almost_equal(*e1.v, *e2.v) && almost_equal(*e1.w, *e2.w)) ||
		(almost_equal(*e1.v, *e2.w) && almost_equal(*e1.w, *e2.v));
}

// DEFINING THE MAIN FUNCTION:



#include <SFML/Graphics.hpp>


int main(int argc, char* argv[])
{
	int numberPoints = 1000;
	if (argc > 1)  numberPoints = atoi(argv[1]);
	std::default_random_engine eng(std::random_device{}());
	std::uniform_real_distribution<double> dist_w(0, 800);
	std::uniform_real_distribution<double> dist_h(0, 600);
	std::cout << "Generating " << numberPoints << " random points" << std::endl;
	std::vector<Vector2> points; // vector2 type contains two variables x and y, so points vector will contain
								 // a vector each one containing two variables.
	for (int i = 0; i < numberPoints; ++i) points.push_back(Vector2{ dist_w(eng), dist_h(eng) }); // will transform the incoming variables into vector2 type

	Delaunay triangulation;	// Triangulation is an object of the dealunay class so it will contain
							// 3 vectors which are declared as private members of the delaunay class namely
							// _triangles,_vertices and _edges 
	const auto start = std::chrono::high_resolution_clock::now(); // chrono is used for measuring time
	const std::vector<Triangle> triangles = triangulation.triangulate(points); // 
	const auto end = std::chrono::high_resolution_clock::now();
	const std::chrono::duration<double> diff = end - start;

	std::cout << triangles.size() << " triangles generated in " << diff.count()
		<< "s\n";
	//	return 0;
	const std::vector<Edge> edges = triangulation.getEdges();
	std::cout << "hello";
	//for (auto x : triangles) std::cout << x << "\n";

	// SFML window // SF  // sf:: is similar to std::
	sf::RenderWindow window(sf::VideoMode(800, 600), "Delaunay triangulation"); // Declares and creates a new window
																				// named Delaunay triangulation. Video mode is used to define the width(in pixels) and height(bits per pixel) of the window
	window.setFramerateLimit(1); // limit the framerate to a larger extent so that the window is opened for a suitable time

	// Transform each points of each vector as a rectangle
	for (const auto p : points) {
		sf::RectangleShape s{ sf::Vector2f(4, 4) };  //sf::Vector2 is a simple class that defines a mathematical vector with two coordinates (x and y).
		s.setPosition(static_cast<float>(p.x), static_cast<float>(p.y));
		window.draw(s);
	}

	std::vector<std::array<sf::Vertex, 2> > lines;
	for (const auto& e : edges) {
		const std::array<sf::Vertex, 2> line{ {
			sf::Vertex(sf::Vector2f(
					static_cast<float>(e.v->x + 2.0),  // the added 2.0 is used to increase the gap between two edges and give more clarity  
					static_cast<float>(e.v->y + 2.0))),
			sf::Vertex(sf::Vector2f(
					static_cast<float>(e.w->x + 2.0),
					static_cast<float>(e.w->y + 2.0))),
		} };
		window.draw(std::data(line), 2, sf::Lines);
	}

	window.display();

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
	}

	return 0;
}



