/*  Copyright (C) 2001-2009 National Institute For Space Research (INPE) - Brazil.

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
  \file Converter.h

  \brief This file contains the support to convert coordinates from a SRS to another.
*/

#ifndef __TERRALIB_SRS_INTERNAL_CONVERTER_H 
#define __TERRALIB_SRS_INTERNAL_CONVERTER_H

#include <string>
#include <map>

#include "Config.h"

namespace te
{
	namespace srs
	{
	//! A Converter is responsible for the conversion of coordinates between different Coordinate Systems (CS).
	/*
		A Converter is responsible for the conversion of coordinates between two 
    different Coordinate Systems (CS) or a Spatial Reference System (SRS). 
    A CS can be uniquely identified by a numeric code. 

		This implementation is based on th PROJ4 cartographic library and only works if
    it has been enabled.

    \sa Proj4Dictionarly.

		\todo Methods to convert 3D coordinates.
	*/
	class TESRSEXPORT Converter
	{
	public:
		//! Default constructor.
		Converter();

		//! Constructor with parameters.
		/*!
			\param sourceSRID the code associated to the source SRS (input).
			\param targetSRID the code associated to the target SRS (input).
		*/
    Converter(int sourceSRID, int targetSRID);

		//! Destructor
		~Converter();

		//! Define the source SRS from a code. 
		/*!
			\param sourceSRID the code associated to the source SRS (input).
			\exception It throws an exception if the code associated 
                 to source SRS is not recognized.
		*/
		void setSourceSRID(int sourceSRID);

		//! Get the source SRS. 
		/*!
			\return the code associated to the source SRS.
		*/
		int getSourceSRID() const;

		//! Define the target SRS from a code. 
		/*!
			\param targetSRID the code associated to the target SRS (input).
			\exception It throws an exception if the code associated to 
                 target SRS is not recognized.
		*/
		void setTargetSRID(int targetSRID);

		//! Get the target SRS ID.  
		/*!
			\return the code associated to the target SRS.
		*/
		int getTargetSRID() const;

		//! Converts a vector of coordinates from source SRS to target SRS. 
		/*!
			Converts a vector of coordinates from source SRS to target SRS.

			Output vectors must be previously allocaded by the caller of this method. 
			Likewise the caller is responsible for deallocating it.

			\param xIn					pointer to array of X values in source SRS (input).
			\param yIn					pointer to array of Y valueS in source SRS (input).
			\param xOut					pointer to array of X values in target SRS (output). 
			\param yOut					pointer to array of X values in target SRS (output).
			\param numCoord			number of coordinates in the array (input).
			\param coordOffset	the step size from value to value (measured in doubles) 
                          within the x/y arrays (input).
			\return true if succeed and false otherwise.
		*/
		bool convert(double *xIn, double *yIn, double *xOut, double* yOut, long numCoord, int coordOffset=1) const;

		//! Converts a vector of coordinates from source SRS to target SRS. 
		/*!
			Converts a vector of coordinates from source SRS to target SRS.

			\param x						pointer to array of X values in source SRS as input 
                          and modified to target SRS for output.
			\param y						pointer to array of Y valueS in source SRS as input 
                          and modified to target SRS for output.
			\param numCoord			number of coordinates in the array (input).
			\param coordOffset	the step size from value to value (measured in doubles) 
                          within the x/y arrays (input).

			\return true if succeed and false otherwise.
		*/
		bool convert(double *x, double* y, long numCoord, int coordOffset=1) const;

		//! Converts a coordinate from source SRS to target SRS. 
		/*!
			Converts a coordinate from source SRS to target SRS.

			\param xIn					X value in source SRS (input).
			\param yIn					Y value in source SRS (input).
			\param xOut					X value in target SRS (output). 
			\param yOut					Y values in target SRS (output).
			\return true if succeed and false otherwise.
		*/
		bool convert(const double xIn, const double yIn, double &xOut, double &yOut) const;

