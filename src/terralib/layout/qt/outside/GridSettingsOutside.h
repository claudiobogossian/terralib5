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
  \file GridSettingsOutside.h
   
  \brief Window (QDialog) map grid setting (MapItem).

  \ingroup layout
*/

#ifndef __TERRALIB_LAYOUT_INTERNAL_GRID_SETTINGS_OUTSIDE_H
#define __TERRALIB_LAYOUT_INTERNAL_GRID_SETTINGS_OUTSIDE_H

// Qt
#include <QDialog>

// TerraLib
#include "../../core/pattern/mvc/OutsideObserver.h"
#include "../../core/property/GridSettingsConfigProperties.h"
#include "../../core/Config.h"
#include "../../../geometry/Envelope.h"
#include "../../../color/RGBAColor.h"
#include "../../core/property/Property.h"

namespace Ui { class GridSettings; }

namespace te
{
  namespace layout
  {
    class OutsideController;
    class Observable;
    class PlanarGridSettingsConfigProperties;
    class GeodesicGridSettingsConfigProperties;

    /*!
    \brief Window (QDialog) map grid setting (MapItem).
	  
	    \ingroup layout

	    \sa te::layout::OutsideObserver
	  */
    class TELAYOUTEXPORT GridSettingsOutside : public QDialog, public OutsideObserver 
    {
	    Q_OBJECT
    
      public:
        
        GridSettingsOutside(OutsideController* controller, Observable* o);

		    virtual ~GridSettingsOutside();

        virtual void updateObserver(ContextItem context);

        virtual void setPosition(const double& x, const double& y);

        virtual te::gm::Coord2D getPosition();

        virtual void blockComponents();

        virtual void unblockComponents();
       
        virtual void load();

      private slots:

        /* Grid */
        virtual void on_pbClose_clicked();
        
        virtual void on_helpPushButton_clicked();

        virtual void on_cmbUnit_currentIndexChanged ( const QString & text );

        virtual void on_chkShowPlanar_clicked();

        virtual void on_cmbPlanarStyle_currentIndexChanged ( const QString & text );

        virtual void on_cmbGridStyle_currentIndexChanged ( const QString & text );

        virtual void on_chkShowGeodesic_clicked();

        ///* Line */

        virtual void on_lneHrzPlanarGap_editingFinished();
        
        virtual void on_lneVrtPlanarGap_editingFinished();  

        virtual void on_lneHorizontalGap_editingFinished();

        virtual void on_lneVerticalGap_editingFinished();
        
        virtual void on_pbPlanarLineColor_clicked();

        virtual void on_cmbPlanarLineType_currentIndexChanged ( const QString & text );

        virtual void on_cmbPlanarLineWidth_currentIndexChanged ( const QString & text );  

        virtual void on_cmbLineType_currentIndexChanged ( const QString & text );

        virtual void on_cmbLineWidth_currentIndexChanged ( const QString & text );

        virtual void on_pbLineColor_clicked();

        ///*Text: Basic Configuration*/

        virtual void on_cmbPlanarTextSize_currentIndexChanged ( const QString & text );

        virtual void on_cmbPlanarFont_currentIndexChanged ( const QString & text );

        virtual void on_pbGridTextPlanarColor_clicked();
        
        virtual void on_chkSuperscriptPlanarText_clicked();

        virtual void on_cmbGeoFont_currentIndexChanged ( const QString & text );

        virtual void on_cmbGeoTextSize_currentIndexChanged ( const QString & text );

        virtual void on_pbGridTextGeoColor_clicked();
        
        virtual void on_chkSuperscriptGeoText_clicked();

        ///*Text: Advanced configuration*/

        virtual void on_xGridInitialPoint_planar_textField_editingFinished();

        virtual void on_yGridInitialPoint_planar_textField_editingFinished();

        virtual void on_xGridInitialPoint_geo_textField_editingFinished();

        virtual void on_yGridInitialPoint_geo_textField_editingFinished();

        virtual void on_chkBottomPlanarText_clicked();

