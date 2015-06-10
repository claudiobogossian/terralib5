%rename (IntPair) IntPairStruct;

%ignore IntPairStruct::operator==;

%{
#include "terralib/sam/rtree/Index.h"

struct IntPairStruct 
{
	enum GeomType 
	{
		POINT,
		SEGMENT
	};
	
	bool operator==(const IntPairStruct& other)
	{
		return (m_pol == other.m_pol && m_pos == other.m_pos);
	}
	
	int m_pol;
	int m_pos;
};
%}

struct IntPairStruct 
{
	enum GeomType 
	{
		POINT,
		SEGMENT
	};
	
	bool operator==(const IntPairStruct& other)
	{
		return (m_pol == other.m_pol && m_pos == other.m_pos);
	}
	
	int m_pol;
	int m_pos;
};

namespace std {
	%template (IntPairVector) vector< IntPairStruct >;
}

%include "terralib/sam/rtree/Index.h"

namespace te { namespace sam { namespace rtree {
	%template (IntPairRTree) Index< IntPairStruct >; 
} } }

