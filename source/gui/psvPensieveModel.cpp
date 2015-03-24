#include "psvPensieveModel.hpp"

#include <QStringList>

namespace psv
{

PensieveModel::PensieveModel(QObject* p_parent):
  QAbstractTableModel(p_parent)
{
  Thought thought;
  thought.SetTitle("toto");
  thought.SetContent("l'escargot qui porte sa maison sur le dos");
  thought.AddFlag("animal");
  thought.AddFlag("joke");
  m_pensieve.GetThoughts().push_back(thought);
  thought = Thought();
  thought.SetTitle("meuh");
  thought.SetContent("la vache à lait");
  thought.AddFlag("animal");
  thought.AddFlag("wtf");
  m_pensieve.GetThoughts().push_back(thought);
}

PensieveModel::~PensieveModel() = default;

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
