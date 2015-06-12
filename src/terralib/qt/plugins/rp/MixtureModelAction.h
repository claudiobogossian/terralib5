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
  \file terralib/qt/plugins/rp/MixtureModelAction.h

  \brief This file defines the mixture model class
*/

#ifndef __TE_QT_PLUGINS_RP_INTERNAL_MIXTUREMODELACTION_H
#define __TE_QT_PLUGINS_RP_INTERNAL_MIXTUREMODELACTION_H

// TerraLib
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
          \class MixtureModelAction

          \brief This class register the mixture model action into RP Plugin.

        */
        class MixtureModelAction : public te::qt::plugins::rp::AbstractAction
        {
          Q_OBJECT

          public:

            MixtureModelAction(QMenu* menu, QMenu* popupMenu);

            virtual ~MixtureModelAction();

          protected slots:

            virtual void onActionActivated(bool checked);

            virtual void onPopUpActionActivated(bool checked);
        };

      } // end namespace rp
    }   // end namespace plugins
  }     // end namespace qt
}       // end namespace te

#endif //__TE_QT_PLUGINS_RP_INTERNAL_MIXTUREMODELACTION_H