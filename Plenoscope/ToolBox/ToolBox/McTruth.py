#!/usr/bin/env python
# coding: utf-8
from __future__ import absolute_import, print_function, division
import numpy as np
import os
from HeaderRepresentation import corsika_event_header_repr
from HeaderRepresentation import corsika_run_header_repr

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

    def __str__(self):
        out = '_Monte_Carlo_Truth_\n'
        out+= corsika_run_header_repr(self.corsika_run_header)
        out+= '\n'
        out+= corsika_event_header_repr(self.corsika_event_header)
        return out

    def __repr__(self):
        return self.__str__()