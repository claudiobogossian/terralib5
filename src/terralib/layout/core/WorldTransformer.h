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
  \file terralib/layout/WorldTransformer.h

  \brief This class implements the logic for transforming from System 1 coordinate to other type of coordinate system and vice-versa.
 */

#ifndef __TERRALIB_LAYOUT_INTERNAL_WORLDTRANSFORMER_H
#define __TERRALIB_LAYOUT_INTERNAL_WORLDTRANSFORMER_H

// TerraLib
#include "../../geometry/Envelope.h"

namespace te
{
  namespace layout
  {
    /*!
      \class WorldTransformer

      \brief This class implements the logic for transforming from System 1 coordinate to other type of coordinate system and vice-versa.

      \ingroup layout

    */
    class WorldTransformer
    {
      public:

        /** @name Initializer Methods
         *  Methods related to instantiation and destruction.
         */
        //@{

        /*! \brief Empty constructor */
        WorldTransformer();

        /*!
          \brief The world coordinates system will fit with 0,0 of another coordinate system (passed as width and height).
                 This method is indicated to create a World Transformer between a coordinate system and viewport (device, logical).

          \param s1llx Lower left x-coordinate of the World (Spatial Coordinate System of the features).
          \param s1lly Lower left y-coordinate of the World (Spatial Coordinate System of the features).
          \param wurx Upper right x-coordinate of the World (Spatial Coordinate System of the features).
          \param s1ury Upper right y-coordinate of the World (Spatial Coordinate System of the features).
          \param s2width  *Can be device (canvas) width in pixels.
          \param s2height  *Can be device (canvas) height in pixels.
        */
        WorldTransformer(const double& s1llx, const double& s1lly,
                               const double& s1urx, const double& s1ury,
                               double s2width, double s2height);

        /*!
          \brief The system 1 coordinates will fit with another coordinate system.
                 This method is indicated to create a World Transformer between a coordinate system and viewport (device, logical).

          \param system1Box Box in System 1 Coordinate System.
          \param system2Box Box in System 2 Coordinate System.
        */
        WorldTransformer(te::gm::Envelope system1Box, te::gm::Envelope system2Box);

        /*! \brief Destructor */
        ~WorldTransformer();

        //@}

        /** @name Transformation Methods
         *  Methods used to transform coordinates from system 1 coordinate system to system 2 coordinate and vice-versa.
         */
        //@{

        /*!
          \brief It adjusts a new transformation function that maps between the world coordinate system to
                 other coordinate system.

          \param system1Box Box in System 1 Coordinate System.
          \param system2Box Box in System 2 Coordinate System.
        */
        void setTransformationParameters(te::gm::Envelope system1Box, te::gm::Envelope system2Box);

        /*!
          \brief It transforms the coordinate wx and wy from world coordinates to
                 other(system) coordinates without using auxiliary variables. 

          \param wx X value in system 1 coordinates.
          \param wy Y value in system 1 coordinates.
        */
        void system1Tosystem2(double& wx, double& wy) const;

        /*!
          \brief It transforms the coordinate wx and wy from world coordinates to other(system) coordinates (dx and dy).

          \param wx X value in system 1 coordinates.
          \param wy Y value in system 1 coordinates.
          \param dx X value in system 2 coordinates.
          \param dy Y value in system 2 coordinates.
        */
        void system1Tosystem2(const double& wx, const double& wy, double& dx, double& dy) const;

        /*!
          \brief It transforms the coordinate dx and dy from system 2 coordinates to
                 syste 1 coordinates without using auxiliary variables. It
                 is supposed to be faster than the other version that has 4 parameters.

          \param dx X value in system 2 coordinates.
          \param dy Y value in system 2 coordinates.
         */
        void system2Tosystem1(double& dx, double& dy) const;

        /*!
          \brief It transforms the coordinate dx and dy from system 2 coordinates to system 1 coordinates (wx and wy).

          \param dx X value in system 2 coordinates.
          \param dy Y value in system 2 coordinates.
          \param wx X value in system 1 coordinates.
          \param wy Y value in system 1 coordinates.
        */
        void system2Tosystem1(double dx, double dy, double& wx, double& wy) const;

