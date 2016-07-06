from setuptools import setup

setup(
    name='mctPlenoscopeToolbox',
    version='0.0.0',
    description='View and work on plenoscope events',
    url='',
    author='Sebastian Mueller, Max L. Ahnen, Dominik Neise',
    author_email='sebmuell@phys.ethz.ch',
    license='MIT',
    packages=[
        'ToolBox.LixelStatistics',
        ],
    install_requires=[
        'numpy',            # in anaconda
    ],
    entry_points={'console_scripts': [
        'mctPlenoscopePlotLixelStatistics = ToolBox.LixelStatistics.__init__:main',
    ]},
    zip_safe=False
)
