#include "psvPensieveModel.hpp"

#include <QStringList>

namespace psv
{

PensieveModel::PensieveModel(QObject* p_parent):
  QAbstractTableModel(p_parent)
{
}

PensieveModel::~PensieveModel() = default;

Pensieve const& PensieveModel::GetPensieve() const
{
  return m_pensieve;
}

void PensieveModel::SetPensieve(Pensieve const& p_pensieve)
{
  // Clear data
  beginRemoveRows(QModelIndex(), 0, m_pensieve.GetThoughts().size() - 1);
  m_pensieve = Pensieve();
  endRemoveRows();

  // Set new data
  beginInsertRows(QModelIndex(), 0, p_pensieve.GetThoughts().size() - 1);
  m_pensieve = p_pensieve;
  endInsertRows();
}

int PensieveModel::rowCount(QModelIndex const& p_parent) const
{
  // This is a table model, there is no child elements
  if(p_parent.isValid())
  {
    return 0;
  }

  return m_pensieve.GetThoughts().size();
}

int PensieveModel::columnCount(QModelIndex const& p_parent) const
{
  // This is a table model, there is no child elements
  if(p_parent.isValid())
  {
    return 0;
  }

  // Title, content and flags
  return 3;
}

QVariant PensieveModel::headerData(int p_section,
  Qt::Orientation p_orientation, int p_role) const
{
  if(p_orientation == Qt::Horizontal && p_role == Qt::DisplayRole)
  {
    switch(p_section)
    {
      case 0:
      {
        return tr("Title");
      }
      case 1:
      {
        return tr("Content");
      }
      case 2:
      {
        return tr("Flags");
      }
    }
  }

  return QAbstractTableModel::headerData(p_section, p_orientation, p_role);
}

QVariant PensieveModel::data(QModelIndex const& p_index, int p_role) const
{
  if(p_index.isValid() &&
    p_index.row() < rowCount() && p_index.column() < columnCount() &&
    p_role == Qt::DisplayRole)
  {
    Thought const& thought = m_pensieve.GetThoughts()[p_index.row()];
    switch(p_index.column())
    {
      case 0:
      {
        return QString::fromStdString(thought.GetTitle());
      }
      case 1:
      {
        return QString::fromStdString(thought.GetContent());
      }
      case 2:
      {
        QStringList qFlags;
        for(auto const& flag: thought.GetFlags())
        {
          qFlags.push_back(QString::fromStdString(flag));
        }
        return qFlags.join("|");
      }
    }
  }

  return QVariant();
}

}
