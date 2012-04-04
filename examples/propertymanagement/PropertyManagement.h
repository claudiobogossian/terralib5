#ifndef QTWIDGETS_PROPERTYMANAGEMENT_H
#define QTWIDGETS_PROPERTYMANAGEMENT_H

// Qt
#include <QMainWindow>

// TerraLib
namespace te
{
  namespace da
  {
    class DataSource;
  }
}

// Application
#include "ui_PropertyManagement.h"


class PropertyManagement : public QMainWindow, public Ui_PropertyManagement
{
  Q_OBJECT

  public:
    PropertyManagement(QWidget* parent = 0);

    void setDataSource(te::da::DataSource* ds) { m_ds = ds; }
    te::da::DataSource* getDataSource() const { return m_ds; }

  protected:
    void closeEvent(QCloseEvent* e);

  private slots:
    void addPropertyPushButtonClicked();
    void removePropertyPushButtonClicked();
    void renamePropertyPushButtonClicked();
    void updatePropertyPushButtonClicked();
    void changePropertyDataPushButtonClicked();

  private:
    te::da::DataSource* m_ds;
    std::string m_dataSourceName;
};

#endif  // QTWIDGETS_PROPERTYMANAGEMENT_H