        double getScaleX();
        double getScaleY();

        double getTranslateX();
        double getTranslateY();
        
        double getS1llx();
        double getS1lly();
        double getS1urx();
        double getS1ury();

        double getS1Width();
        double getS1Height();

        double getS2llx();
        double getS2lly();
        double getS2urx();
        double getS2ury();

        double getS2Width();
        double getS2Height();

        bool isValid();

        bool isMirroring();

        void setMirroring(bool mirror);

      protected:

        void initVariables(te::gm::Envelope system1Box, te::gm::Envelope system2Box);

        //@}

      protected:

        double m_scaleX;        //!< Map units per unit along x-direction between Coordinate Systems.
        double m_scaleY;        //!< Map units per unit along y-direction between Coordinate Systems.

        double m_translateX;    //!< The value corresponds to the X-Axis translation of system 2.
        double m_translateY;    //!< The value corresponds to the Y-Axis translation of system 2.
        
        double m_s1llx;         //!< Lower left x-coordinate of the System 1 
        double m_s1lly;         //!< Lower left y-coordinate of the System 1 
        double m_s1urx;         //!< Upper right x-coordinate of the System 1
        double m_s1ury;         //!< Upper right y-coordinate of the System 1 
        double m_s1Width;       //!< Width of System 1 coordinate system.
        double m_s1Height;      //!< Height of System 1 coordinate system.
        
        double m_s2llx;         //!< Lower left x-coordinate of the System 2 
        double m_s2lly;         //!< Lower left y-coordinate of the System 2 
        double m_s2urx;         //!< Upper right x-coordinate of the System 2 
        double m_s2ury;         //!< Upper right y-coordinate of the System 2 
        double m_s2Width;       //!< Width of System 2 coordinate system.
        double m_s2Height;      //!< Height of System 2 coordinate system.

        bool m_valid;

        bool m_mirroring;
    };

    inline WorldTransformer::WorldTransformer() :
      m_scaleX(0.),
      m_scaleY(0.),
      m_translateX(0),
      m_translateY(0),
      m_s1llx(0.),
      m_s1lly(0.),
      m_s1urx(0.),
      m_s1ury(0.),
      m_s1Width(0.),
      m_s1Height(0.),
      m_s2llx(0.),
      m_s2lly(0.),
      m_s2urx(0.),
      m_s2ury(0.),
      m_s2Width(0.),
      m_s2Height(0.),
      m_valid(false),
      m_mirroring(true)
    {
    }

    inline WorldTransformer::WorldTransformer(const double& s1llx, const double& s1lly,
                                              const double& s1urx, const double& s1ury,
                                                       double s2width, double s2height)
    {
      te::gm::Envelope system1Box(s1llx, s1lly, s1urx, s1ury);
      te::gm::Envelope system2Box(0, 0, s2width, s2height);

      setTransformationParameters(system1Box, system2Box);
    }

    inline WorldTransformer::WorldTransformer( te::gm::Envelope system1Box, te::gm::Envelope system2Box )
    {
      setTransformationParameters(system1Box, system2Box);
    }

    inline WorldTransformer::~WorldTransformer()
    {
    }
    
    inline void WorldTransformer::setTransformationParameters( 
                te::gm::Envelope system1Box, te::gm::Envelope system2Box )
    {
      m_valid = true;
      if((!system1Box.isValid()) || (!system2Box.isValid()))
      {
        m_valid = false;
        return;
      }

      initVariables(system1Box, system2Box);

      m_scaleX = m_s2Width / m_s1Width;   // map units per unit along x-direction
      m_scaleY = m_s2Height / m_s1Height; // map units per unit along y-direction

      m_translateX = m_s2llx - m_s1llx * m_scaleX;
      m_translateY = m_s2lly - m_s1lly * m_scaleY;
    }

    inline void WorldTransformer::system1Tosystem2(double& wx, double& wy) const
    {
      wx = (m_scaleX * wx) + m_translateX;
      wy = (m_scaleY * wy) + m_translateY;

      if(m_mirroring)
      {
        double dyCopy = wy;
        wy = m_s2Height - 1 - dyCopy; // mirror
      }
    }

