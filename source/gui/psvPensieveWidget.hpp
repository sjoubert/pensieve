#ifndef PSV_PENSIEVEWIDGET_HPP_20150325182530
#define PSV_PENSIEVEWIDGET_HPP_20150325182530

#include <QWidget>

class QGridLayout;
class QSignalMapper;

namespace psv
{
class Pensieve;
class ThoughtWidget;

/**
 * Widget of a pensieve
 */
class PensieveWidget:
  public QWidget
{
  Q_OBJECT

public:
  /**
   * Construct the widget
   * @param p_parent Parent widget
   */
  PensieveWidget(QWidget* p_parent = nullptr);

  PensieveWidget(PensieveWidget const&) = delete;

  /**
   * Destruct the widget
   */
  ~PensieveWidget();

  PensieveWidget& operator=(PensieveWidget const&) = delete;

  /**
   * Get the underlying pensieve data
   * @return Pensieve data
   */
  Pensieve GetPensieve() const;

  /**
   * Set the pensieve data to use
   * @param p_pensieve Pensieve data
   */
  void SetPensieve(Pensieve const& p_pensieve);

  /**
   * Adjust the widgets layout
   * @param p_event Resize event to handle
   */
  void resizeEvent(QResizeEvent* p_event) override;

Q_SIGNALS:
  /**
   * Emitted when data have been modified
   */
  void Modified() const;

private Q_SLOTS:
  /**
   * Handle the beginning of a though edition
   * @param p_widget Widget being edited
   */
  void StartEdition(QWidget* p_widget);

  /**
   * Handle the end of a thought edition
   */
  void EndEdition();

  /**
   * Delete a thought
   * @param p_widget Widget requesting the deletion
   */
  void DeleteThought(QWidget* p_widget);

private:
  /**
   * Layout the widget according to their number and size
   */
  void LayoutWidgets();

  /// Main layout
  QGridLayout* m_mainLayout;
  /// Thought widgets container
  std::vector<ThoughtWidget*> m_thoughtWidgets;
  /// Mapper for edition start signal
  QSignalMapper* m_startEditionMapper;
  /// Mapper for deletion signal
  QSignalMapper* m_deletionMapper;
};

}

#endif
