from launch import LaunchDescription
from launch_ros.actions import Node
from ament_index_python.packages import get_package_share_directory
import os

def generate_launch_description():
    urdf_file = os.path.join(
        get_package_share_directory('my_robot_arm'),
        'urdf', 'arm.urdf')

    return LaunchDescription([
        Node(
            package='robot_state_publisher',
            executable='robot_state_publisher',
            parameters=[{'robot_description': open(urdf_file).read()}]
        ),
        
        Node(
            package='rviz2',
            executable='rviz2',


        ),

# When you run that command(ros2 pkg create --build-type ament_python my_robot_arm), ROS 2 is "smart." It sees that you are creating a robot package, so it automatically generates a bunch of template files.
# This is Youssef's Robot
    ])
