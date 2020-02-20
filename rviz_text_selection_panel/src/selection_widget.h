#ifndef SELECTION_WIDGET_H
#define SELECTION_WIDGET_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>


namespace rviz_text_selection_panel
{
    class SelectionWidget: public QWidget
    {

        int current_index = 0;
        int increment = 1;

        Q_OBJECT
    public:
        // This class is not instantiated by pluginlib::ClassLoader, so the
        // constructor has no restrictions.
        SelectionWidget( QWidget* parent = 0 );
        QLabel *display_label, *display_index;
        QLineEdit *increment_editor;

        std::vector<std::string> text_options;
        bool options_available;

        // We emit outputVelocity() whenever it changes.
    Q_SIGNALS:
        void requestNewFile(std::string filename);
            
    protected:
        void updateLabel();

    protected Q_SLOTS:
        void next();
        void prev();
        void updateIncrement();
        void updateTextToSelect(std::vector<std::string> options);
    };

} // end namespace rviz_text_selection_panel


#endif // DRIVE_WIDGET_H
