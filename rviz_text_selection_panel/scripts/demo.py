#!/usr/bin/env python


import rospy
from rviz_text_selection_panel_msgs.msg import TextSelectionOptions
from std_msgs.msg import String
from visualization_msgs.msg import Marker


OPTIONS_TOPIC = '/text_selection_options'
SELECTED_TOPIC = '/selected_option'
MARKER_TOPIC = '/selected_option_marker'



text = ['Here is some text',
        'you can select this',
        'or this',
        'or any other text',
        'from this list',
        'data_1',
        'data_2',
        'data_3',
        'data_4',
        'data_5',
        'data_6',
        'data_7',
        'data_8',
]


def send_options(pub):
    options = TextSelectionOptions()
    options.options = text
    pub.publish(options)

def republish_as_marker(pub, str_msg):
    rospy.loginfo("Received selection {}. Republishing as a marker".format(str_msg.data))
    marker = Marker()
    marker.header.frame_id = "world"
    marker.color.a = 1.0
    marker.color.g = 1.0
    marker.type = Marker.TEXT_VIEW_FACING
    marker.scale.x=0.3
    marker.scale.y=0.3
    marker.scale.z=0.1
    marker.text = str_msg.data
    pub.publish(marker)
    

if __name__ == "__main__":
    rospy.init_node('demo_text_selection')
    rospy.loginfo("Running Demo Script for rviz text selection")

    pub = rospy.Publisher(OPTIONS_TOPIC, TextSelectionOptions, queue_size=1)
    marker_pub = rospy.Publisher(MARKER_TOPIC, Marker, queue_size=1)
    sub = rospy.Subscriber(SELECTED_TOPIC, String, lambda x: republish_as_marker(marker_pub, x))
    rospy.sleep(1.0)
    send_options(pub)
    rospy.spin()
