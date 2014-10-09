/*  Copyright (C) 2010-2013 National Institute For Space Research (INPE) - Brazil.

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
  \file terralib/qt/widgets/slider/TimeSliderWidget.h

  \brief A widget used to alow the user to control the visualization of temporal data
*/

#ifndef __TERRALIB_QT_WIDGETS_INTERNAL_TIMESLIDERWIDGET_H
#define __TERRALIB_QT_WIDGETS_INTERNAL_TIMESLIDERWIDGET_H

//TerraLib
#include "../Config.h"
#include "../../../geometry.h"
#include "../../../datatype.h"
#include "../InterfaceController.h"
#include "SliderPropertiesDialog.h"
// Qt
#include <QWidget>
#include <QAbstractAnimation>
#include <QDateTime>
#include <QUrl>

//STL
#include <memory>

namespace Ui { class TimeSliderWidgetForm; }

class QGraphicsView;
class QParallelAnimationGroup;
class QGraphicsItem;
class QDateTimeEdit;

namespace te
{
  namespace dt
  {
    class TimePeriod;
    class TimeInstant;
  }
  namespace gm
  {
    class Envelope;
  }
  namespace st
  {
    class TrajectoryDataSet;
    class TrajectoryDataSetLayer;
  }

  namespace qt
  {
    namespace widgets
    {
      class MapDisplay;
      class Animation;
      class AnimationItem;
      class TrajectoryItem;
      class PixmapItem;
      class AnimationScene;

      /*!
        \class TimeSliderWidget

        \brief A wdiget used to control the visualization of temporal data
      */
      class TEQTWIDGETSEXPORT TimeSliderWidget : public QWidget, public InterfaceController
      {
        Q_OBJECT

        friend class SliderPropertiesDialog;
        public:

          /*!
            \brief Constructor

            It constructs a Time Slider Widget
          */
          TimeSliderWidget(MapDisplay* d, QWidget* parent = 0,  Qt::WindowFlags f = 0);

          /*!
            \brief Destructor

            It destructs a Time Slider Widget
          */
          ~TimeSliderWidget();

          /*!
            \brief This method is used to set current layer
            
          */
          virtual void layerSelected(te::map::AbstractLayerPtr layer);

          /*!
            \brief This method is used to add a new layer
            
          */
          virtual void layerAdded(te::map::AbstractLayerPtr layer);

          /*!
            \brief This method is used to remove a layer
            
          */
          virtual void layerRemoved(te::map::AbstractLayerPtr layer);

          void addTemporalImages(const QString& filePath);

          te::qt::widgets::PixmapItem* getMetadata(const QString& path);

          te::qt::widgets::PixmapItem* getGoesMetadata(const QString& path);
          te::dt::TimeInstant getGoesTime(const QString& file);

          te::qt::widgets::PixmapItem* getHidroMetadata(const QString& path);
          te::qt::widgets::PixmapItem* getHidroCtlParameters(const QString& path);
          void setHidroLUT(te::qt::widgets::PixmapItem* pi);
          te::dt::TimeInstant getHidroTime(const QString& file);

          te::qt::widgets::PixmapItem* getEtaMetadata(const QString& path);
          te::qt::widgets::PixmapItem* getEtaCtlParameters(const QString& path);
          void setEtaLUT(te::qt::widgets::PixmapItem* pi);
          te::dt::TimeInstant getEtaTime(const QString& file);

          te::qt::widgets::PixmapItem* getTemporalImageMetadata(const QString& path);
          te::qt::widgets::PixmapItem* getTemporalImageCtlParameters(const QString& path);
          te::dt::TimeInstant getTemporalImageTime(const QString& file);


          /*!
            \brief Add trajectory to animation scene.

            \param title The trajectory title.
            \param pixmapFile The trajectory icon.
            \param poinstFile The trajectory points.
          */
          void addTrajectory(te::st::TrajectoryDataSetLayer* tl, const QString& pixmapFile);