		//! Converts a coordinate from source SRS to target SRS. 
		/*!
			Converts a coordinate from source SRS to target SRS.

			\param x	X value in source SRS as input and modified 
                  to target SRS for output.
			\param y	Y value in source SRS as input and modified 
                  to target SRS for output.
			\return true if succeed and false otherwise.
		*/
		bool convert(double &x, double &y) const;

		//! Inverts a vector of coordinates from source SRS to target SRS. 
		/*!
			Inverts a vector of coordinates from source SRS to target SRS.

			Output vectors must be previously allocaded by the caller of this method. 
			Likewise the caller is responsible for deallocating it.

			\param xIn					pointer to array of X values in target SRS (input).
			\param yIn					pointer to array of Y valueS in target SRS (input).
			\param xOut					pointer to array of X values in source SRS (output). 
			\param yOut					pointer to array of Y values in source SRS (output).
			\param numCoord			number of coordinates in the array (input).
			\param coordOffset	the step size from value to value (in double size) 
                          within the x/y arrays (input).
			\return true if succeed and false otherwise.
		*/
		bool invert(double *xIn, double *yIn, double *xOut, double* yOut, long numCoord, int coordOffset=1) const;

		//! Inverts a vector of coordinates from target SRS to source SRS. 
		/*!
			Inverts a vector of coordinates from target SRS to source SRS
			\param x						pointer to array of X values in target SRS as input 
                          and modified to source SRS for output.
			\param y						pointer to array of Y values in target SRS as input 
                          and modified to source SRS for output.
			\param numCoord			number of coordinates in the array (input).
			\param coordOffset	the step size from value to value in double size) 
                           within the x/y arrays (input).
			\return true if succeed and false otherwise.
		*/
		bool invert(double *x, double* y, long numCoord, int coordOffset=1) const;

		//! Inverts a coordinate from source SRS to target SRS. 
		/*!
			Inverts a coordinate from source SRS to target SRS.

			\param xIn	pointer to array of X values in target SRS (input).
			\param yIn	pointer to array of Y valueS in target SRS (input).
			\param xOut	pointer to array of X values in source SRS (output). 
			\param yOut	pointer to array of Y values in source SRS (output).
			\return true if succeed and false otherwise.
		*/
		bool invert(const double xIn, const double yIn, double &xOut, double &yOut) const;

		//! Inverts a coordinate from target SRS to source SRS. 
		/*!
			Inverts a coordinate from target SRS to source SRS
			\param x						pointer to array of X values in target SRS as input 
                          and modified to source SRS for output.
			\param y						pointer to array of Y values in target SRS as input 
                          and modified to source SRS for output.
			\return true if succeed and false otherwise.
		*/
		bool invert(double &x, double &y) const;

    //! Converts a coordinate from a projected SRS to its underlying geographic SRS. 
		/*!
			Converts a coordinate from a projected SRS to its underlying geographic SRS 
      (same Datum).

			\param x projected X-coordinate. Will return the geographic longitude coordinate.
			\param y projected Y-coordinate. Will retunr the geographic latitude coordinate.
      \param SRID the id of the projected SRS that y and y refers to.
			\return true if succeed and false otherwise.
		*/
		bool convertToGeographic(double &x, double &y, int SRID) const;

    //! Converts a coordinate from a geographic SRS to a projected SRS. 
		/*!
			Converts a coordinate from a geographic SRS to a projected SRS based on 
      the same Datum.

			\param lon geographic longitude. Will return the projected x-coordinate.
			\param lat geogrpahic latitude. Will return the projected y-coordinate.
      \param SRID the id of the target projected SRS.
			\return true if succeed and false otherwise.
		*/
		bool convertToProjected(double &lon, double &lat, int SRID) const;


    private:

			int m_targetSRID;			
			int m_sourceSRID;

			void* m_sourcePj4Handler;	// Proj4 handler to source SRS
			void* m_targetPj4Handler;	// Proj4 handler to target SRS

		};  
	}
} // end TerraLib

#endif // __TERRALIB_SRS_INTERNAL_CONVERTER_H


