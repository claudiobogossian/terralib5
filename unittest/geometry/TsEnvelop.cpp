/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

    This file is part of the TerraLib - a Framework for building GIS enabled applications.

    TerraLib is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License,
    or (at your option) any later version.

    TerraLib is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with TerraLib. See COPYING. If not, write to
    TerraLib Team at <terralib-team@terralib.org>.
 */

// Unit-Test TerraLib
#include "TsEnvelop.h"

// TerraLib
//#include <terralib/common.h>
//#include <terralib/geometry.h>

// STL
//#include <cstdio>
//#include <cstdlib>
//#include <cstring>

CPPUNIT_TEST_SUITE_REGISTRATION( TsEnvelop );

void TsEnvelop::setUp()
{
  boxA = new te::gm::Envelope(-43.9329795837402, -20.6328010559082, -43.4036407470703, -20.1612071990967);
  boxAEqual = new te::gm::Envelope(-43.9329795837402, -20.6328010559082, -43.4036407470703, -20.1612071990967);

  boxB= new te::gm::Envelope(-43.7329795837402, -20.3328010559082, 10.4036407470703, 10.1612071990967);
  boxC= new te::gm::Envelope(-43.4036407470703, -20.6328010559082, 0, -20.1612071990967);
  boxD= new te::gm::Envelope(-43.8329795837402, -20.4328010559082, -43.6036407470703, -20.2612071990967);
  boxE= new te::gm::Envelope(43.9329795837402,  -20.6328010559082, 43.4036407470703, -20.1612071990967);
  boxF= new te::gm::Envelope(-43.7329795837402, -25.6328010559082, 10.4036407470703, -20.3328010559082); //touch B by a horiz line
  boxG= new te::gm::Envelope(-43.4036407470703, -20.1612071990967, 25.4036407470703, 10.1612071990967 ); //touch A by a point)
  boxH= new te::gm::Envelope(-43.7329795837402, 10.1612071990967, 0, 25.1612071990967 ); //touch G by a segment of line)

  boxAeB= new te::gm::Envelope(-43.7329795837402,  -20.3328010559082 , -43.4036407470703,  -20.1612071990967);
  boxAeF= new te::gm::Envelope(-43.7329795837402,  -20.6328010559082 , -43.4036407470703, -20.3328010559082);

  boxAexpandedB= new te::gm::Envelope(-43.9329795837402, -20.6328010559082,10.4036407470703, 10.1612071990967);
  boxGexpandedA= new te::gm::Envelope(-43.9329795837402, -20.6328010559082,25.4036407470703, 10.1612071990967);

}

void TsEnvelop::tearDown()
{
  delete boxA;
  delete boxAEqual;

  delete boxB;
  delete boxC;
  delete boxD;
  delete boxE;
  delete boxF; 
  delete boxG;
  delete boxH;

  delete boxAeB;
  delete boxAeF;
  delete boxAexpandedB;
}

void TsEnvelop::tcEnvelopInvalid()
{
//#ifdef TE_COMPILE_ALL
  te::gm::Envelope* box = new te::gm::Envelope();

  CPPUNIT_ASSERT(box->m_llx  == std::numeric_limits<double>::max() );
  CPPUNIT_ASSERT(box->m_lly  == std::numeric_limits<double>::max() );
  CPPUNIT_ASSERT(box->m_urx  == -(std::numeric_limits<double>::max() ) );
  CPPUNIT_ASSERT(box->m_ury  == -(std::numeric_limits<double>::max() ) );
  delete box;

//#endif
}

void TsEnvelop::tcEnvelopValid()
{
//#ifdef TE_COMPILE_ALL
  te::gm::Envelope* box = new te::gm::Envelope(1.1,1.1,2.2,2.2);
  CPPUNIT_ASSERT(box->m_llx  ==   1.1);
  CPPUNIT_ASSERT(box->m_lly  ==   1.1);
  CPPUNIT_ASSERT(box->m_urx  ==  2.2);
  CPPUNIT_ASSERT(box->m_ury  ==  2.2);
  delete box;

//#endif
}

