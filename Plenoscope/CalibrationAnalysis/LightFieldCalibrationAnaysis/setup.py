from distutils.core import setup

setup(
    name='LightFieldCalibrationAnaysis',
    version='0.0.1',
    description='analyse the light field calibration of a plenoscope',
    author='Sebastian Mueller',
    author_email='sebmuell@phys.ethz.ch',
    license='MIT',
    packages=[
        'LightFieldCalibrationAnaysis',
    ],
    install_requires=[
        'docopt'
    ],
    entry_points={'console_scripts': [
        'mctLightFieldCalibrationAnaysis = LightFieldCalibrationAnaysis.__init__:main',
    ]},
    zip_safe=False,
)
