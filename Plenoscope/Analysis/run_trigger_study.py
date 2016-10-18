"""
Run Trigger study

Usage: run_trigger_study -i=STEERING_CARD_PATH -o=OUTPUT_PATH -c=CALIB_PATH -p=PROPCONFIG -n=NUMBER_RUNS -m=MCTPROPEXE [-s]

Options:
    -i --input_path=STEERING_CARD_PATH          Path to corsika steering card
                                                template.
    -o --output_path=OUTPUT_PATH                Directory to collect trigger 
                                                study results of rach run.
    -n --number_runs=NUMBER_RUNS
    -c --calib_plenoscope_path=CALIB_PATH 
    -p --propagation_config_path=PROPCONFIG_PATH
    -m --mctracer=MCTPROPEXE
    -s --save_stdout                            Saves stdout and stderr of 
                                                Corsika next to OUTPUT_PATH.
"""
import docopt
import numpy as np
import plenopy as pl
import corsika_wrapper as cw

from scoop import shared
from scoop import futures
import scoop as sc
from subprocess import call
import tempfile
import os
import json

def mctPlenoscopePropagation_path():
    return sc.shared.getConst('mctPlenoscopePropagation', timeout=5)


def mct_propagation_config_path():
    return sc.shared.getConst('mct_propagation_config', timeout=5)


def plenoscope_calibration_path():
    return sc.shared.getConst('plenoscope_calibration', timeout=5)


def analyse_plenoscope_response(plenoscope_response_path, output_path):
    run = pl.Run(plenoscope_response_path)
    trigger_info = []
    for event in run:
        trigger_info.append(pl.trigger_study.export_trigger_information(event))
    pl.trigger_study.write_dict_to_file(trigger_info, output_path)


def make_plenoscope_response(corsika_run_path, plenoscope_response_path, prng_seed):
    stdout = open(plenoscope_response_path+'.stdout.txt', 'w')
    stderr = open(plenoscope_response_path+'.stderr.txt', 'w')
    call([
        mctPlenoscopePropagation_path(),
        '-l', plenoscope_calibration_path(),
        '-c', mct_propagation_config_path(),
        '-i', corsika_run_path,
        '-o', plenoscope_response_path,
        '-r', str(prng_seed)],
        stdout=stdout,
        stderr=stderr)
    stderr.close()
    stdout.close()            


def trigger_study_run(steering):
    with tempfile.TemporaryDirectory() as temp_path:
        corsika_run_path = os.path.join(temp_path, 'corsika_run.evtio')
        plenoscope_response_path = os.path.join(temp_path, 'response.pleno')

        cw.corsika(
            steering_card=steering['steering_card'],
            output_path=corsika_run_path, 
            save_stdout=True)

        make_plenoscope_response(
            corsika_run_path=corsika_run_path,
            plenoscope_response_path=plenoscope_response_path,
            prng_seed=steering['mctracer_seed'])

        analyse_plenoscope_response(
            plenoscope_response_path=plenoscope_response_path,
            output_path=steering['output_path'])
    return True

if __name__ == '__main__':

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
        steering = []
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

            steering.append({
                'steering_card': dict2corsika(d),
                'output_path': os.path.join(output_path, str(int(run_number))+'.json'),
                'mctracer_seed': run_number
            })
        return steering

    try:
        arguments = docopt.docopt(__doc__)
        sc.shared.setConst(mctPlenoscopePropagation=arguments['--mctracer'])
        sc.shared.setConst(plenoscope_calibration=arguments['--calib_plenoscope_path'])
        sc.shared.setConst(mct_propagation_config=arguments['--propagation_config_path'])
        output_path=arguments['--output_path']

        steering = make_corsika_steering_cards(int(arguments['--number_runs']))
        results = list(sc.futures.map(trigger_study_run, steering))
    except docopt.DocoptExit as e:
        print(e)