void TsEnvelop::tcEnvelopCopy()
{
//#ifdef TE_COMPILE_ALL
  te::gm::Envelope box_rhs(1.1,1.1,2.2,2.2);
  te::gm::Envelope box(box_rhs);

  CPPUNIT_ASSERT(box.m_llx  ==  1.1); 
  CPPUNIT_ASSERT(box.m_lly  ==  1.1);
  CPPUNIT_ASSERT(box.m_urx  ==  2.2);
  CPPUNIT_ASSERT(box.m_ury  ==  2.2);

//#endif
}

void TsEnvelop::tcEnvelopInit()
{
//#ifdef TE_COMPILE_ALL
  te::gm::Envelope box;
  box.init(1.1,1.1,2.2,2.2);

  CPPUNIT_ASSERT(box.m_llx  ==  1.1);
  CPPUNIT_ASSERT(box.m_lly  ==  1.1);
  CPPUNIT_ASSERT(box.m_urx  ==  2.2);
  CPPUNIT_ASSERT(box.m_ury  ==  2.2);

//#endif
}

void TsEnvelop::tcEquals()
{
  CPPUNIT_ASSERT(boxA->equals(*boxAEqual) ==  true);
}

void TsEnvelop::tcGetLowerLeftX()
{
//#ifdef TE_COMPILE_ALL
  te::gm::Envelope box;
  box.init(1.1,1.2,2.1,2.2);
  const double llx = box.getLowerLeftX();
  CPPUNIT_ASSERT(llx  ==  1.1);

  //Testing big number.decimal
  const double llxB = boxB->getLowerLeftX();
  CPPUNIT_ASSERT(llxB  ==  -43.7329795837402);

//#endif
}

void TsEnvelop::tcGetLowerLeftY()
{
//#ifdef TE_COMPILE_ALL
  te::gm::Envelope box;
  box.init(1.1,1.2,2.1,2.2);
  const double lly = box.getLowerLeftY();
  CPPUNIT_ASSERT(lly  ==  1.2);

  //Testing big number.decimal
  const double llyB = boxB->getLowerLeftY();
  CPPUNIT_ASSERT(llyB  ==  -20.3328010559082);

//#endif
}

void TsEnvelop::tcGetUpperRightX()
{
//#ifdef TE_COMPILE_ALL
  te::gm::Envelope box;
  box.init(1.1,1.2,2.1,2.2);
  const double urx = box.getUpperRightX();
  CPPUNIT_ASSERT(urx  ==  2.1);

  //Testing big number.decimal
  const double urxB = boxB->getUpperRightX();
  CPPUNIT_ASSERT(urxB  ==   10.4036407470703);

//#endif
}

void TsEnvelop::tcGetUpperRightY()
{
//#ifdef TE_COMPILE_ALL
  te::gm::Envelope box;
  box.init(1.1,1.2,2.1,2.2);
  const double ury = box.getUpperRightY();
  CPPUNIT_ASSERT(ury  ==  2.2);

  //Testing big number.decimal
  const double uryB = boxB->getUpperRightY();
  CPPUNIT_ASSERT(uryB  ==  10.1612071990967);

//#endif
}

void TsEnvelop::tcGetLowerLeftCoord2D()
{
//#ifdef TE_COMPILE_ALL
  te::gm::Envelope box;
  box.init(1.1,1.2,2.1,2.2);
  te::gm::Coord2D llCoord2D  = box.getLowerLeft();
  CPPUNIT_ASSERT(llCoord2D.x  ==  1.1);
  CPPUNIT_ASSERT(llCoord2D.y  ==  1.2);

//#endif
}

void TsEnvelop::tcGetUpperRightCoord2D()
{
//#ifdef TE_COMPILE_ALL
  te::gm::Envelope box;
  box.init(1.1,1.2,2.1,2.2);
  te::gm::Coord2D urCoord2D  = box.getUpperRight();
  CPPUNIT_ASSERT(urCoord2D.x  ==  2.1);
  CPPUNIT_ASSERT(urCoord2D.y  ==  2.2);

//#endif
}

