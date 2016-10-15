#!/usr/bin/env python
import numpy as np
import plenopy as pl
import corsika_wrapper as cw

from scoop import shared
from scoop import futures
import scoop as sc
from subprocess import call
import tempfile
import os

def mctPlenoscopePropagation_path():
    return sc.shared.getConst('mctPlenoscopePropagation', timeout=5)


def mct_propagation_config_path():
    return sc.shared.getConst('mct_propagation_config', timeout=5)


def plenoscope_calibration_path():
    return sc.shared.getConst('plenoscope_calibration', timeout=5)


def analyse_plenoscope_response(plenoscope_response_path):
    run = pl.Run(plenoscope_response_path)
    trigger_info = []
    for event in run:
        trigger_info.append(pl.trigger_studie.export_trigger_information(event))
    return trigger_info


def make_plenoscope_response(corsika_run_path, plenoscope_response_path):

    stdout = open(plenoscope_response_path+'.stdout.txt', 'w')
    stderr = open(plenoscope_response_path+'.stderr.txt', 'w')

    call([
        mctPlenoscopePropagation_path(),
        '-l', plenoscope_calibration_path(),
        '-c', mct_propagation_config_path(),
        '-i', corsika_run_path,
        '-o', plenoscope_response_path],
        stdout=stdout,
        stderr=stderr)

    stderr.close()
    stdout.close()            


def trigger_study_run(steering_card):
    with tempfile.TemporaryDirectory() as temp_path:
        corsika_run_path = os.path.join(temp_path, 'corsika_run.evtio')
        plenoscope_response_path = os.path.join(temp_path, 'response.pleno')

        cw.corsika(
            steering_card=steering_card,
            output_path=corsika_run_path, 
            save_stdout=True)

        make_plenoscope_response(
            corsika_run_path=corsika_run_path,
            plenoscope_response_path=plenoscope_response_path)

        trigger_info = analyse_plenoscope_response(plenoscope_response_path)

    return trigger_info

if __name__ == '__main__':

    sc.shared.setConst(mctPlenoscopePropagation='/home/sebastian/raytracing/build/mctPlenoscopePropagation')
    sc.shared.setConst(plenoscope_calibration='/home/sebastian/Desktop/demo_big/portal_calibration')
    sc.shared.setConst(mct_propagation_config='/home/sebastian/Desktop/demo_big/propagation_config.xml')

    def dict2corsika(d):
        steering_card = [
            'RUNNR    '+str(d['RUNNR'])+'\n',
            'EVTNR    1\n',
            'NSHOW    '+str(d['NSHOW'])+'\n',
            'PRMPAR   '+str(d['PRMPAR'])+'\n',
            'ESLOPE  '+str(d['ESLOPE'])+'\n',
            'ERANGE  '+str(d['ERANGE'][0])+' '+str(d['ERANGE'][1])+'\n',
            'THETAP  0.  0.\n',
            'PHIP    0.  360.\n',
            'SEED '+str(d['SEED'])+' 0 0 \n',
            'SEED '+str(d['SEED'])+' 0 0 \n',
            'OBSLEV  '+str(d['OBSLEV'])+'\n',
            'FIXCHI  0. \n',
            'MAGNET 1e-99 1e-99\n',
            'ELMFLG  T  T\n',
            'MAXPRT  1\n',
            'PAROUT  F F\n',
            'TELESCOPE  0. 0. 0. '+str(d['TELESCOPE_radius'])+'\n',
            'ATMOSPHERE 26 T\n',
            'CWAVLG 290 700\n',
            'CSCAT 1 '+str(d['CSCAT_radius'])+' 0'+'\n',
            'CERQEF F T F\n', # pde, atmo, mirror
            'CERSIZ 1\n',
            'CERFIL F\n',
            'TSTART T\n',
            'EXIT\n']
        return steering_card
   
    def make_corsika_steering_cards(number_of_runs=1):
        steering_cards = []
        for run_index in range(number_of_runs):
            run_number = run_index + 1
            d={}
            d['RUNNR'] = run_number
            d['NSHOW'] = 100
            d['PRMPAR'] = 1 #gamma
            d['ESLOPE'] = -2.6
            d['ERANGE'] = [0.5, 500.0]
            d['SEED'] = run_number
            d['OBSLEV'] = 5000e2
            d['TELESCOPE_radius'] = 55e2
            d['CSCAT_radius'] = 500e2

            steering_cards.append(dict2corsika(d))
        return steering_cards

    steering_cards = make_corsika_steering_cards(3)
    result = list(sc.futures.map(trigger_study_run, steering_cards))