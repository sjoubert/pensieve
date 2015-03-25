#ifndef PSV_PENSIEVEWINDOW_HPP_20150322133442
#define PSV_PENSIEVEWINDOW_HPP_20150322133442

#include "psvPensieve.hpp"

#include <QAbstractTableModel>

namespace psv
{

/**
 * Qt model of a pensieve
 */
class PensieveModel:
  public QAbstractTableModel
{
public:
  /**
   * Construct the model
   * @param p_parent Parent object
   */
  PensieveModel(QObject* p_parent = nullptr);

  PensieveModel(PensieveModel const&) = delete;

  /**
   * Destruct the pensieve
   */
  ~PensieveModel();

  PensieveModel& operator=(PensieveModel const&) = delete;

  /**
   * Get the underlying pensieve data
   * @return Current pensieve data
   */
  Pensieve const& GetPensieve() const;

  /**
   * Set the pensieve data
   * @param p_pensieve Data to set in the model
   */
  void SetPensieve(Pensieve const& p_pensieve);

  /**
   * Get the number of row in the model
   * @param p_parent Parent element whose row count to return
   * @return Current number of row
   */
  int rowCount(QModelIndex const& p_parent = QModelIndex()) const override;

  /**
   * Get the number of column in the model
   * @param p_parent Parent element whose column count to return
   * @return Current number of column
   */
  int columnCount(QModelIndex const& p_parent = QModelIndex()) const override;

  /**
   * Get the header data of the model
   * @param p_section Index of the section (row/column)
   * @param p_orientation Orientation of the header
   * @param p_role Role for which to return a data
   * @return The corresponding header data
   */
  QVariant headerData(int p_section, Qt::Orientation p_orientation,
    int p_role = Qt::DisplayRole) const override;

  /**
   * Get an element data
   * @param p_index Element index
   * @param p_role Role for which to return a data
   * @return The corresponding data
   */
  QVariant data(QModelIndex const& p_index,
    int p_role = Qt::DisplayRole) const override;

private:
  /// Pensieve instance
  Pensieve m_pensieve;
};

}

#endif
