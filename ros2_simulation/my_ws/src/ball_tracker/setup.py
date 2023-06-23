from setuptools import setup
from glob import glob

package_name = 'ball_tracker'

setup(
    name=package_name,
    version='0.0.0',
    packages=[package_name],
    data_files=[
        ('share/ament_index/resource_index/packages',
            ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),
        ('share/' + package_name + '/launch', glob('launch/*launch.py')),
        ('share/' + package_name + '/config', glob('config/*.yaml')),
    ],
    install_requires=['setuptools'],
    zip_safe=True,
    maintainer='newans',
    maintainer_email='josh.newans@gmail.com',
    description='TODO: Package description',
    license='TODO: License declaration',
    tests_require=['pytest'],
    entry_points={
        'console_scripts': [
            'detect_stop_sign = ball_tracker.detect_stop_sign:main',
            'detect_stop_sign_3d = ball_tracker.detect_stop_sign_3d:main',
            'follow_path = ball_tracker.follow_path:main',
            'detect_path = ball_tracker.detect_path:main',
        ],
    },
)
