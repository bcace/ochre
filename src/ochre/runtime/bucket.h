#ifndef bucket_h
#define bucket_h

#include <vector>



struct OcRange
{
	int beg, end;

	OcRange(int _beg, int _end) : beg(_beg), end(_end) {}
};

typedef std::vector<OcRange> OcRanges;



class OcAgent;
class OcGrid;

class OcBucket : public std::vector<OcAgent *>
{
public:
	OcBucket();
	~OcBucket();

	void Clear(bool);
	OcAgent *Pop();

	OcRange &Range(int);
	void UpdateRanges(int);

private:
	OcRanges ranges;
};



class OcBuckets : public std::vector<OcBucket>
{
public:
	OcBuckets();
	OcBuckets(int &);

	void Clear(bool);
};

#endif
