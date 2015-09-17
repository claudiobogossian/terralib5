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
  \file terralib/qt/plugins/mnt/TINGenerationAction.h

  \brief This file defines the TIN Generation class
*/

#ifndef __TE_QT_PLUGINS_MNT_INTERNAL_TINGENERATIONACTION_H
#define __TE_QT_PLUGINS_MNT_INTERNAL_TINGENERATIONACTION_H

// TerraLib
#include "Config.h"
#include "AbstractAction.h"

namespace te
{
  namespace qt
  {
    namespace plugins
    {
      namespace mnt
      {
        /*!
          \class TINGenerationAction

          \brief This class register the contrast action into MNT Plugin.

        */
        class TINGenerationAction : public te::qt::plugins::mnt::AbstractAction
        {
          Q_OBJECT

          public:

            TINGenerationAction(QMenu* menu);

            virtual ~TINGenerationAction();

          protected slots:

            virtual void onActionActivated(bool checked);
        };

      } // end namespace mnt
    }   // end namespace plugins
  }     // end namespace qt
}       // end namespace te

#endif //__TE_QT_PLUGINS_MNT_INTERNAL_TINGENERATIONACTION_H