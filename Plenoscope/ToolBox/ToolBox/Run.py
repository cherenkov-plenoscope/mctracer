#!/usr/bin/env python
# coding: utf-8
from __future__ import absolute_import, print_function, division
import numpy as np
import glob, os
from LixelStatistics import LixelStatistics
from Event import Event

__all__ = ['Run']
    
class Run(object):
    def __init__(self, path):
        self.__path = os.path.abspath(path)
        self.__path_input = os.path.join(self.__path, 'input')
        self.__path_input_plenoscope = os.path.join(self.__path_input, 'plenoscope')

        self.lixel_statistics = LixelStatistics(self.__path_input_plenoscope)
        self.event_numbers = self.__event_numbers_in_run()

    def __event_numbers_in_run(self):
        files_in_run_folder = glob.glob(os.path.join(self.__path, '*'))
        events =[]
        for fi in files_in_run_folder:
            if os.path.isdir(fi) and os.path.basename(fi).isdigit():
                events.append(int(os.path.basename(fi)))
        events = np.array(events)
        events.sort()
        return events

    def __getitem__(self, index):
        try:
            event_path = os.path.join(self.__path, str(index+1))
            return Event(event_path, self.lixel_statistics)
        except(FileNotFoundError):
            raise StopIteration