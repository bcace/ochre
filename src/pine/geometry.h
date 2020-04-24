#ifndef geometry_h
#define geometry_h



namespace pine {

	struct Point
	{
		double x, y;

		Point();
		Point(double, double);

		Point &operator=(const Point &);
		Point operator+(const Point &) const;
		Point operator-(const Point &) const;
		Point operator*(double) const;
		Point &operator+=(double);
		Point &operator-=(double);
		bool operator<=(const Point &) const;
		bool operator>=(const Point &) const;
	};



	struct Rect
	{
		Point min, max;

		Rect();
		Rect(const Point &, const Point &);
		Rect(double, double, double, double);

		Rect &operator=(const Rect &);

		double W() const;
		double H() const;
		Point Center() const;
		bool Contains(const Point &) const;
	};



	struct Vertex
	{
		double x, y, z;

		Vertex() : x(0.0), y(0.0), z(0.0) {}
		Vertex(double _x, double _y, double _z) : x(_x), y(_y), z(_z) {}
	};



	struct Range
	{
		double min, max;

		Range() : min(0.0), max(0.0) {}

		Range(double _max) : min(0.0), max(_max) {}

		Range(double _min, double _max) :
			min(_min), max(_max) {}

		double Size() { return max - min; }

		void Absorb(const Range &r) {
			if (r.min < min) min = r.min;
			if (r.max > max) max = r.max;
		}
	};



	struct Box
	{
		Vertex min, max;
	};



	struct Bounds
	{
		Range x, y, z;

		Bounds();
		Bounds(double);
		Bounds(const Range &, const Range &, const Range &);
		Bounds(const Box &);
		Bounds(double *);

		double Diagonal();
		void Absorb(const Bounds &);
	};



	template<typename T>
	T CiMax(const T &a, const T &b) {
		if (a > b) return a;
		else return b;
	}

	template<typename T>
	T CiMin(const T &a, const T &b) {
		if (a < b) return a;
		else return b;
	}
}

#endif