          /*!
            \brief It calculates the spatial extent.
          */
          void calculateSpatialExtent();

          /*!
            \brief It calculates the temporal extent.
          */
          void calculateTemporalExtent();

          /*!
            \brief Change direction.
          */
          void changeDirection();

          /*!
            \brief Sets the mode of animation.
            \param t True if the animation goes back and forth 
          */
          void setGoAndBack(bool t);

          /*!
            \brief Sets the animation direction.
            \param direction The animation direction.
          */
          void setDirection(const QAbstractAnimation::Direction& direction);

          /*!
            \brief Sets the animation duration.
            \param duration The animation duration in miliseconds.
          */
          void setDuration(const unsigned int& duration);

        /*!
          \brief It configures automatic pan over a animation path.
                 It toggles auto pan state.
          \param title The animation title.
        */
          void setAutomaticPan(const QString& title);

          /*!
            \brief create new pixmap.
          */
          void createNewPixmap();

          /*!
            \brief Set transformation matrix and draw current animation.
          */
          void draw();

          /*!
            \brief It emits, indirectly, a sign for this widget is destroyed.
          */
          void deleteMe();

          /*!
            \brief It puts graphic item to front.

            \param item The graphic item.
          */
          void putToFront(AnimationItem* item);

          /*!
            \brief It puts graphic item to back.

            \param item The graphic item.
          */
          void putToBack(AnimationItem* item);

          /*!
            \brief It gets the initial time.

            \return The initial time.
          */
           te::dt::TimeInstant getInitialTime();

          /*!
            \brief It gets the final time.

            \return The final time.
          */
           te::dt::TimeInstant getFinalTime();

        protected:

          /*!
            \brief it filters the events of interest.
          */
          bool eventFilter(QObject*, QEvent*);

          /*!
            \brief Update animation time.
          */
          void updateTimeTextEdit();

          /*!
            \brief Updates the position of the slider with respect to time animation.
          */
          void updateSliderPos();

          /*!
            \brief Play animation.
          */
          void play();

          /*!
            \brief Create animations.
          */
          void createAnimations();

        /*!
          \brief
          It it gets the animation time on a time instant.
          /param tinstant The time instant.

          \return The animation time in miliseconds.
        */
        int getAnimationTime(const te::dt::TimeInstant& tinstant);

        /*!
          \brief
          It it gets the animation time instant.

          \return The animation time instant.
        */
        te::dt::TimeInstant getTimeInstant();

        /*!
          \brief
          It Erase trajectory piece.
        */
        void erase(const unsigned int& curTime);

        /*!
          \brief
          True to erase trajectory piece perfectly.
          Note: Perfectly clear implies loss of performance.
        */
        void setErasePerfectly(bool b);

        /*!
          \brief
          It corrects the edited date time

          /param dte DateTimeEdit to be fixed.
          /param t The edite date time.
        */
        QDateTime fixDateTimeEdit(QDateTimeEdit* dte, const QDateTime& t);

        /*!
          \brief
          Draw the pixmap item.

          /param pi The pixmap item.
          /param dwrect The rect of map display in world coordinates.
          /param painter The painter.
        */
        void drawPixmapItem(PixmapItem* pi, const QRectF& dwrect, QPainter* painter);

        QImage* getImage(te::qt::widgets::PixmapItem* pi);

        void loadAnimation(const QString& title);

        void removeAnimation(const QString& title);

        void removeOnPropertieCombo(const QString& title);

        /*!
          \brief
          Draw the trajectory icon.

          /param t The trajectory item.
          /param pos The top left position in device coordinates.
          /param painter The painter.
        */
        void drawTrajectoryIcon(const TrajectoryItem* t, const QPoint& pos, QPainter* painter);

        bool trajectoryAlreadyExists(QPair<QString, te::st::TrajectoryDataSetLayer*>& item);
        bool coverageAlreadyExists(QPair<QString, QString>& item);

          /*!
            \brief It initialize a property animation dialog
          */
          void initProperty();

