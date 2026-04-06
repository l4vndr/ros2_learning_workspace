from setuptools import find_packages, setup

package_name = "my_py_pkg"

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
            "py_node = my_py_pkg.first_node:main",
            "py_test = my_py_pkg.first_node_oop:main",
            "robo_news = my_py_pkg.robot_news:main",
            "robo_news_rec = my_py_pkg.robo_news_rec:main",
            "sum_server = my_py_pkg.sum_server:main",
            "sum_client = my_py_pkg.sum_client:main",
            "sum_client_oop = my_py_pkg.sum_client_oop:main",
            "number_publisher = my_py_pkg.number_publisher:main",
            "number_counter = my_py_pkg.number_counter:main",
            "number_publisher_client = my_py_pkg.number_publisher_client:main",
            "led_panel = my_py_pkg.led_panel:main",
            "battery = my_py_pkg.battery:main",
        ],
    },
)
