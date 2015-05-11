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
  \file PageSetupOutside.h
   
  \brief 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_SYSTEMATIC_SCALE_OUTSIDE_H
#define __TERRALIB_LAYOUT_INTERNAL_SYSTEMATIC_SCALE_OUTSIDE_H

// Qt
#include <QDialog>

// TerraLib
#include "../../core/pattern/mvc/OutsideObserver.h"
#include "../../core/Config.h"
#include "../../../geometry/Envelope.h"
#include "../../../color/RGBAColor.h"
#include "../../core/enum/AbstractType.h"

namespace Ui { class SystematicScale; }

namespace te
{
  namespace layout
  {
    class OutsideController;
    class Observable;

    class TELAYOUTEXPORT SystematicScaleOutside : public QDialog, public OutsideObserver 
    {
	    Q_OBJECT
    
      public:
        
        SystematicScaleOutside(OutsideController* controller, Observable* o);

		    virtual ~SystematicScaleOutside();

        virtual void updateObserver(ContextItem context);

        virtual void setPosition(const double& x, const double& y);

        virtual te::gm::Coord2D getPosition();

      signals:

        void systematicApply(double scale, SystematicScaleType type);
        
      protected slots:
                    
        /* Grid */
        virtual void on_btnOK_clicked();

        virtual void on_btnCancel_clicked();

      protected:

        virtual SystematicScaleType getType(double scale);

      private:

        std::auto_ptr<Ui::SystematicScale> m_ui;
    };
  }
}

#endif
