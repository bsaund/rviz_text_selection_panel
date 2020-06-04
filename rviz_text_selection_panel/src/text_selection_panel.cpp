
#include <stdio.h>

#include <QPainter>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QTimer>

#include <geometry_msgs/Twist.h>


#include "selection_widget.h"
#include "text_selection_panel.h"
#include <std_msgs/String.h>

namespace rviz_text_selection_panel
{
    TextSelectionPanel::TextSelectionPanel( QWidget* parent )
        : rviz::Panel( parent )
    {
        // Next we lay out the "output topic" text entry field using a
        // QLabel and a QLineEdit in a QHBoxLayout.
        QHBoxLayout* output_topic_layout = new QHBoxLayout;
        output_topic_layout->addWidget( new QLabel( "Output Topic:" ));
        output_topic_editor_ = new QLineEdit;
        output_topic_layout->addWidget( output_topic_editor_ );

        QHBoxLayout* input_topic_layout = new QHBoxLayout;
        input_topic_layout->addWidget( new QLabel( "Input Topic:" ));
        input_topic_editor_ = new QLineEdit;
        input_topic_layout->addWidget( input_topic_editor_ );

        // Then create the control widget.
        selection_widget_ = new SelectionWidget;

        // Lay out the topic field above the control widget.
        QVBoxLayout* layout = new QVBoxLayout;
        layout->addLayout( input_topic_layout );
        layout->addLayout( output_topic_layout );
        layout->addWidget( selection_widget_ );
        setLayout( layout );

        // Next we make signal/slot connections.
        connect( selection_widget_, SIGNAL( requestNewFile( std::string )), this, SLOT( publishSelection(std::string) ));
        connect( output_topic_editor_, SIGNAL( editingFinished() ), this, SLOT( updateTopic() ));
        connect( input_topic_editor_, SIGNAL( editingFinished() ), this, SLOT( updateTopic() ));

        connect( this, SIGNAL( setNewSelections(std::vector<std::string>) ),
                 selection_widget_, SLOT( updateTextToSelect(std::vector<std::string>) ));

        // Make the control widget start disabled, since we don't start with an output topic.
        selection_widget_->setEnabled( false );
    }

    void TextSelectionPanel::publishSelection(std::string selected)
    {
        std_msgs::String out_str;
        out_str.data = selected;
        selection_publisher_.publish(out_str);
    }

    void TextSelectionPanel::updateSelectionStrings(rviz_text_selection_panel_msgs::TextSelectionOptions new_strs)
    {
        setNewSelections(new_strs.options);
    }

    void TextSelectionPanel::updateTopic()
    {
        setOutputTopic( output_topic_editor_->text() );
        setInputTopic( input_topic_editor_->text() );
    }

    void TextSelectionPanel::setOutputTopic( const QString& new_topic )
    {
        if( new_topic != output_topic_ )
        {
            Q_EMIT configChanged();
            output_topic_ = new_topic;
            // If the topic is the empty string, don't publish anything.
            if( output_topic_ == "" )
            {
                selection_publisher_.shutdown();
            }
            else
            {
                selection_publisher_ = nh_.advertise<std_msgs::String>( output_topic_.toStdString(), 1 );
            }
        }
        selection_widget_->setEnabled( output_topic_ != "" && input_topic_ != "");
    }

    void TextSelectionPanel::setInputTopic( const QString& new_topic )
    {
        if( new_topic != input_topic_ )
        {
            Q_EMIT configChanged();
            input_topic_ = new_topic;
            // If the topic is the empty string, don't publish anything.
            if( input_topic_ == "" )
            {
                selection_subscriber_.shutdown();
            }
            else
            {
                // TODO This needs to be a string array
                selection_subscriber_ = nh_.subscribe( input_topic_.toStdString(), 1,
                                                       &TextSelectionPanel::updateSelectionStrings, this); 
            }
        }
        selection_widget_->setEnabled( output_topic_ != "" && input_topic_ != "");
    }


// Save all configuration data from this panel to the given
// Config object.  It is important here that you call save()
// on the parent class so the class id and panel name get saved.
    void TextSelectionPanel::save( rviz::Config config ) const
    {
        rviz::Panel::save( config );
        config.mapSetValue( "OutputTopic", output_topic_ );
        config.mapSetValue( "InputTopic", input_topic_ );
    }

// Load all configuration data for this panel from the given Config object.
    void TextSelectionPanel::load( const rviz::Config& config )
    {
        rviz::Panel::load( config );
        QString topic;
        if( config.mapGetString( "OutputTopic", &topic ))
        {
            output_topic_editor_->setText( topic );
            updateTopic();
        }
        if( config.mapGetString( "InputTopic", &topic ))
        {
            input_topic_editor_->setText( topic );
            updateTopic();
        }
    }

} // end namespace rviz_text_selection_panel


// Tell pluginlib about this class.  Every class which should be
// loadable by pluginlib::ClassLoader must have these two lines
// compiled in its .cpp file, outside of any namespace scope.
#include <pluginlib/class_list_macros.h>
PLUGINLIB_EXPORT_CLASS(rviz_text_selection_panel::TextSelectionPanel,rviz::Panel )