        virtual void on_chkLeftPlanarText_clicked();

        virtual void on_chkRightPlanarText_clicked();

        virtual void on_chkTopPlanarText_clicked();

        virtual void on_chkBottomGeoText_clicked();

        virtual void on_chkLeftGeoText_clicked();

        virtual void on_chkRightGeoText_clicked();

        virtual void on_chkTopGeoText_clicked();

        virtual void on_chkBottomRotatePlanarText_clicked();

        virtual void on_chkLeftRotatePlanarText_clicked();

        virtual void on_chkRightRotatePlanarText_clicked();

        virtual void on_chkTopRotatePlanarText_clicked();

        virtual void on_chkBottomRotateGeoText_clicked();

        virtual void on_chkLeftRotateGeoText_clicked();

        virtual void on_chkRightRotateGeoText_clicked();

        virtual void on_chkTopRotateGeoText_clicked();

        virtual void on_lneVrtPlanarDisplacement_editingFinished();

        virtual void on_chkVisibleTextsPlanarText_clicked();

        virtual void on_lneHrzPlanarDisplacement_editingFinished();

        virtual void on_lneVrtGeoDisplacement_editingFinished();

        virtual void on_lneHrzGeoDisplacement_editingFinished();

        virtual void on_chkDegreesGeoText_clicked();

        virtual void on_chkMinutesGeoText_clicked();

        virtual void on_chkSecondsGeoText_clicked();

        virtual void on_chkVisibleTextsGeoText_clicked();

        virtual void on_cmbCornerGeoFont_currentIndexChanged ( const QString & text );

        virtual void on_cmbCornerGeoTextSize_currentIndexChanged ( const QString & text );

        virtual void on_pbCornerTextGeoColor_clicked();
        
        virtual void on_lneCornerHrzGeoDisplacement_editingFinished();

        virtual void on_lneCornerVrtGeoDisplacement_editingFinished();

        virtual void on_chkLowerRightCornerGeoText_clicked();

        virtual void on_chkUpperRightCornerGeoText_clicked();

        virtual void on_chkLowerLeftCornerGeoText_clicked();

        virtual void on_chkUpperLeftCornerGeoText_clicked();

        virtual void on_chkVisibleCornerTextsGeoText_clicked();

        /*Geodesic: Topographic Map*/

        virtual void on_ckDefineScale_clicked();
        
        virtual void on_cmbScale_currentIndexChanged ( const QString & text );

        virtual void on_ckbClip_clicked();

        virtual void on_lneX1_editingFinished();

        virtual void on_lneX2_editingFinished();

        virtual void on_lneY1_editingFinished();

        virtual void on_lneY2_editingFinished();

        virtual void on_lneX3_editingFinished();

        virtual void on_lneX4_editingFinished();

        virtual void on_lneY3_editingFinished();

        virtual void on_lneY4_editingFinished();

      signals:

        void updateProperty(Property prop);

      protected:

        virtual bool checkValidDegreeValue(const QString &);

        virtual te::color::RGBAColor configColor(QWidget* widget);

        virtual void init();

        virtual void initString( QWidget* widget, std::string nameComponent, EnumType* gridType );

        virtual void initInt( QWidget* widget, std::string nameComponent, EnumType* gridType );

        virtual void initDouble( QWidget* widget, std::string nameComponent, EnumType* gridType );

        virtual void initBool( QWidget* widget, std::string nameComponent, EnumType* gridType );

        virtual void initColor( QWidget* widget, std::string nameComponent, EnumType* gridType );

        virtual void initCombo(QWidget* widget, std::string nameComponent, EnumType* gridType);
                
      protected:

        PlanarGridSettingsConfigProperties* m_planarGridSettings;
        GeodesicGridSettingsConfigProperties* m_geodesicGridSettings;
        EnumType* m_planarType;
        EnumType* m_geodesicType;

      private:

        std::auto_ptr<Ui::GridSettings> m_ui;
    };
  }
}

#endif



