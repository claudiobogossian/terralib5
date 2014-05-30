// Examples
#include "GeometryExamples.h"

bool spatialRelation(te::gm::Geometry* g1, te::gm::Geometry* g2, te::gm::SpatialRelation relation)
{
	switch(relation)
	{
		case te::gm::CONTAINS:
			return g1->contains(g2);

		case te::gm::COVEREDBY:
			return g1->coveredBy(g2);

		case te::gm::COVERS:
			return g1->covers(g2);

		case te::gm::CROSSES:
			return g1->crosses(g2);

		case te::gm::DISJOINT:
			return g1->disjoint(g2);

		case te::gm::EQUALS:
			return g1->equals(g2);

		case te::gm::INTERSECTS:
			return g1->intersects(g2);

		case te::gm::OVERLAPS:
			return g1->overlaps(g2);

		case te::gm::TOUCHES:
			return g1->touches(g2);

		case te::gm::WITHIN:
			return g1->within(g2);
            
        default:
            return false;
	}

	return false;
}

