#include "HighlightDelegate.h"

namespace te 
{
  namespace qt 
  {
    namespace widgets 
    {
      HighlightDelegate::HighlightDelegate(QObject * parent) :
      QStyledItemDelegate(parent),
      m_enabled(true)
      {
      }

      HighlightDelegate::~HighlightDelegate()
      {
      }

      void HighlightDelegate::paint(QPainter * painter, const QStyleOptionViewItem & option, const QModelIndex & index) const
      {
        if(!index.isValid())
          return;

        if(m_enabled)
        {
          std::string key = getPKey(index);

          if(!key.empty() && m_hlOids.find(key) != m_hlOids.end())
            painter->fillRect(option.rect, m_color);
        }

        QStyledItemDelegate::paint(painter, option, index);
      }

      void HighlightDelegate::setEnabled(const bool& enable)
      {
        m_enabled = enable;
      }

      void HighlightDelegate::setHighlightedPkeys(const std::set<std::string> & value)
      {
        m_hlOids = value;
      }

      void HighlightDelegate::setHighlightColor(const QColor & value)
      {
        m_color = value;
      }

      QColor HighlightDelegate::getHighlightColor() const
      {
        return m_color;
      }

      void HighlightDelegate::setPKeysColumns(const std::vector<size_t> & pKeyCols)
      {
        m_pkeys = pKeyCols;
      }

      std::vector<size_t> HighlightDelegate::getPKeysColumns()
      {
        return m_pkeys;
      }

      std::string HighlightDelegate::getPKey(const QModelIndex & index, char separator) const
      {
        std::string key;
        int row = index.row();
        QModelIndex aux;

        for(size_t i = 0; i < m_pkeys.size(); i++)
        {
          if(i > 0)
            key += separator;

          aux = index.sibling(row, (int) m_pkeys[i]);

          key += aux.data(Qt::DisplayRole).toString().toAscii().data();
        }

        return key;
      }

      std::set<std::string> HighlightDelegate::getHighlightKeys()
      {
        return m_hlOids;
      }

      void HighlightDelegate::clearSelection()
      {
        m_hlOids.clear();
      }

      void HighlightDelegate::setGroupName(const QString& name)
      {
        m_grp_name = name;
      }
      
      QString HighlightDelegate::getGroupName() const
      {
        return m_grp_name;
      }
    } // namespace te::qt::widgets
  } // namespace te::qt
} // namespace te