void TsEnvelop::tcMakeInvalid()
{
//#ifdef TE_COMPILE_ALL
  te::gm::Envelope box;
  box.init(1.1,1.1,2.2,2.2);
  CPPUNIT_ASSERT(box.m_llx  ==  1.1);
  CPPUNIT_ASSERT(box.m_lly  ==  1.1);
  CPPUNIT_ASSERT(box.m_urx  ==  2.2);
  CPPUNIT_ASSERT(box.m_ury  ==  2.2);

  box.makeInvalid();
  CPPUNIT_ASSERT(box.m_llx  == std::numeric_limits<double>::max() );
  CPPUNIT_ASSERT(box.m_lly  == std::numeric_limits<double>::max() );
  CPPUNIT_ASSERT(box.m_urx  == -(std::numeric_limits<double>::max()) );
  CPPUNIT_ASSERT(box.m_ury  == -(std::numeric_limits<double>::max()) );

//#endif
}

void TsEnvelop::tcIsValidRect()
{
//#ifdef TE_COMPILE_ALL
  te::gm::Envelope box;
  box.init(1.1,1.2,2.1,2.2);
  bool isValidRect = box.isValid();
  CPPUNIT_ASSERT(isValidRect  ==  true);
  box.init(2.1,1.2,2.0,2.2);
  isValidRect = box.isValid();
  CPPUNIT_ASSERT(isValidRect  ==  false);

//#endif
}

void TsEnvelop::tcGetWidth_Right_Area()
{
//#ifdef TE_COMPILE_ALL
  te::gm::Envelope box;
  box.init(1.1,1.1,2.2,2.2);
  double width = box.getWidth();
  double right = box.getHeight();
  double area = box.getArea();
  CPPUNIT_ASSERT(width ==  1.1);
  CPPUNIT_ASSERT(right ==  1.1);
  //CPPUNIT_ASSERT_area  ==  1.21);
  CPPUNIT_ASSERT_DOUBLES_EQUAL(1.21, area, 0.000000001); 

//#endif
}

void TsEnvelop::tcSpatialOperations()
{
//#ifdef TE_COMPILE_ALL
  ////////te::gm::Envelope boxA(1.0,1.0,2.0,2.0);
  ////////te::gm::Envelope boxAEqual(1.0,1.0,2.0,2.0);

  ////////te::gm::Envelope boxB(1.5,1.5,2.5,2.5);
  ////////te::gm::Envelope boxC(2.0,1.0,3.0,2.0);
  ////////te::gm::Envelope boxD(1.4,1.4,1.8,1.8);
  ////////te::gm::Envelope boxE(10.22,10.22,11.22,11.22);
  ////////te::gm::Envelope boxAeB(1.5,1.5,2.0,2.0);

  ////////te::gm::Envelope boxAexpandedB(1.0,1.0,2.5,2.5);


  ////////CPPUNIT_ASSERT(boxA.equals( boxAEqual) ==  true);
  ////////CPPUNIT_ASSERT(boxA.intersects( boxB) ==  true);
  ////////CPPUNIT_ASSERT(boxA.intersects( boxC) ==  true); // only a vertical line
  ////////CPPUNIT_ASSERT(boxA.intersects( boxE) ==  false); // disjoint

  ////////CPPUNIT_ASSERT(boxA.touches( boxC) ==  true);
  ////////CPPUNIT_ASSERT(boxA.contains( boxD) ==  true);
  ////////CPPUNIT_ASSERT(boxD.within( boxA) ==  true);
  ////////CPPUNIT_ASSERT(boxA.disjoint( boxE) ==  true);
  ////////te::gm::Envelope* boxAeB_res = boxA.intersection( boxB);
  ////////CPPUNIT_ASSERT(boxAeB_res->equals(boxAeB));

  ////////boxA.expand(boxB);
  ////////CPPUNIT_ASSERT(boxA.equals(boxAexpandedB) ==  true);

//#endif
}

void TsEnvelop::tcIntersects()
{
  CPPUNIT_ASSERT(boxA->intersects(*boxB) ==  true);
  CPPUNIT_ASSERT(boxA->intersects(*boxC) ==  true); //by a vertical line
  CPPUNIT_ASSERT(boxC->intersects(*boxA) ==  true); //by a vertical line

  CPPUNIT_ASSERT(boxA->intersects(*boxE) ==  false); // disjoint
  CPPUNIT_ASSERT(boxA->intersects(*boxF) ==  true);
  CPPUNIT_ASSERT(boxA->intersects(*boxG) ==  true); //by a point (urA)
  CPPUNIT_ASSERT(boxB->intersects(*boxF) ==  true); //by a horiz line
  CPPUNIT_ASSERT(boxF->intersects(*boxB) ==  true); //by a horiz line
  CPPUNIT_ASSERT(boxG->intersects(*boxH) ==  true); //by a segment of horiz line

}

