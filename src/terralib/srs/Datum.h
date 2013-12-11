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

     \ingroup srs

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
      
      //! Destructor.
      ~Datum();
      
      //! Returns the Datum name.
      const std::string& getName() const;
      
      /*!
       \brief Sets the Datum name.
       \param name The Datum name.
       */
      void setName(const std::string& name);
      
      //! Returns the Ellipsoid associated to the Datum.
      const Ellipsoid* getEllipsoid() const;
      
      /*!
       \brief Sets the Ellipsoid associated to the Datum.
       \param ellps Pointer to the ellipsoid. Do not pass null. Class takes the pointer ownership.
       */
      void setEllipsoid(Ellipsoid* ellps);
      
      /*!
       \brief Sets the Datum shifting parameters relative to WGS84.
       
       This indicates a list of up to 7 Bursa Wolf transformation parameters.
       These parameters can be used to approximate a transformation from a given datum to the WGS84.
       \param params A vector of shifting parameters.
       */
      void setToWGS84Params(const std::vector<double>& params);
      
      //! Returns the WGS84 shifting parameters.
      const std::vector<double>& getToWGS84Params() const;
      
      //! Returns a WKT string that represent the Datum.
      std::string getWKT() const;
      
    private:
      
      std::string m_name;      //!< Datum name.
      Ellipsoid* m_ellipsoid;  //!< Reference ellipsoid.
      std::vector<double> m_towgs84; //!< To store shifting parametes necessary to execute Datum shifting to a WGS84 Datum.
    };
  }
}
#endif //__TERRALIB_SRS_INTERNAL_DATUM_H
