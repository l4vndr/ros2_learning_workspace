from setuptools import find_packages, setup

package_name = "line_follower_python"

setup(
    name=package_name,
    version="0.0.0",
    packages=find_packages(exclude=["test"]),
    data_files=[
        ("share/ament_index/resource_index/packages", ["resource/" + package_name]),
        ("share/" + package_name, ["package.xml"]),
    ],
    install_requires=["setuptools"],
    zip_safe=True,
    maintainer="root",
    maintainer_email="arinkumar10@outlook.com",
    description="TODO: Package description",
    license="TODO: License declaration",
    extras_require={
        "test": [
            "pytest",
        ],
    },
    entry_points={
        "console_scripts": [
            "line_sensor = line_follower_python.line_sensor:main",
            "pid_controller = line_follower_python.pid_controller:main",
            "motor_driver = line_follower_python.motor_driver:main",
        ],
    },
)
