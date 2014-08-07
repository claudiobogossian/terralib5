/*  Copyright (C) 2011-2012 National Institute For Space Research (INPE) - Brazil.
  
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
  \file terralib/qt/plugins/sa/SpatialStatisticsAction.h

  \brief This file defines the concrete class for SpatialStatistics Action
*/

#ifndef TE_QT_PLUGINS_SA_SPATIALSTATISTICSACTION_H
#define TE_QT_PLUGINS_SA_SPATIALSTATISTICSACTION_H

#include "AbstractAction.h"

namespace te
{
  namespace qt
  {
    namespace plugins
    {
      namespace sa
      {
        
        class SpatialStatisticsAction : public te::qt::plugins::sa::AbstractAction
        {
            Q_OBJECT

          public:

            SpatialStatisticsAction(QMenu* menu);

            virtual ~SpatialStatisticsAction();

          protected slots:

            virtual void onActionActivated(bool checked);
        };
        
      } // namespace sa
    } // namespace plugins
  } // namespace qt
} // namespace te

#endif // TE_QT_PLUGINS_SA_SPATIALSTATISTICSACTION_H