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
   
  \brief Utility class with functions to manipulate the canvas and conversion between projections.

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
    /*!
	    \brief Utility class with functions to manipulate the canvas and conversion between projections.
	  
	    \ingroup layout
	  */
    class TELAYOUTEXPORT Utils
    {
      public:

        /*!
          \brief Constructor
        */ 
        Utils();

        /*!
          \brief Destructor
        */
        virtual ~Utils();

        /*!
          \brief Draw a rectangle in world coordinates (mm).
		  
		      \param box in mm
        */ 
        virtual void drawRectW(te::gm::Envelope box);

        /*!
          \brief Draw a line in world coordinates (mm).
		  
		      \param LinearRing pointer with coordinates in millimeters	  
        */ 
        virtual void drawLineW(te::gm::LinearRing* line);
        
        /*!
          \brief Returns a pointer RGBA colors that representing an image.
		  
		      \param box in mm	  
          \return The Pointer RGBA colors
        */ 
        virtual te::color::RGBAColor** getImageW(te::gm::Envelope boxmm);

        /*!
          \brief Millimeter to pixel
		  
		      \param millimeter value
          \return pixel value
        */ 
        virtual int mm2pixel(double mm);

        /*!
          \brief Creates a LinearRing (line) pointer from a box in world coordinates (mm) 
		  
		      \param box in mm
          \return new line
        */ 
        virtual te::gm::LinearRing* createSimpleLine(te::gm::Envelope box);

        /*!
          \brief Creates a line with n points in x axis. Method used to create the grid lines on a map. Horizontal line.
            
		  
		      \param box in mm
          \param axisCoord axis y value
          \param gap distance between the points on the x axis
          \return
        */ 
        virtual te::gm::LinearRing* addCoordsInX(te::gm::Envelope box, double axisCoord, double gap);

        /*!
          \brief Creates a line with n points in y axis. Method used to create the grid lines on a map. Vertical line.
		  
          \param box in mm
          \param axisCoord axis x value
          \param gap distance between the points on the y axis
          \return
        */ 
        virtual te::gm::LinearRing* addCoordsInY(te::gm::Envelope box, double axisCoord, double gap);

        /*!
          \brief Sets the viewport and window of the canvas. The viewport is only changed if the resize parameter is true.
		  
		      \param box window coordinate (mm)	  
        */ 
        virtual void configCanvas(te::gm::Envelope box, bool resize = true, bool applyZoom = true);
                     
        /*!
          \brief Converts the box world (mm) to screen coordinates (pixel).
		  
		      \param box in mm	  
          \return box in pixel
        */ 
        virtual te::gm::Envelope viewportBox(te::gm::Envelope box);

        /*!
          \brief A method that calculates the height and width of a text.
		  
		      \param w returns the width	  
          \param h returns the height
          \param text 
        */ 
        virtual void textBoundingBox(double &w, double &h, std::string txt);
        
        /*!
          \brief Returns a WorldTransformer object to transformations between geo coordinates and millimeter coordinates.
          		  
		      \param box in geo coordinates 	  
          \return box in mm
        */ 
        te::layout::WorldTransformer getTransformGeo(te::gm::Envelope boxgeo, te::gm::Envelope boxmm);

        /*!
          \brief Converts decimal geo coordinates to degrees.
		  
		      \param value in decimal
          \param bDegrees true if should appear in the return string, false otherwise
          \param bMinutes true if should appear in the return string, false otherwise
          \param bSeconds true if should appear in the return string, false otherwise
          \return string value in degree
        */ 
        virtual std::string convertDecimalToDegree(const double& value, bool bDegrees, bool bMinutes, bool bSeconds);

        /*!
          \brief Converts degree geo coordinates to decimal.
		  	  
          \return double value in decimal
        */ 
        virtual double convertDegreeToDecimal();

        /*!
          \brief Number to string.
		  
		      \param value 
          \param precision decimal places
          \return number as string
        */ 
        virtual std::string convertNumberToString(const double& value, int precision);

        /*!
          \brief Rounds double to int.
		  
		      \param value	 
          \return number rounded
        */ 
        virtual int roundNumber(const double& value);

        /*!
          \brief Returns string wkt with UTM projection in the specified zone.
		  
		      \param zone UTM	
          \return string wkt
        */
        std::string proj4DescToPlanar(int zone);

        /*!
          \brief Returns string wkt with non-planar projection.
		 
          \return wkt
        */
        std::string proj4DescToGeodesic();

        /*!
          \brief Calculates the area from a box in coordinated latlong
		  
		      \param latLongBox box in latlong
          \return UTM zone
        */
        int calculatePlanarZone(te::gm::Envelope latLongBox);

        /*!
          \brief Returns a UnitOfMeasurePtr pointer.
		  
		      \param srid projection number	
          \return A UnitOfMeasurePtr pointer
        */
        te::common::UnitOfMeasurePtr unitMeasure(int srid);

        /*!
          \brief Map latlong to UTM zone.
		  
		      \param box in latlong
          \param zone returns UTM zone
        */
        virtual void remapToPlanar(te::gm::Envelope* latLongBox, int zone);

        /*!
          \brief Map latlong LinearRing (line) to UTM zone.
		  
		      \param line line in latlong
          \param zone returns UTM zone
        */
        virtual void remapToPlanar(te::gm::LinearRing* line, int zone);

        /*!
          \brief Map latlong Point (point) to UTM zone.
		  
		      \param zone returns UTM zone	  
        */
        virtual void remapToPlanar(te::gm::Point* point, int zone);

        /*!
          \brief Convert LinearRing from one coordinate system to mm
		  
		      \param Object with logic for transforming
          \param line LinearRing pointer in one coordinate system
        */
        virtual void convertToMillimeter(WorldTransformer transf, te::gm::LinearRing* line); 

        /*!
          \brief Convert Polygon from one coordinate system to mm
		  
		      \param Object with logic for transforming
          \param line LinearRing pointer in one coordinate system
        */
        virtual void convertToMillimeter(WorldTransformer transf, te::gm::Polygon* poly); 

        /*!
          \brief Opens the file and loads the image into memory and draws on canvas.
		  
		      \param fileName full path to image
          \param box in mm
        */
        virtual void drawImage(std::string fileName, te::gm::Envelope box);

        /*!
          \brief Opens the file and loads the image into memory and converts to char*.
		  
		      \param fileName full path to image 	  
          \return size length of file
        */
        virtual char* imageToChar(std::string fileName, std::ifstream::pos_type &size);

        /*!
          \brief Returns the file extension.
		  
		      \param fileName full path to image	
          \return file extension
        */
        virtual std::string getFileExtension(std::string fileName);

        /*!
          \brief Returns the file type. Ex.: .png (te::map::PNG).
		  
		      \param filName full path to image  	  
          \return file type
        */
        virtual te::map::ImageType getFileExtensionType(std::string filName);

        /*!
          \brief 
		  
		      \param 	  
        */
        virtual void setApplyZoom(bool apply);

        /*!
          \brief 
		  
		      \param 	 
          \return
        */
        virtual bool getApplyZoom();

        /*!
          \brief Clears the canvas content and fills with the background color. Sets all width with 1. 
        */
        virtual void resetCanvas();
                
      protected:
        
        /*!
          \brief Sets the viewport and window of the canvas. The viewport is only changed if the resize parameter is true. 
		  
		      \param viewport viewport box(pixel)
          \param world world box(mm)
          \param resize true if to resize, false otherwise
        */
        virtual void changeCanvas(te::gm::Envelope viewport, te::gm::Envelope world, bool resize = true);

        /*!
          \brief Transforms the box (mm) to screen coordinates (pixel).
		  
		      \param transf logic for transforming from device coordinate to world coordinate and vice-versa. 	  
          \return box in screen coordinates (pixel)
        */
        virtual te::gm::Envelope transformToViewport(te::map::WorldDeviceTransformer transf, te::gm::Envelope box);
        
        /*!
          \brief Converts the box world (mm) to screen coordinates (pixel).
		  
		      \param box in mm	 
        */ 
        virtual te::gm::Envelope viewportBoxFromMM(te::gm::Envelope box);
        
      protected:

        bool                    m_applyZoom; //!<
    };
  }
}

#endif 
