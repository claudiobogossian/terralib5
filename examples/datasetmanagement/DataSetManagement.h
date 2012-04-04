#ifndef TERRALIB_EXAMPLE_DATASETMANAGEMENT_H
#define TERRALIB_EXAMPLE_DATASETMANAGEMENT_H

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
#include "ui_DataSetManagement.h"


class DataSetManagement : public QMainWindow, public Ui_DataSetManagement
{
  Q_OBJECT

  public:
    DataSetManagement(QWidget* parent = 0);

    void setDataSource(te::da::DataSource* ds) { m_ds = ds; }
    te::da::DataSource* getDataSource() const { return m_ds; }

  protected:
    void closeEvent(QCloseEvent* e);

  private slots:
    void createDataSetPushButtonClicked();
    void removeDataSetPushButtonClicked();
    void renameDataSetPushButtonClicked();

    void addPrimaryKeyPushButtonClicked();
    void removePrimaryKeyPushButtonClicked();

    void addForeignKeyPushButtonClicked();
    void removeForeignKeyPushButtonClicked();

    void addUniqueKeyPushButtonClicked();
    void removeUniqueKeyPushButtonClicked();

    void addCheckConstraintPushButtonClicked();
    void removeCheckConstraintPushButtonClicked();

    void addIndexPushButtonClicked();
    void removeIndexPushButtonClicked();

    void addSequencePushButtonClicked();
    void removeSequencePushButtonClicked();

  private:
    te::da::DataSource* m_ds;
    std::string m_dataSourceName;
};

#endif  // TERRALIB_EXAMPLE_DATASETMANAGEMENT_H
