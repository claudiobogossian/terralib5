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
  \file terralib/qt/widgets/layer/selector/core/HiddenDataSetLayerSelector.h

  \brief ....
*/

#ifndef __TERRALIB_QT_WIDGETS_LAYER_SELECTOR_CORE_INTERNAL_HIDDENDATASETLAYERSELECTOR_H
#define __TERRALIB_QT_WIDGETS_LAYER_SELECTOR_CORE_INTERNAL_HIDDENDATASETLAYERSELECTOR_H

// TerraLib
#include "AbstractLayerSelector.h"

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      /*!
        \class HiddenDataSetLayerSelector

        \brief ....
      */
      class TEQTWIDGETSEXPORT HiddenDataSetLayerSelector : public AbstractLayerSelector
      {
        //Q_OBJECT

        public:

          HiddenDataSetLayerSelector(QWidget* parent = 0, Qt::WindowFlags f = 0);

          ~HiddenDataSetLayerSelector();

          void set(const std::list<te::da::DataSourceInfoPtr>& datasources);

          std::list<te::map::AbstractLayerPtr> getLayers();

        private:

          std::list<te::da::DataSourceInfoPtr> m_datasources;
      }; 

    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_LAYER_SELECTOR_CORE_INTERNAL_HIDDENDATASETLAYERSELECTOR_H