          /*!
            \brief Show Property

            \param b true = show, false = hide
          */
          void showPropertySection(bool b);

          /*!
            \brief Is Setting Changed

            \return true = changed, false = not changed
          */
          bool isSettingChanged();

          /*!
            \brief Remove animation item from the opacity combo box and trajectory color combo box.

            \param ai The item to be removed.
          */
          //void removeComboItem(te::qt::widgets::AnimationItem* ai);

          //void getAuxInfo(te::qt::widgets::AnimationItem* ai, int index = -1);

          //void setAuxInfo(te::qt::widgets::AnimationItem* ai, int index = -1);
          
          void adjustTrajectoryGroupBox(te::qt::widgets::AnimationItem*);

          QString getDateString(const te::dt::TimeInstant& t);


        protected slots:

          /*!
            \brief it draw on display.
            \param p Pointer to QPainter.
          */
          void onDisplayPaintEvent(QPainter* p);

          /*!
            \brief Drag enter event on display.
            \param e The drag enter event.
          */
          void onAnimationDragEnterEvent(QDragEnterEvent* e);

          void onAnimationDropEvent(QDropEvent*);
          /*!
            \brief it opens the configuration window animation.
          */
          void onSettingsToolButtonnClicked();

          /*!
            \brief Play or pause the animation.
          */
          void onPlayToolButtonnClicked();

          /*!
            \brief Stop animation.
          */
          void onStopToolButtonnClicked();

          /*!
            \brief It takes the necessary measures after slider move.
            \param e The slider value.
          */
          void onSliderMoved(int value);

          /*!
            \brief It takes the necessary measures after the change of extent.
          */
          void onExtentChanged();

          /*!
            \brief It takes the necessary measures after the change of extent.
          */
          void onSridChanged();

          /*!
            \brief It takes the necessary measures after the end of the trajectory.
          */
          void onFinishAnimation();

          /*!
            \brief It takes the necessary steps after changing the duration.
            \param v Value of duration in miliseconds.
          */
          void onDurationValueChanged(int v);

          /*!
            \brief It takes the necessary steps after changing the duration.
            \param v Value of opacity (0 - 255).
          */
          void onOpacityValueChanged(int v);

          /*!
            \brief It takes the necessary steps after changing the current time of animation.
            \param t The new current time.
          */
          void onDateTimeEditChanged(const QDateTime& t);

          void onAutoPanCheckBoxClicked(bool);
          void onPanFactorValueChanged(double);
          void onFrontPushButtonClicked(bool b);
          void onBackPushButtonClicked(bool b);

          /*!
            \brief Help button clicked.
          */
          void onHelpPushButtonClicked();

          /*!
            \brief Draw track check box clicked.

            \param b True if the button is checked, or false if the button is unchecked
          */
          void onDrawTrailCheckBoxClicked(bool b);         

          /*!
            \brief Apply animation items push button clicked. It Sets the visibility of animations.

            \param b True if the button is checked, or false if the button is unchecked
          */
          //void onApplyAnimationItemPushButtonClicked(bool);
          
          /*!
            \brief Forward radio button clicked.

            \param b True if the button is checked, or false if the button is unchecked
          */
          void onForwardRadioButtonClicked(bool b);

          /*!
            \brief Backward radio button clicked.

            \param b True if the button is checked, or false if the button is unchecked
          */
          void onBackwardRadioButtonClicked(bool b);

          /*!
            \brief Loop check box clicked.

            \param b True if the button is checked, or false if the button is unchecked
          */
          void onLoopCheckBoxClicked(bool b);

          /*!
            \brief Forward and then backward check box clicked.

            \param b True if the button is checked, or false if the button is unchecked
          */
          void onGoAndBackCheckBoxClicked(bool b);

          /*!
            \brief Apply time interval push button clicked.

            \param b True if the button is checked, or false if the button is unchecked
          */
          void onApplyTimeIntervalPushButtonClicked(bool b);

