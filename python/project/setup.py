from distutils.core import setup
from setuptools import find_packages, findall

setup(
    name='AP2_DevOps',
    version='0.0.1',
    python_requires='>=3.6',
    packages=find_packages(exclude=("tests*",)),
    scripts=findall('./scripts'),
)
