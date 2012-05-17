/*! 
  \file Datum.h

  \brief This file contains the structs necessary to represent a geodetic datum.
*/

#ifndef __TERRALIB_SRS_INTERNAL_DATUM_H 
#define __TERRALIB_SRS_INTERNAL_DATUM_H

#include <string>
#include <vector>

#include "Config.h"
#include "Ellipsoid.h"

namespace te
{
	namespace srs
	{   
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
      
      //! Default constructor
      Datum();

      /*!
        \brief Initializes a new Datum from parameters.

        \param name Datum name.
        \param ellps The Ellipsoid associated to the Datum.
       */
      Datum(const std::string& name, const Ellipsoid& ellps);

      /*!
        \brief Returns the Datum name.
        \return The Datum name.
       */
      const std::string& getName() const { return m_name; }

      /*!
        \brief Sets the Datum name.        
        \param name The Datum name.
       */
      void setName(const std::string& name) { m_name = name; }

      /*!
        \brief Returns the Ellipsoid associated to the Datum.
        \return The Ellipsoid associated to the Datum.
       */
      const Ellipsoid& getEllipsoid() const { return m_ellipsoid; }

      /*!
        \brief Sets the Ellipsoid associated to the Datum.
        \param ellps The Ellipsoid that will be associated to the Datum.
       */
      void setEllipsoid(const Ellipsoid& ellps) { m_ellipsoid = ellps; }

     /*!
        \brief Sets the Datum Shift parameters relative to WGS84.
        This indicates a list of up to 7 Bursa Wolf transformation parameters. 
        These parameters can be used to approximate a transformation from the  
        datum to the WGS84 datum.
        \param params The list of transformation parameters.
     */
      void setToWGS84Params(const std::vector<double>& params);

     /*!
        \brief Gets the datum Shift parameters relative to WGS84.
     */
      const std::vector<double>& getToWGS84Params() const {return m_towgs84; }

      //! Returns a WKT string that represent the Datum.
      std::string getWKT() const;

    private:

      std::string m_name;     //!< Datum name.
      Ellipsoid m_ellipsoid;  //!< Reference ellipsoid.
      std::vector<double> m_towgs84; //!< To store shifting parametes necessary to execute Datum shifting to a WGS84 Datum.
    };
  }
}
#endif //__TERRALIB_SRS_INTERNAL_DATUM_H
