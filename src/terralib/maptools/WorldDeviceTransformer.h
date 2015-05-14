/*  Copyright (C) 2008 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/maptools/WorldDeviceTransformer.h

  \brief This class implements the logic for transforming from device coordinate to world coordinate and vice-versa.
 */

#ifndef __TERRALIB_MAPTOOLS_INTERNAL_WORLDDEVICETRANSFORMER_H
#define __TERRALIB_MAPTOOLS_INTERNAL_WORLDDEVICETRANSFORMER_H

namespace te
{
  namespace map
  {
    /*!
      \class WorldDeviceTransformer

      \brief This class implements the logic for transforming from device coordinate to world coordinate and vice-versa.

      \ingroup map

      \sa Canvas
    */
    class WorldDeviceTransformer
    {
      public:

        /** @name Initializer Methods
         *  Methods related to instantiation and destruction.
         */
        //@{

        /*! \brief Empty constructor */
        WorldDeviceTransformer();

        /*!
          \brief Creates a new transformation functor that maps between the
                 spatial coordinate system of a feature to device (canvas) coordinate system.

          \param wllx Lower left x-coordinate of the World (Spatial Coordinate System of the features).
          \param wlly Lower left y-coordinate of the World (Spatial Coordinate System of the features).
          \param wurx Upper right x-coordinate of the World (Spatial Coordinate System of the features).
          \param wury Upper right y-coordinate of the World (Spatial Coordinate System of the features).
          \param deviceWidth  Device (canvas) width in pixels.
          \param deviceHeight Device (canvas) height in pixels.
        */
        WorldDeviceTransformer(const double& wllx, const double& wlly,
                               const double& wurx, const double& wury,
                               int deviceWidth, int deviceHeight);

        /*! \brief Destructor */
        ~WorldDeviceTransformer();

        //@}

        /** @name Transformation Methods
         *  Methods used to transform coordinates from world (feature coordinate system) to device (canvas) and vice-versa.
         */
        //@{

        /*!
          \brief It adjusts a new transformation function that maps between the spatial coordinate system of a feature to
                 device (canvas) coordinate system.

          \param wllx Lower left x-coordinate of the World (Spatial Coordinate System of the features).
          \param wlly Lower left y-coordinate of the World (Spatial Coordinate System of the features).
          \param wurx Upper right x-coordinate of the World (Spatial Coordinate System of the features).
          \param wury Upper right y-coordinate of the World (Spatial Coordinate System of the features).
          \param deviceWidth  Device (canvas) width in pixels.
          \param deviceHeight Device (canvas) height in pixels.
        */
        void setTransformationParameters(const double& wllx, const double& wlly,
                                         const double& wurx, const double& wury,
                                         int deviceWidth, int deviceHeight);

        /*!
          \brief It transforms the coordinate wx and wy from world coordinates to
                 device (canvas) coordinates without using auxiliary variables. It
                 is supposed to be faster than the other version that has 4 parameters.

          \param wx X value in world coordinates.
          \param wy Y value in world coordinates.
        */
        void world2Device(double& wx, double& wy) const;

        /*!
          \brief It transforms the line coordinates from world coordinates to device (canvas) coordinates.

          \param line    Must be a buffer of double, with npoints.
          \param npoints The number of points in the line.
          \param pts     The device coordinate buffer.

          \note We must have buffer aligned as: buffer[0] -> x0, buffer[1] -> y0, buffer[2] -> x1, buffer[3] -> y1 and so on.
         */
        void world2Device(double* line, unsigned int npoints, double* pts);

        /*!
          \brief It transforms the coordinate wx and wy from world coordinates to device (canvas) coordinates (dx and dy).

          \param wx X value in world coordinates.
          \param wy Y value in world coordinates.
          \param dx X value in device coordinates.
          \param dy Y value in device coordinates.
        */
        void world2Device(const double& wx, const double& wy, double& dx, double& dy) const;

        /*!
          \brief It transforms the coordinate dx and dy from device coordinates to
                 world coordinates without using auxiliary variables. It
                 is supposed to be faster than the other version that has 4 parameters.

          \param dx X value in device coordinates.
          \param dy Y value in device coordinates.
         */
        void device2World(double& dx, double& dy) const;

