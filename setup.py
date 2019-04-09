#!/usr/bin/env python

"""
setup.py file for SWIG example
"""

from distutils.core import setup, Extension


merlict_module = Extension(
    '_merlict',
    sources=[
        'merlict_wrap.cpp',
        'merlict.cpp'
    ],
)

setup (
    name='merlict',
    version='0.1',
    author="SWIG Docs",
    description="""Simple swig example from docs""",
    ext_modules=[merlict_module],
    py_modules=["merlict"],
)