void TsEnvelop::tcDisjoint()
{
  CPPUNIT_ASSERT(boxA->disjoint( *boxE) ==  true);
  CPPUNIT_ASSERT(boxB->disjoint( *boxE) ==  true);
  CPPUNIT_ASSERT(boxC->disjoint( *boxE) ==  true);
  CPPUNIT_ASSERT(boxD->disjoint( *boxE) ==  true);
  CPPUNIT_ASSERT(boxG->disjoint(*boxH) ==  false); //by a segment of horiz line

}

void TsEnvelop::tcTouches()
{
  CPPUNIT_ASSERT(boxA->touches( *boxE) ==  false);
  CPPUNIT_ASSERT(boxA->touches( *boxB) ==  false);
  CPPUNIT_ASSERT(boxA->touches( *boxC) ==  true); //by a vertical line
  CPPUNIT_ASSERT(boxA->touches( *boxG) ==  true); //by point corner urA

  CPPUNIT_ASSERT(boxB->touches( *boxF) ==  true); //by a vert line
  CPPUNIT_ASSERT(boxF->touches( *boxB) ==  true); //by a vert line

  CPPUNIT_ASSERT(boxB->touches(*boxF) ==  true); //by a horiz line
  CPPUNIT_ASSERT(boxF->touches(*boxB) ==  true); //by a horiz line

  CPPUNIT_ASSERT(boxG->touches(*boxH) ==  true); //by a segment of horiz line
  CPPUNIT_ASSERT(boxH->touches(*boxG) ==  true); //by a segment of horiz line
}

void TsEnvelop::tcIntersection()
{
  te::gm::Envelope boxAeB_res = boxA->intersection( *boxB); //rectangle
  CPPUNIT_ASSERT(boxAeB_res.equals(*boxAeB) == true);
  CPPUNIT_ASSERT_MESSAGE("Intersection should be a rectangle ", boxAeB_res.equals(*boxAeB) ==  true); 

  te::gm::Envelope boxAeC = boxA->intersection( *boxC); //by a vertical  line
  bool isLine = (boxAeC.m_llx == boxAeC.m_urx); 
  CPPUNIT_ASSERT_MESSAGE("Intersection should be a vertical line ", isLine ==  true); 

  te::gm::Envelope boxAeG = boxA->intersection( *boxG); 
  bool isPoint = ((boxAeG.m_llx == boxAeG.m_urx) && (boxAeG.m_lly == boxAeG.m_ury)); //it is a point
  CPPUNIT_ASSERT_MESSAGE("Intersection should be a point ", isPoint ==  true);

  te::gm::Envelope boxBeF = boxB->intersection( *boxF); //by a horiz line
  isLine = (boxBeF.m_lly == boxBeF.m_ury);
  CPPUNIT_ASSERT_MESSAGE("Intersection should be a horizontal line ", isLine ==  true); 

  te::gm::Envelope boxGeH = boxG->intersection( *boxH); //by a segment of horiz line
  isLine = (boxGeH.m_lly == boxGeH.m_ury && boxGeH.m_llx != boxGeH.m_urx);
  CPPUNIT_ASSERT_MESSAGE("Intersection should be a segment of horiz line ", isLine ==  true);
}


void TsEnvelop::tcContains()
{
  CPPUNIT_ASSERT(boxA->contains( *boxD) ==  true);
}
void TsEnvelop::tcWithin()
{
  CPPUNIT_ASSERT(boxD->within( *boxA) ==  true);
}

void TsEnvelop::tcUnion()
{
  boxA->Union(*boxB);
  CPPUNIT_ASSERT(boxA->equals(*boxAexpandedB) ==  true);

  boxG->Union(*boxA);
  CPPUNIT_ASSERT(boxG->equals(*boxGexpandedA) ==  true);
}

