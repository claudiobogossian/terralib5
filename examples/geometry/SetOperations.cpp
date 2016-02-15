// Examples
#include "GeometryExamples.h"

te::gm::Geometry* difference(te::gm::Geometry* g1, te::gm::Geometry* g2)
{
	return g1->difference(g2);
}

te::gm::Geometry* intersection(te::gm::Geometry* g1, te::gm::Geometry* g2)
{
	return g1->intersection(g2);
}

te::gm::Geometry* Union(te::gm::Geometry* g1, te::gm::Geometry* g2)
{
	return g1->Union(g2);
}
