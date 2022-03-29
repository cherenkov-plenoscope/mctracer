import setuptools
import os

with open("README.md", "r", encoding="utf-8") as f:
    long_description = f.read()

setuptools.setup(
    name="merlict_camera_server",
    version="0.0.0",
    author="Sebastian Achim Mueller",
    author_email="sebastian-achim.mueller@mpi-hd.mpg.de",
    description="Communicate with merlict's camera-server to render images",
    long_description=long_description,
    long_description_content_type="text/md",
    url="https://github.com/cherenkov-plenoscope/merlict_development_kit",
    classifiers=[
        "Programming Language :: Python :: 3",
        "License :: OSI Approved :: MIT License",
        "Operating System :: OS Independent",
    ],
    packages=["merlict_camera_server",],
    python_requires=">=3.0",
)
