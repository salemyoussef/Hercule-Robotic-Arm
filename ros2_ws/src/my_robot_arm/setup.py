from setuptools import setup

package_name = 'my_robot_arm'

setup(
    name=package_name,
    version='0.0.0',
    packages=[package_name],
    data_files=[
        ('share/ament_index/resource_index/packages',
            ['resource/my_robot_arm']),
        ('share/my_robot_arm', ['package.xml']),
        ('share/my_robot_arm/urdf', ['urdf/arm.urdf']),
        ('share/my_robot_arm/launch', ['launch/display.launch.py']),
        ('share/my_robot_arm/config', ['config/arm.rviz']),
    ],
    install_requires=['setuptools'],
    zip_safe=True,
    maintainer='salemyoussef',
    maintainer_email='salemyoussef@users.noreply.github.com',
    description='ROS 2 package for Hercule 6-DOF robotic arm visualization and kinematics',
    license='Apache-2.0',
    tests_require=['pytest'],
    entry_points={
        'console_scripts': [
           'ik_target = my_robot_arm.ik_target:main',
        ],
    },
)
