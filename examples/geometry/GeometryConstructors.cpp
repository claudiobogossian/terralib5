// Examples
#include "GeometryExamples.h"

te::gm::Point* createPoint(const double& x, const double& y)
{
	te::gm::Point* p = new te::gm::Point(x, y, 0, 0);
	return p;
}

te::gm::LineString* createLineString(const double& xi, const double& yi,
																		 const double& xf, const double& yf)
{
	te::gm::LineString* l = new te::gm::LineString(2, te::gm::LineStringType);
	l->setPoint(0, xi, yi);
	l->setPoint(1, xf, yf);
	return l;
}

te::gm::LinearRing* createSquare(const double& xc, const double& yc, const double& size)
{
	te::gm::LinearRing* s = new te::gm::LinearRing(5, te::gm::LineStringType);
	double halfSize = size * 0.5;
	s->setPoint(0, xc - halfSize, yc - halfSize); // lower left 
	s->setPoint(1, xc - halfSize, yc + halfSize); // upper left
	s->setPoint(2, xc + halfSize, yc + halfSize); // upper rigth
	s->setPoint(3, xc + halfSize, yc - halfSize); // lower rigth
	s->setPoint(4, xc - halfSize, yc - halfSize); // closing
	return s;
}

te::gm::Polygon* createPolygon()
{
	te::gm::Polygon* p = new te::gm::Polygon(0, te::gm::PolygonType);
	p->push_back(createSquare(5, 5, 10));
	return p;
}

te::gm::Polygon* createPolygonWithHole()
{
	te::gm::Polygon* p = new te::gm::Polygon(0, te::gm::PolygonType);
	p->push_back(createSquare(5, 5, 10)); // external square
	p->push_back(createSquare(5, 5, 5));  // internal square (the hole!)
	return p;
}

te::gm::GeometryCollection* createGeometryCollection()
{
	te::gm::GeometryCollection* gColl = new te::gm::GeometryCollection(0, te::gm::GeometryCollectionType); 	
	gColl->add(createPoint(0.0, 0.0));
	gColl->add(createLineString(0.0, 0.0, 10.0, 10.0));
	gColl->add(createLineString(0.0, 10.0, 10.0, 0.0));
	gColl->add(createPolygon());
	gColl->add(createPolygonWithHole());
	return gColl;
}