void TsEnvelop::tcSpatialOperations1()
{
//#ifdef TE_COMPILE_ALL
  //OuroPreto boxA
  //te::gm::Envelope boxA(-43.9329795837402, -20.6328010559082, -43.4036407470703, -20.1612071990967);
  //te::gm::Envelope boxAEqual(-43.9329795837402, -20.6328010559082, -43.4036407470703, -20.1612071990967);

  //te::gm::Envelope boxB(-43.7329795837402, -20.3328010559082, 10.4036407470703, 10.1612071990967);
  //te::gm::Envelope boxC(-43.4036407470703, -20.6328010559082, 0, -20.1612071990967);
  //te::gm::Envelope boxD(-43.8329795837402, -20.4328010559082, -43.6036407470703, -20.2612071990967);
  //te::gm::Envelope boxE(43.9329795837402,  -20.6328010559082, 43.4036407470703, -20.1612071990967);
  //te::gm::Envelope boxF(-43.7329795837402, -25.6328010559082, 10.4036407470703, -20.3328010559082); //touch B by a horiz line
  //te::gm::Envelope boxG(-43.4036407470703, -20.1612071990967, 10.4036407470703, 10.1612071990967 ); //touch A by a point)

  //te::gm::Envelope boxAeB(-43.7329795837402,  -20.3328010559082 , -43.4036407470703,  -20.1612071990967);
  //te::gm::Envelope boxAeF(-43.7329795837402,  -20.6328010559082 , -43.4036407470703, -20.3328010559082);

  //te::gm::Envelope boxAexpandedB(-43.9329795837402, -20.6328010559082,10.4036407470703, 10.1612071990967);


  //CPPUNIT_ASSERT(boxA->equals(*boxAEqual) ==  true);
  //CPPUNIT_ASSERT(boxA->intersects(*boxB) ==  true);
  //CPPUNIT_ASSERT(boxA->touches( *boxB) ==  false);

  //CPPUNIT_ASSERT(boxA->intersects( boxC) ==  true); // only a vertical line
  //CPPUNIT_ASSERT(boxC->intersects( boxA) ==  true); // only a vertical line
  //CPPUNIT_ASSERT(boxA->touches( boxC) ==  true); // touches by a vertical line
  //te::gm::Envelope* boxAeC = boxA->intersection( boxC); 
  //bool isLine = boxAeC->isValid(); //it is a vertical line

  //CPPUNIT_ASSERT(boxA->intersects( boxE) ==  false); // disjoint
  //CPPUNIT_ASSERT(boxA->intersects( boxF) ==  true);
  //CPPUNIT_ASSERT(boxA->intersects( boxG) ==  true); // only a point (urA)
  //CPPUNIT_ASSERT(boxA->touches( boxG) ==  true); // only a poin corner urA
  //te::gm::Envelope* boxAeG = boxA->intersection( boxG); 
  //bool isPoint = boxAeG->isValid(); //it is a point
  ////CPPUNIT_ASSERT(boxAeG->isValid() ==  false); //it is a point

  //CPPUNIT_ASSERT(boxA->contains( boxD) ==  true);
  //CPPUNIT_ASSERT(boxD->within( boxA) ==  true);
  //CPPUNIT_ASSERT(boxA->disjoint( boxE) ==  true);

  //te::gm::Envelope* boxAeB_res = boxA->intersection( boxB);
  //CPPUNIT_ASSERT(boxAeB_res->equals(boxAeB) == true);
  //te::gm::Envelope* boxAeF_res = boxA->intersection( boxF);
  //CPPUNIT_ASSERT(boxAeF_res->equals(boxAeF) == true);

  //CPPUNIT_ASSERT(boxB->touches( boxF) ==  true); //by line
  //CPPUNIT_ASSERT(boxF->touches( boxB) ==  true); //by line
  //CPPUNIT_ASSERT(boxB->intersects( boxF) ==  true); //by horiz line
  //CPPUNIT_ASSERT(boxF->intersects( boxB) ==  true); //by horiz line

  //te::gm::Envelope* boxBeF = boxB->intersection( boxF); //returns a envelop, but it is a horiz line
  //bool isLineHoriz = boxBeF->isValid(); 
  //boxA->expand(boxB);
  //CPPUNIT_ASSERT(boxA->equals(boxAexpandedB) ==  true);

  //delete boxAeC;
  //delete boxAeG;
  //delete boxAeB_res;
  //delete boxAeF_res;
  //delete boxBeF;

//#endif
}
