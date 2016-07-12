#!/usr/bin/env python
# coding: utf-8
from __future__ import absolute_import, print_function, division
import numpy as np
import os
from HeaderRepresentation import corsika_event_header_repr
from HeaderRepresentation import corsika_run_header_repr

__all__ = ['McTruth']

def CORSIKA_primary_ID2str(PRMPAR):
    PRMPAR = int(PRMPAR)
    if PRMPAR == 1:
        return 'gamma'
    elif PRMPAR == 2:
        return 'e^+'
    elif PRMPAR == 3:
        return 'e^-'
    elif PRMPAR == 5:
        return 'muon^+'
    elif PRMPAR == 6:
        return 'muon^-'
    elif PRMPAR == 7:
        return 'pion^0'
    elif PRMPAR == 8:
        return 'pion^+'
    elif PRMPAR == 9:
        return 'pion^-'
    elif PRMPAR == 14:
        return 'p'
    elif PRMPAR > 200:

        A = int(np.floor(PRMPAR/100))
        Z = PRMPAR - A*100
        if A == 4 and Z == 2:
            return 'He A4 Z2'
        else:
            return 'A'+str(round(A))+' Z'+str(round(Z))
    else:
        return str(PRMPAR)          

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

    def short_event_info(self):
        az = str(round(np.rad2deg(self.corsika_event_header[12-1]),2))
        zd = str(round(np.rad2deg(self.corsika_event_header[11-1]),2))
        core_y = str(round(0.01*self.corsika_event_header[118],2))
        core_x = str(round(0.01*self.corsika_event_header[98],2))
        E = str(round(self.corsika_event_header[4-1],2))
        PRMPAR = self.corsika_event_header[3-1]

        return str(
            CORSIKA_primary_ID2str(PRMPAR)+', '+
            "E: "+E+"GeV, \n"+
            "core pos: x="+core_x+'m, '+
            "y="+core_y+'m, '+
            "direction: Zd="+zd+'deg, '+
            "Az="+az+'deg')