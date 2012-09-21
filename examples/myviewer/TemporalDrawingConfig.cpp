#include "TemporalDrawingConfig.h"

//QT
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QGroupBox>
#include <QAction>

TemporalDrawingConfig::TemporalDrawingConfig(QWidget* parent, Qt::WindowFlags f) : 
  QDialog(parent, f)
{
  setWindowTitle("Temporal Drawing Config");
  QVBoxLayout* vlayout = new QVBoxLayout(this);
  QGroupBox* g1 = new QGroupBox(this);
  QLabel* label1 = new QLabel("Date Interval:", g1);
  m_intervalDateComboBox = new QComboBox(g1);
  QGridLayout* layout1 = new QGridLayout(g1);
  layout1->addWidget(label1, 0, 0, Qt::AlignLeft);
  layout1->addWidget(m_intervalDateComboBox, 0, 1, Qt::AlignLeft);

  QLabel* label11 = new QLabel("Drawing Interval:", g1);
  m_intervalDrawingComboBox = new QComboBox(g1);
  layout1->addWidget(label11, 1, 0, Qt::AlignLeft);
  layout1->addWidget(m_intervalDrawingComboBox, 1, 1, Qt::AlignLeft);

  m_drawLinesCheckBox = new QCheckBox("Draw Lines", g1);
  m_loopCheckBox = new QCheckBox("Loop", g1);
  layout1->addWidget(m_drawLinesCheckBox, 2, 0, Qt::AlignLeft);
  layout1->addWidget(m_loopCheckBox, 3, 0, Qt::AlignLeft);
  vlayout->addWidget(g1);

  QGroupBox* g2 = new QGroupBox(this);
  QGridLayout* layout2 = new QGridLayout(g2);
  QLabel* label2 = new QLabel("Initial Time:", g2);
  m_initialTimeLineEdit = new QLineEdit(g2);
  QLabel* label21 = new QLabel("Final Time:", g2);
  m_finalTimeLineEdit = new QLineEdit(g2);
  m_defaultTimePushButton = new QPushButton("Default", g2);
  layout2->addWidget(label2, 0, 0, Qt::AlignLeft);
  layout2->addWidget(m_initialTimeLineEdit, 0, 1, Qt::AlignLeft);
  layout2->addWidget(label21, 1, 0, Qt::AlignLeft);
  layout2->addWidget(m_finalTimeLineEdit, 1, 1, Qt::AlignLeft);
  layout2->addWidget(m_defaultTimePushButton, 2, 0, Qt::AlignLeft);
  vlayout->addWidget(g2);

  QGroupBox* g3 = new QGroupBox(this);
  m_okPushButton = new QPushButton("Ok", g3);
  m_cancelPushButton = new QPushButton("Cancel", g3);
  QHBoxLayout* layout5 = new QHBoxLayout(g3);
  layout5->addWidget(m_okPushButton);
  layout5->addSpacing(50);
  layout5->addWidget(m_cancelPushButton);
  vlayout->addWidget(g3);

  setLayout(vlayout);

  QObject::connect(m_defaultTimePushButton, SIGNAL(clicked()), this, SLOT(defaultTimeSlot()));
  QObject::connect(m_okPushButton, SIGNAL(clicked()), this, SLOT(okSlot()));
  QObject::connect(m_cancelPushButton, SIGNAL(clicked()), this, SLOT(cancelSlot()));

  populeWidgets();
}

TemporalDrawingConfig::~TemporalDrawingConfig()
{
}

void TemporalDrawingConfig::populeWidgets()
{
  m_intervalDateComboBox->addItem("15 min");
  m_intervalDateComboBox->addItem("30 min");
  m_intervalDateComboBox->addItem("1 hora");
  m_intervalDateComboBox->addItem("1 dia");
  m_intervalDateComboBox->addItem("2 dias");
  m_intervalDateComboBox->addItem("3 dias");
  m_intervalDateComboBox->addItem("4 dias");
  m_intervalDateComboBox->addItem("5 dias");
  m_intervalDateComboBox->addItem("6 dias");
  m_intervalDateComboBox->addItem("7 dias");
  m_intervalDateComboBox->addItem("8 dias");
  m_intervalDateComboBox->addItem("9 dias");
  m_intervalDateComboBox->addItem("10 dias");
  m_intervalDateComboBox->addItem("20 dias");
  m_intervalDateComboBox->addItem("30 dias");
  m_intervalDateComboBox->addItem("40 dias");
  m_intervalDateComboBox->addItem("50 dias");
  m_intervalDateComboBox->addItem("60 dias");
  m_intervalDateComboBox->addItem("70 dias");
  m_intervalDateComboBox->addItem("80 dias");
  m_intervalDateComboBox->addItem("90 dias");
  m_intervalDateComboBox->addItem("100 dias");

  m_intervalDrawingComboBox->addItem("100 ms");
  m_intervalDrawingComboBox->addItem("200 ms");
  m_intervalDrawingComboBox->addItem("300 ms");
  m_intervalDrawingComboBox->addItem("400 ms");
  m_intervalDrawingComboBox->addItem("500 ms");
  m_intervalDrawingComboBox->addItem("600 ms");
  m_intervalDrawingComboBox->addItem("700 ms");
  m_intervalDrawingComboBox->addItem("800 ms");
  m_intervalDrawingComboBox->addItem("900 ms");
  m_intervalDrawingComboBox->addItem("1000 ms");
  m_intervalDrawingComboBox->addItem("1100 ms");
  m_intervalDrawingComboBox->addItem("1200 ms");
  m_intervalDrawingComboBox->addItem("1300 ms");
  m_intervalDrawingComboBox->addItem("1400 ms");
  m_intervalDrawingComboBox->addItem("1500 ms");
  m_intervalDrawingComboBox->addItem("1600 ms");
  m_intervalDrawingComboBox->addItem("1700 ms");
  m_intervalDrawingComboBox->addItem("1800 ms");
  m_intervalDrawingComboBox->addItem("1900 ms");
  m_intervalDrawingComboBox->addItem("2000 ms");
}

void TemporalDrawingConfig::okSlot()
{
  accept();
  hide();
}

void TemporalDrawingConfig::cancelSlot()
{
  reject();
  hide();
}

void TemporalDrawingConfig::setDefaultTimes(te::dt::DateTime* initial, te::dt::DateTime* final)
{
  te::dt::TimeInstant* i = (te::dt::TimeInstant*)initial;
  te::dt::TimeInstant* f = (te::dt::TimeInstant*)final;
  m_initialDefaultTime = *i;
  m_finalDefaultTime = *f;

  if(m_initialTimeLineEdit->text().isEmpty())
    defaultTimeSlot();
}

void TemporalDrawingConfig::defaultTimeSlot()
{
  QString qinitial = m_initialDefaultTime.toString().c_str();
   QString qfinal = m_finalDefaultTime.toString().c_str();

  m_initialTimeLineEdit->setText(qinitial);
  m_finalTimeLineEdit->setText(qfinal);
}