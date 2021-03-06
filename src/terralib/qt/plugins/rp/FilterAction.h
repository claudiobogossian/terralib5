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
  \file terralib/qt/plugins/rp/FilterAction.h

  \brief This file defines the Filter class
*/

#ifndef __TE_QT_PLUGINS_RP_INTERNAL_FILTERACTION_H
#define __TE_QT_PLUGINS_RP_INTERNAL_FILTERACTION_H

// TerraLib
#include "../../../qt/widgets/rp/FilterDialogForm.h"
#include "Config.h"
#include "AbstractAction.h"

namespace te
{
  namespace qt
  {
    namespace plugins
    {
      namespace rp
      {
        /*!
          \class FilterAction

          \brief This class register the filter action into RP Plugin.

        */
        class FilterAction : public te::qt::plugins::rp::AbstractAction
        {
          Q_OBJECT

          public:

            FilterAction(QMenu* menu, QMenu* popupMenu);

            virtual ~FilterAction();

          protected slots:

            virtual void onActionActivated(bool checked);

            void addLayer(te::map::AbstractLayerPtr outputLayer);

            void closeTool();

          private:
            te::qt::widgets::FilterDialogForm* m_filterDlg;
        };

      } // end namespace rp
    }   // end namespace plugins
  }     // end namespace qt
}       // end namespace te

#endif //__TE_QT_PLUGINS_RP_INTERNAL_FILTERACTION_H
