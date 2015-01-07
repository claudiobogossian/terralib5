/*  Copyright (C) 2001-2014 National Institute For Space Research (INPE) - Brazil.

    This file is part of the TerraLib - a Framework for building GIS enabled applications.

    TerraLib is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License,
    or (at your option) any later version.

    TerraLib is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with TerraLib. See COPYING. If not, write to
    TerraLib Team at <terralib-team@terralib.org>.
 */

/*!
  \file Utils.h
   
  \brief 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_UTILS_H 
#define __TERRALIB_LAYOUT_INTERNAL_UTILS_H

// TerraLib
#include "../../geometry/Envelope.h"
#include "../../geometry/LinearRing.h"
#include "../../color/RGBAColor.h"
#include "../../maptools/Canvas.h"
#include "../../maptools/WorldDeviceTransformer.h"
#include "WorldTransformer.h"
#include "../../common/UnitOfMeasure.h"
#include "../../maptools/Enums.h"
#include "../../geometry/Point.h"
#include "../../geometry/Polygon.h"
#include "Config.h"

// STL
#include <string>
#include <iostream>
#include <fstream>

const double	TeCDR =	0.01745329251994329576;		//!< Conversion factor: degrees to radians
const double	TeCRD = 57.29577951308232087679;	//!< Conversion factor: radians to degrees

namespace te
{
  namespace layout
  {
    class TELAYOUTEXPORT Utils
    {
      public:

        Utils();

        virtual ~Utils();

        virtual void drawRectW(te::gm::Envelope box);

        virtual void drawLineW(te::gm::LinearRing* line);
        
        virtual te::color::RGBAColor** getImageW(te::gm::Envelope boxmm);

        virtual int mm2pixel(double mm);

        virtual te::gm::LinearRing* createSimpleLine(te::gm::Envelope box);

        virtual te::gm::LinearRing* addCoordsInX(te::gm::Envelope box, double axisCoord, double gap);

        virtual te::gm::LinearRing* addCoordsInY(te::gm::Envelope box, double axisCoord, double gap);

        virtual void configCanvas(te::gm::Envelope box, bool resize = true, bool applyZoom = true);

        /* The calculation of the viewport is from the box in mm */
        virtual void configGeoCanvas(te::gm::Envelope boxgeo, te::gm::Envelope boxmm, bool resize = true, bool applyZoom = true);
                
        virtual te::gm::Envelope viewportBox(te::gm::Envelope box);

        virtual void textBoundingBox(double &w, double &h, std::string txt);
        
        te::layout::WorldTransformer getTransformGeo(te::gm::Envelope boxgeo, te::gm::Envelope boxmm);

        virtual std::string convertDecimalToDegree(const double& value, bool bDegrees, bool bMinutes, bool bSeconds);

        virtual double convertDegreeToDecimal();

        virtual std::string convertNumberToString(const double& value, int precision);

        /* Rounds double to int */
        virtual int roundNumber(const double& value);

        std::string proj4DescToPlanar(int zone);

        std::string proj4DescToGeodesic();

        int calculatePlanarZone(te::gm::Envelope latLongBox);

        te::common::UnitOfMeasurePtr unitMeasure(int srid);

        virtual void remapToPlanar(te::gm::Envelope* latLongBox, int zone);

        virtual void remapToPlanar(te::gm::LinearRing* line, int zone);

        virtual void remapToPlanar(te::gm::Point* point, int zone);

        virtual void convertToMillimeter(WorldTransformer transf, te::gm::LinearRing* line); 

        virtual void convertToMillimeter(WorldTransformer transf, te::gm::Polygon* poly); 

        virtual void drawImage(std::string fileName, te::gm::Envelope box);

        virtual char* imageToChar(std::string fileName, std::ifstream::pos_type &size);

        virtual std::string getFileExtension(std::string fileName);

        virtual te::map::ImageType getFileExtensionType(std::string filName);

        virtual void setApplyZoom(bool apply);

        virtual bool getApplyZoom();

        virtual void resetCanvas();
                
      protected:
        
        virtual void changeCanvas(te::gm::Envelope viewport, te::gm::Envelope world, bool resize = true);

        virtual te::gm::Envelope transformToViewport(te::map::WorldDeviceTransformer transf, te::gm::Envelope box);

        virtual te::gm::Envelope transformToMM(te::layout::WorldTransformer transf, te::gm::Envelope boxGeo);

        virtual te::gm::Envelope viewportBoxFromMM(te::gm::Envelope box);
        
      protected:

        bool                    m_applyZoom;
    };
  }
}

#endif 
