/*! 
  \file Datum.h

  \brief This file contains the support to represent a geodetic datum.
*/

#ifndef __TERRALIB_SRS_INTERNAL_DATUM_H 
#define __TERRALIB_SRS_INTERNAL_DATUM_H

// TerraLib
#include "Config.h"

// STL
#include <string>
#include <vector>

namespace te
{
	namespace srs
	{   
    class Ellipsoid;
    
    /*!
     \class Datum.
     
      \brief A geodetic datum defines a reference to measure Earth's surface.
      Planimetric datum parameters are used for defining geodetic coordinates, 
      which are necessary to establish a map projection system.
      \todo Verify if it is necessary to keep an <Id,Authority> information to this object.
    */
    class TESRSEXPORT Datum
    {
    
    public:

      /*!
        \brief Constructor with parameters.

        \param name Datum name (default empty string)..
        \param ellps Pointer to the Ellipsoid associated to the Datum. Default null. Class takes the pointer ownership.
       */
      Datum(const std::string& name="", Ellipsoid* ellps=0);
      
      /*! \brief Destructor. */
      ~Datum();

      /*!
        \brief Returns the Datum name.
        \return The Datum name.
       */
      const std::string& getName() const;

      /*!
        \brief Sets the Datum name.        
        \param name The Datum name.
       */
      void setName(const std::string& name);

      /*!
        \brief Returns the Ellipsoid associated to the Datum.
        \return Pointer to the ellipsoid. Class maintains the pointer ownership.
       */
      const Ellipsoid* getEllipsoid() const;

      /*!
        \brief Sets the Ellipsoid associated to the Datum.
        \param ellps Pointer to the ellipsoid. Do not pass null. Class takes the pointer ownership.
       */
      void setEllipsoid(Ellipsoid* ellps);

     /*!
        \brief Sets the Datum Shift parameters relative to WGS84.
        This indicates a list of up to 7 Bursa Wolf transformation parameters. 
        These parameters can be used to approximate a transformation from a given  
        datum to the WGS84 datum.
        \param params A vector of shift parameters.
     */
      void setToWGS84Params(const std::vector<double>& params);

     /*!
        \brief Gets the datum Shift parameters relative to WGS84.
      \return Reference to the shift parameters vector.
     */
      const std::vector<double>& getToWGS84Params() const;

      /*! 
       \brief Returns a WKT string that represent the Datum. 
       \return WKT string for the Datum */
      std::string getWKT() const;

    private:

      std::string m_name;      //!< Datum name.
      Ellipsoid* m_ellipsoid;  //!< Reference ellipsoid.
      std::vector<double> m_towgs84; //!< To store shifting parametes necessary to execute Datum shifting to a WGS84 Datum.
    };
  }
}
#endif //__TERRALIB_SRS_INTERNAL_DATUM_H