          /*!
            \brief Trajectory color combo box activated.

            \param i The index of combo box.
          */
          //void onTrajectoryColorComboBoxActivated(int i);

          /*!
            \brief Opacity combo box activated.

            \param i The index of combo box.
          */
          void onAnimationComboBoxActivated(int i);

          /*!
            \brief Remove item animation.
          */
          void onRemovePushButtonClicked(bool);

          /*!
            \brief Remove all animations.
          */
          void onRemoveAllPushButtonClicked(bool);

          /*!
            \brief Reset initial time button clicked.
          */
          void onResetInitialTimePushButtonClicked();

          /*!
            \brief Reset final time button clicked.
          */
          void onResetFinalTimePushButtonClicked();

          void onWidthValueChanged(int);

          void onHeightValueChanged(int);

          void dropAction();

        signals:

          /*!
            \brief This sign warns that this widget must be destroyed.
          */
          void deleteTimeSliderWidget();

        private:

          //struct AnimationAuxInfo
          //{
          //  QString       type;
          //  QString       title;
          //  unsigned char opacity;
          //  bool          drawTrail;
          //  bool          autoPan;
          //  QColor        forwardColor;
          //  QColor        backwardColor;
          //};

          bool                                                    m_loop;                     //!< This property holds whether the slider's animation is on loop.
          int                                                     m_duration;                 //!< The animation's duration time in miliseconds
          QAbstractAnimation::Direction                           m_direction;                //!< QAbstractAnimation::Forward or QAbstractAnimation::Backward
          bool                                                    m_goAndBack;                //!< Forward and then backward direction
          bool                                                    m_comingBack;               //!< Flag to indicate that the trend is coming back
          MapDisplay*                                             m_display;                  //!< The map display
          QGraphicsView*                                          m_animationView;            //!< The animation graphics view
          AnimationScene*                                         m_animationScene;           //!< The animation scene
          QParallelAnimationGroup*                                m_parallelAnimation;        //!< The parallel animation
          te::gm::Envelope                                        m_spatialExtent;            //!< Spatial extent.
          te::dt::TimePeriod                                      m_temporalExtent;           //!< Temporal extent.
          te::dt::TimePeriod                                      m_temporalAnimationExtent;  //!< Animation temporal extent. It is used for show animation.
          int                                                     m_currentTime;              //!< Current animation time (relative to time duration).
          bool                                                    m_erasePerfectly;           //!< flag to erase trajectory piece perfectly (default = false).
          std::auto_ptr<Ui::TimeSliderWidgetForm>                 m_ui;                       //!< The widget form.
          QDateTime                                               m_oldQDateTime;             //!< The old Qt date time.
          QDateTime                                               m_oldIQDateTime;            //!< The old initial Qt date time.
          QDateTime                                               m_oldFQDateTime;            //!< The old final Qt date time.
          bool                                                    m_dateTimeChanged;
          int                                                     m_maxSliderValue;           //!< The max slider value.
          bool                                                    m_finished;
          bool                                                    m_paused;
          QList<QPair<QString, te::st::TrajectoryDataSetLayer*> > m_trajectoryItemList;       //!< List of all trajectory items (title, layer).
          QList<QPair<QString, QString> >                         m_coverageItemList;         //!< List of all animation items (title, path).
          Qt::KeyboardModifiers                                   m_dropModifiers;            //!< Control pressed to add animation with drag and drop.
          QList<QUrl>                                             m_dropUrls;                 //!< Urls to animation with drag and drop.
          QByteArray                                              m_dropBA;                   //!< Layer animation with drag and drop.
          SliderPropertiesDialog*                                 m_spd;                      //!< Slider Properties Dialog.
          //QMap<int, AnimationAuxInfo>                             m_auxInfo;                  //!< animation auxiliar information                                        
      };
    } // end namespace widgets
  }   // end namespace qt
}     // end namespace te

#endif  // __TERRALIB_QT_WIDGETS_INTERNAL_TIMESLIDERWIDGET_H