        /*!
          \brief It transforms the coordinate dx and dy from device (canvas) coordinates to world coordinates (wx and wy).

          \param dx X value in device coordinates.
          \param dy Y value in device coordinates.
          \param wx X value in world coordinates.
          \param wy Y value in world coordinates.
        */
        void device2World(int dx, int dy, double& wx, double& wy) const;

        //@}

      public:

        double m_mapUnitsPP;   //!< Map units per-pixel.
        double m_wllx;         //!< Lower left x-coordinate of the World (Spatial Coordinate System of the features).
        double m_wlly;         //!< Lower left y-coordinate of the World (Spatial Coordinate System of the features).
        double m_wurx;         //!< Upper right x-coordinate of the World (Spatial Coordinate System of the features).
        double m_wury;         //!< Upper right y-coordinate of the World (Spatial Coordinate System of the features).
    };

    inline WorldDeviceTransformer::WorldDeviceTransformer()
      : m_mapUnitsPP(0.0),
        m_wllx(0.0),
        m_wlly(0.0),
        m_wurx(0.0),
        m_wury(0.0)
    {
    }

    inline WorldDeviceTransformer::WorldDeviceTransformer(const double& wllx, const double& wlly,
                                                          const double& wurx, const double& wury,
                                                          int deviceWidth, int deviceHeight)
    {
      setTransformationParameters(wllx, wlly, wurx, wury, deviceWidth, deviceHeight);
    }

    inline WorldDeviceTransformer::~WorldDeviceTransformer()
    {
    }

    inline void WorldDeviceTransformer::setTransformationParameters(const double& wllx, const double& wlly,
                                                                    const double& wurx, const double& wury,
                                                                    int deviceWidth, int deviceHeight)
    {
      double worldWidth = wurx - wllx;
      double worldHeight = wury - wlly;

      double muppX = worldWidth / static_cast<double>(deviceWidth);   // map unit per pixel along x-direction
      double muppY = worldHeight / static_cast<double>(deviceHeight); // map unit per pixel along y-direction
      
      if(muppY > muppX)
      {
        m_mapUnitsPP = muppY;
        m_wlly = wlly;
        m_wury = wury;
        double whitespace = ((static_cast<double>(deviceWidth) * m_mapUnitsPP) - worldWidth) * 0.5;
        m_wllx = wllx - whitespace;
        m_wurx = wurx + whitespace;
      }
      else
      {
        m_mapUnitsPP = muppX;
        m_wllx = wllx;
        m_wurx = wurx;
        double whitespace = ((static_cast<double>(deviceHeight) * m_mapUnitsPP) - worldHeight) * 0.5;
        m_wlly = wlly - whitespace;
        m_wury = wury + whitespace;
      }
    }


    inline void WorldDeviceTransformer::world2Device(double& wx, double& wy) const
    {
      wx = (wx - m_wllx) / m_mapUnitsPP;
      wy = (m_wury - wy) / m_mapUnitsPP;
    }

    inline void WorldDeviceTransformer::world2Device(double* line, unsigned int npoints, double* pts)
    {
      const unsigned int nstep = 2 * npoints;

      for(register unsigned int i = 0; i != nstep; i+=2)
      {
        pts[i] = (line[i] - m_wllx) / m_mapUnitsPP;
        pts[i + 1] = (m_wury - line[i + 1]) / m_mapUnitsPP;
      }
    }

    inline void WorldDeviceTransformer::world2Device(const double& wx, const double& wy,
                                                     double& dx, double& dy) const
    {
      dx = (wx - m_wllx) / m_mapUnitsPP;
      dy = (m_wury - wy) / m_mapUnitsPP;
    }

    inline void WorldDeviceTransformer::device2World(double& dx, double& dy) const
    {
      dx = dx * m_mapUnitsPP + m_wllx;
      dy = m_wury - dy * m_mapUnitsPP;
    }

    inline void WorldDeviceTransformer::device2World(int dx, int dy,
                                                     double& wx, double& wy) const
    {
      wx = ((double)(dx)) * m_mapUnitsPP + m_wllx;
      wy = m_wury - ((double)(dy)) * m_mapUnitsPP;
    }

  }   // end namespace map
}     // end namespace te

#endif  // __TERRALIB_MAPTOOLS_INTERNAL_WORLDDEVICETRANSFORMER_H
