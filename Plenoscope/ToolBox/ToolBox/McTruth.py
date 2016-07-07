#!/usr/bin/env python
# coding: utf-8
from __future__ import absolute_import, print_function, division
import numpy as np
import os

__all__ = ['McTruth']

class McTruth(object):
    def __init__(self, path):
        self.corsika_event_header = self.__read_273_float_header(
            os.path.join(path, 'corsika_event_header.bin')
        )

        self.corsika_run_header = self.__read_273_float_header(
            os.path.join(path, 'corsika_run_header.bin')
        )

    def __read_273_float_header(self, path):
        raw = np.fromfile(path, dtype=np.float32)
        return raw