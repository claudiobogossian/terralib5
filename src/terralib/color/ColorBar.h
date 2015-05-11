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
  \file terralib/color/ColorBar.h

  \brief The concept of color bar.
*/

#ifndef __TERRALIB_COLOR_INTERNAL_COLORBAR_H
#define __TERRALIB_COLOR_INTERNAL_COLORBAR_H

// TerraLib
#include "RGBAColor.h"

// STL
#include <vector>
#include <map>

namespace te
{
  namespace color
  {
    /*!
      \class ColorBar

      \brief It models the concept of color bar.
      
      It is used to construct color bar.

      \ingroup color
    */
    class TECOLOREXPORT ColorBar
    {
      public:

        /** @name Initializer Methods
         *  Methods related to instantiation and destruction.
         */
        //@{

        /*!
          \brief It initializes a new ColorBar(default constructor)
         */
        ColorBar();

        /*!
          \brief It initializes a new ColorBar.

          \param size Number of colors to be generated.
          \note By default the first color is red and the second is white.
         */
        ColorBar(int size);

        /*!
          \brief It initializes a new ColorBar.

          \param colorBegin Begin color.
          \param colorEnd End color.
          \param size Number of colors to be generated.
         */
        ColorBar(const RGBAColor& colorBegin, const RGBAColor& colorEnd, int size);

         /*! \brief Copy constructor. */
        ColorBar(const ColorBar& rhs);

        /*! Assignment operator.  */
        te::color::ColorBar& operator=(const ColorBar& rhs);
        
        /*! \brief Destructor */
        ~ColorBar();

        //@}

        /** @name Accessor methods
         *  Methods used to get or set properties.
         */
        //@{

        /*!
          \brief It returns the color bar name.

          \return The color bar name.
         */
        const std::string& getName() const;

        /*!
          \brief It sets the color bar name and adjust its entry in the scheme group if needed.

          \param name The new color bar name.
         */
        void setName(const std::string& name);

        /*!
          \brief It gets color map.

          \return Generated color bar.
         */
        const std::map<double, te::color::RGBAColor>& getColorMap() const;

        /*!
          \brief It generates color bar.

          \return Generated color bar.
         */
        const std::vector<te::color::RGBAColor>& getColorBar();
 
        /*!
          \brief It generates color bar.
          \param n Number of slices. It is the amount of legends.

          \return Generated color steps.
         */
        const std::vector<te::color::RGBAColor>& getSlices(const int& n);

        const te::color::RGBAColor& getMeanSlice();

        const std::vector<te::color::RGBAColor>& getLowerMeanSlices(const int& n);

        const std::vector<te::color::RGBAColor>& getUpperMeanSlices(const int& n);

        /*!
          \brief It sets number of colors in the color bar.

          \param size Number of colors in the color bar.
         */
       void setBarSize(const int& size);

        /*!
          \brief It returns number of colors in the color bar.

          \return number of colors in the color bar.
         */
       int getBarSize();

        /*!
          \brief It adds a color in the color bar.

          \param color Color to be added in the color bar.
          \param pos Position where the color is added.
          \note 0 <= pos <= 1
          \note If you add to the first position, the color previously occupied is shifted to be between this and the next color of the bar.
          \note If you add to the last position, the color previously occupied is shifted to be between this and the previous color of the bar.
          \note If you add where there is already another, the color previously occupied is shifted to be between this and the next/previous color of the bar.
         */
       void addColor(const RGBAColor& color, const double& pos);

        /*!
          \brief It removes a color in the color bar.

          \param color Color to be removed in the color bar.
          \note If you remove the first color, the color next to occupy its place.
          \note If you remove the last color, the previous color will take its place.
          \note The bar should have at least two colors.
         */
       void remove(const RGBAColor& color);

        /*!
          \brief It removes a color in the color bar.

          \param index Index of color to be removed in the color bar.
          \note 0:remove the first color; 1:remove the second color...
          \note If you remove the first color, the color next to occupy its place.
          \note If you remove the last color, the previous color will take its place.
          \note The bar should have at least two colors.
         */
       void remove(const int& index);

        /*!
          \brief It removes a color in the color bar.

          \param pos Position of color to be removed in the color bar.
          \note If you remove the first color, the color next to occupy its place.
          \note If you remove the last color, the previous color will take its place.
          \note The bar should have at least two colors.
         */
       void remove(const double& pos);

        /*!
          \brief It removes a color in the color bar.

          \param index Reverse index of color to be removed in the color bar.
          \note 0:remove the last color; 1:remove the penultimate color...
          \note If you remove the first color, the color next to occupy its place.
          \note If you remove the last color, the previous color will take its place.
          \note The bar should have at least two colors.
         */
       void removeReverse(const int& index);

        /*!
          \brief It moves a color to new posisiton.

          \param pos position of the color in the color bar.
          \param color The new color.
         */
       void changeColor(const double& pos, const RGBAColor& color);

        /*!
          \brief It moves a color to new posisiton.

          \param index index of the color in the color bar.
          \param color The new color.
         */
       void changeColor(const int& index, const RGBAColor& color);

        /*!
          \brief It moves a color to new posisiton.

          \param color Color to be moved in the color bar.
          \note Move the first color matching found in the color bar.
          \note If you place the color where there is already another, the color previously occupied is shifted to be between this and the next color of the bar.
         */
       void move(const RGBAColor& color, const double& pos);

        /*!
          \brief It moves a color to new posisiton.

          \param index Index of color to be moved in the color bar.
          \param pos The new postion of color in the color bar.
          \note 0:move the first color; 1:remove the second color...
          \note If you place the color where there is already another, the color previously occupied is shifted to be between this and the next color of the bar.

          \return The index of color. The index may have changed depending on the new position that the color will take on the color bar.
         */
       int move(const int& index, const double& pos);

        /*!
          \brief It moves a color to new posisiton.

          \param index Index of color to be moved in the color bar.
          \param pos The new postion of color in the color bar.
          \note 0:move the first color; 1:remove the second color...
          \note If you place the color where there is already another, the color previously occupied is shifted to be between this and the next color of the bar.

          \return The position of color. The position may have changed depending on the new position that the color will take on the color bar.
         */
       double move(const double& pi, const double& pm);

        /*!
          \brief It moves a color to new posisiton.

          \param index Reverse index of color to be moved in the color bar.
          \param pos The new postion of color in the color bar.
          \note 0:move the last color; 1:remove the penultimate color...
          \note If you place the color where there is already another, the color previously occupied is shifted to be between this and the next color of the bar.

          \return The position of color. The position may have changed depending on the new position that the color will take on the color bar.
         */
       int moveReverse(const int& index, const double& pos);

        //@}

      private:
         /** @name Private methods
         *  Methods used to generate colors.
         */
        //@{
        /*!
          \brief It generates bar colors.
         */
        void generateColorBar();

        /*!
          \brief It generates legend colors.

          \param n Number of slices. It is the amount of legends.
         */
        void generateSlices(const int& n);

        //@}

      private:

        std::vector<RGBAColor> m_colorBarVec;      //!< Color vector.
        std::vector<RGBAColor> m_colorSliceVec;    //!< Color step vector.
        std::string m_name;                        //!< Color bar name.
        int m_barSize;                             //!< Size of color bar (number of colors in the color bar).
        std::map<double, RGBAColor> m_colorMap;    //!< Color map.
    };

  }   // end namespace color
}     // end namespace te

#endif  // __TERRALIB_COLOR_INTERNAL_COLORBAR_H

