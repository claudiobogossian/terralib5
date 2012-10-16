#include "HostDBParamsGetter.h"

#include "ui_HostDBParamsGetter.h"

//! Qt include files
#include <QIntValidator>

namespace te
{
  namespace qt
  {
    namespace widgets
    {
      HostDBParamsGetter::HostDBParamsGetter(QWidget* parent) :
      QWidget(parent),
      m_ui(new Ui::HostDBParamsGetter)
      {
        m_ui->setupUi(this);
        m_ui->m_port_lne->setValidator(new QIntValidator(this));
      }
      
      HostDBParamsGetter::HostDBParamsGetter(QWidget* parent, const std::map<std::string, std::string>& info) :
        QWidget(parent),
        m_ui(new Ui::HostDBParamsGetter)
      {
        m_ui->setupUi(this);
        m_ui->m_port_lne->setValidator(new QIntValidator(this));

        if(!info.empty())
          init(info);
      }

      std::map<std::string, std::string> HostDBParamsGetter::getParams() const
      {
        std::map<std::string, std::string> res;

        res["dbname"] = m_ui->m_dbname_lne->text().toLatin1().data();
        res["host"] = m_ui->m_host_lne->text().toLatin1().data();
        res["user"] = m_ui->m_user_lne->text().toLatin1().data();
        res["password"] = m_ui->m_pwd_lne->text().toLatin1().data();
        res["port"] = m_ui->m_port_lne->text().toLatin1().data();

        return res;
      }

      void HostDBParamsGetter::init(const std::map<std::string, std::string>& info)
      {
        std::map<std::string, std::string>::const_iterator it;

        it = info.find("dbname");
        if(it != info.end())
          m_ui->m_dbname_lne->setText(it->second.c_str());

        it = info.find("host");
        if(it != info.end())
          m_ui->m_host_lne->setText(it->second.c_str());

        it = info.find("user");
        if(it != info.end())
          m_ui->m_user_lne->setText(it->second.c_str());

        it = info.find("password");
        if(it != info.end())
          m_ui->m_pwd_lne->setText(it->second.c_str());

        it = info.find("port");
        if(it != info.end())
          m_ui->m_port_lne->setText(it->second.c_str());
      }
    }
  }
}
