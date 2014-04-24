/*  Copyright (C) 2001-2014 National Institute For Space Research (INPE) - Brazil.

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
  \file GridSettingsOutside.h
   
  \brief 

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_GRID_SETTINGS_OUTSIDE_H
#define __TERRALIB_LAYOUT_INTERNAL_GRID_SETTINGS_OUTSIDE_H

// Qt
#include <QtGui/QDialog>

// TerraLib
#include "OutsideObserver.h"
#include "../../../include/core/Config.h"
#include "../../../../geometry/Envelope.h"

namespace Ui { class GridSettings; }

namespace te
{
  namespace layout
  {
    class OutsideController;
    class Observable;

    class TELAYOUTEXPORT GridSettingsOutside : public QDialog, public OutsideObserver 
    {
	    Q_OBJECT
    
      public:
        
        GridSettingsOutside(OutsideController* controller, Observable* o);

		    virtual ~GridSettingsOutside();

        virtual void updateObserver(ContextItem context);

        virtual void setPosition(const double& x, const double& y);

        virtual te::gm::Coord2D getPosition();

      private slots:

        virtual void changeLineColor();

        virtual void on_pbPlanarLineColor_clicked();
        
        virtual void on_pbClose_clicked();

        virtual void on_pbCancel_clicked();

        virtual void on_pbApply_clicked();

        virtual void on_helpPushButton_clicked();

        virtual void on_ckDefineScale_clicked();

        virtual void on_cmdGridTextPlanarColor_clicked();

        virtual void on_cmdGridTextGeoColor_clicked();

        virtual void on_cmdCornerTextGeoColor_clicked();
        
        virtual void on_cmbLineType_editTextChange(QString value);

        virtual void on_xGridInitialPoint_geo_textField_textChanged(const QString& text);

      signals:

        void updateProperty();

      protected:

        virtual bool checkValidDegreeValue(const QString &);

        virtual void configColor(QWidget* widget);

        virtual void init();

      private:

        std::auto_ptr<Ui::GridSettings> m_ui;
    };
  }
}

#endif