    inline void WorldTransformer::system1Tosystem2(const double& wx, const double& wy,
                                                     double& dx, double& dy) const
    {
      dx = (m_scaleX * wx) + m_translateX;
      dy = (m_scaleY * wy) + m_translateY;
                                    
      if(m_mirroring)
      {
        double dyCopy = dy;
        dy = m_s2Height - 1 - dyCopy; // mirror
      }
    }

    inline void WorldTransformer::system2Tosystem1(double& dx, double& dy) const
    {
      double dyCopy = dy;

      if(m_mirroring)
      {
        dy = m_s2Height - 1 - dyCopy; // mirror
      }

      dx = (dx - m_translateX) / m_scaleX;
      dy = (dyCopy - m_translateY) / m_scaleY;
    }

    inline void WorldTransformer::system2Tosystem1(double dx, double dy,
                                                     double& wx, double& wy) const
    {
      double dyCopy = dy;

      if(m_mirroring)
      {
        dy = m_s2Height - 1 - dyCopy; // mirror
      }

      wx = (dx - m_translateX) / m_scaleX;
      wy = (dyCopy - m_translateY) / m_scaleY;
    }

    inline void WorldTransformer::initVariables( 
                te::gm::Envelope system1Box, te::gm::Envelope system2Box )
    {
      m_scaleX = 0.;
      m_scaleY = 0.;

      m_translateX = 0.;
      m_translateY = 0.;
      
      m_s1llx = system1Box.getLowerLeftX();
      m_s1lly = system1Box.getLowerLeftY();
      m_s1urx = system1Box.getUpperRightX();
      m_s1ury = system1Box.getUpperRightY();
      m_s1Width = system1Box.getWidth();
      m_s1Height = system1Box.getHeight();

      m_s2llx = system2Box.getLowerLeftX();
      m_s2lly = system2Box.getLowerLeftY();
      m_s2urx = system2Box.getUpperRightX();
      m_s2ury = system2Box.getUpperRightY();
      m_s2Width = system2Box.getWidth();
      m_s2Height = system2Box.getHeight();
    }

    inline double WorldTransformer::getScaleX()
    {
      return m_scaleX;
    }

    inline double WorldTransformer::getScaleY()
    {
      return m_scaleY;
    }

    inline double WorldTransformer::getTranslateX()
    {
	    return m_translateX;
    }

    inline double WorldTransformer::getTranslateY()
    {
	    return m_translateY;
    }

    inline double WorldTransformer::getS1llx()
    {
      return m_s1llx;
    }

    inline double WorldTransformer::getS1lly()
    {
      return m_s1lly;
    }

    inline double WorldTransformer::getS1urx()
    {
      return m_s1urx;
    }

    inline double WorldTransformer::getS1ury()
    {
      return m_s1ury;
    }

    inline double WorldTransformer::getS1Width()
    {
      return m_s1Width;
    }

    inline double WorldTransformer::getS1Height()
    {
      return m_s1Height;
    }

    inline double WorldTransformer::getS2llx()
    {
      return m_s2llx;
    }

    inline double WorldTransformer::getS2lly()
    {
      return m_s2lly;
    }

    inline double WorldTransformer::getS2urx()
    {
      return m_s2urx;
    }

    inline double WorldTransformer::getS2ury()
    {
      return m_s2ury;
    }

    inline double WorldTransformer::getS2Width()
    {
      return m_s2Width;
    }

    inline double WorldTransformer::getS2Height()
    {
      return m_s2Height;
    }

    inline bool WorldTransformer::isValid()
    {
      if((m_scaleX <= 0.) && (m_scaleY <= 0.))
        return false;

      return true;
    }

    inline bool WorldTransformer::isMirroring()
    {
      return m_mirroring;
    }

    inline void WorldTransformer::setMirroring( bool mirror )
    {
      m_mirroring = mirror;
    }
  }   // end namespace layout
}     // end namespace te

#endif 
