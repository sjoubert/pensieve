#ifndef PSV_PENSIEVEWIDGET_HPP_20150325182530
#define PSV_PENSIEVEWIDGET_HPP_20150325182530

#include <QScrollArea>
#include <QSignalMapper>

#include <memory>

namespace Ui
{
class PensieveWidget;
}

namespace psv
{
class Pensieve;
class Thought;
class ThoughtWidget;

/**
 * Widget of a pensieve
 */
class PensieveWidget:
  public QScrollArea
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

  /**
   * Set the flags filter regex
   * @param p_filter Flags filter
   */
  void SetFlagsFilter(QRegExp const& p_filter);

  /**
   * Get the flags filter regex
   * @return Flags filter
   */
  QRegExp const& GetFlagsFilter() const;

  /**
   * Get the number of thoughts matching the flags filter
   * @return The number of highlighted thoughts
   */
  unsigned int GetHighlightedCount() const;

public Q_SLOTS:
  /**
   * Create a new empty thought
   */
  void CreateThought();

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

  /**
   * Helper method to determine if a thought should be highlighted
   * @param p_thought Thought to test
   * @return If the thought should be highlighted
   */
  bool ShouldBeHighlighted(Thought const& p_thought) const;

  /// Widget UI
  std::unique_ptr<Ui::PensieveWidget> m_ui;
  /// Thought widgets container
  std::vector<ThoughtWidget*> m_thoughtWidgets;
  /// Mapper for edition start signal
  QSignalMapper m_startEditionMapper;
  /// Mapper for deletion signal
  QSignalMapper m_deletionMapper;
  /// Flags filter
  QRegExp m_flagsFilter;
};

}

#endif
