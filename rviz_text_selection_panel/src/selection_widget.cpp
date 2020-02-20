#include <stdio.h>
#include <math.h>

#include <QPainter>
#include <QMouseEvent>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include <QPushButton>
#include <iostream>

#include "selection_widget.h"

namespace rviz_text_selection_panel
{
    std::vector<std::string> tmp_filenames = {
        "aaaaa_0", "aaaaa_1", "aaaaa_2", "aaaaa_3",
        "aaaab_0", "aaaab_1", "aaaab_2", "aaaab_3",
        "aaaac_0", "aaaac_1", "aaaac_2", "aaaac_3",
    }; 

    SelectionWidget::SelectionWidget( QWidget* parent )
        : QWidget( parent ),
          options_available(false)
    {
        QVBoxLayout* layout = new QVBoxLayout(this);

        QHBoxLayout* skip_ahead_layout = new QHBoxLayout();
        skip_ahead_layout->addWidget(new QLabel("increment: "));
        increment_editor = new QLineEdit("1");
        skip_ahead_layout->addWidget(increment_editor);

        connect( increment_editor, SIGNAL( editingFinished() ), this, SLOT( updateIncrement() ));

        
        
        display_label = new QLabel("No option received yet");
        display_index = new QLabel(std::to_string(current_index).c_str());
        auto next_button = new QPushButton("next");
        auto prev_button = new QPushButton("prev");

        next_button->setFixedWidth(4*10);
        prev_button->setFixedWidth(4*10);
        display_index->setFixedWidth(5*10);
        
        QHBoxLayout* selection_layout = new QHBoxLayout();        
        selection_layout->addWidget(prev_button);
        selection_layout->addWidget(display_label);
        selection_layout->addWidget(display_index);
        selection_layout->addWidget(next_button);

        layout->addLayout(skip_ahead_layout);
        layout->addLayout(selection_layout);
        


        connect(next_button, SIGNAL (released()), this, SLOT(next()));
        connect(prev_button, SIGNAL (released()), this, SLOT(prev()));
        updateLabel();

    }

    void SelectionWidget::updateTextToSelect(std::vector<std::string> options)
    {
        if(options.size() <= 0)
        {
            return;
        }
        text_options = options;
        options_available = true;
        current_index = 0;
        updateLabel();
    }


    void SelectionWidget::next()
    {
        current_index += increment;
        current_index = std::min<int>(current_index, text_options.size() - 1);
        updateLabel();
    }

    void SelectionWidget::prev()
    {
        current_index -= increment;
        current_index = std::max<int>(current_index, 0);
        updateLabel();
    }

    void SelectionWidget::updateIncrement()
    {
        std::string text = increment_editor->text().toStdString();
        increment = std::stoi(text);
    }
    

    void SelectionWidget::updateLabel()
    {
        if(!options_available)
        {
            return;
        }
        
        display_label->setText(text_options[current_index].c_str());
        display_index->setText(std::to_string(current_index).c_str());
        Q_EMIT requestNewFile(text_options[current_index]);
    }


} // end namespace rviz_text_selection_panel
