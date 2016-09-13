#!/usr/bin/env python
from __future__ import absolute_import, print_function, division

from multiprocessing.pool import ThreadPool
from subprocess import call

corsika_path = '/home/relleums/corsika/corsika-75000/run/corsika75000Linux_QGSII_urqmd'
output_path  = '/data/relleums/lebowsky/20160726'
#output_path  = '/home/sebastian/Desktop/mctout'
#corsika_path = '/home/sebastian/corsika/corsika-74005/run/corsika74005Linux_QGSII_urqmd'


def corsika_ID_given_A_Z(A, Z):
    if A < 2 or A > 56:
        raise
    return A*100+Z

def dict2corsika(d):
    ic = 'RUNNR    '+str(d['RUNNR'])+'\n'
    ic+= 'EVTNR    1\n'
    ic+= 'NSHOW    '+str(d['NSHOW'])+'\n'
    ic+= 'PRMPAR   '+str(d['PRMPAR'])+'\n'
    ic+= 'ESLOPE  '+str(d['ESLOPE'])+'\n'
    ic+= 'ERANGE  '+str(d['ERANGE'][0])+' '+str(d['ERANGE'][1])+'\n'
    ic+= 'THETAP  0.  0.\n'
    ic+= 'PHIP    0.  360.\n'
    ic+= 'SEED '+str(d['SEED'])+' 0 0 \n'
    ic+= 'SEED '+str(d['SEED'])+' 0 0 \n'
    ic+= 'OBSLEV  '+str(d['OBSLEV'])+'\n'
    ic+= 'FIXCHI  0. \n'
    ic+= 'MAGNET 30.3 24.1\n'
    ic+= 'ELMFLG  F   T\n'
    ic+= 'MAXPRT  1\n'
    ic+= 'TELFIL  \"'+str(d['TELFIL'])+'\"\n'
    ic+= 'PAROUT  F F\n'
    ic+= 'TELESCOPE  0. 0. 0. '+str(d['TELESCOPE_radius'])+'\n'
    ic+= 'ATMOSPHERE 6 T\n'
    ic+= 'CWAVLG 290 700\n'
    ic+= 'CSCAT 1 '+str(d['CSCAT_radius'])+' 0'+'\n'
    ic+= 'TSTART T\n' #  needed for primary cherenkov emission
    ic+= 'CERSIZ 1\n'
    ic+= 'CERFIL F\n'
    ic+= 'EXIT\n'
    return ic

particles = {
    'gamma':{'id': 1, 'runs':100, 'ESLOPE': 0.0, 'Emin': 1, 'Emax': 250},
    'electron':{'id': 3, 'runs':10, 'ESLOPE': 0.0, 'Emin': 1, 'Emax': 250},
    'proton':{'id': 14, 'runs':10, 'ESLOPE': 0.0, 'Emin': 1, 'Emax': 1000},
    'Fe':{'id': corsika_ID_given_A_Z(56,26), 'runs':10, 'ESLOPE': 0.0, 'Emin': 500, 'Emax': 10000},
    #'He':{'id': corsika_ID_given_A_Z(4,2), 'runs':10, 'ESLOPE': 0.0, 'Emin': 500, 'Emax': 5000},
    #'Li':{'id': corsika_ID_given_A_Z(7,3), 'runs':10, 'ESLOPE': 0.0, 'Emin': 500, 'Emax': 5000},
}

thread_limit = 8

input_card_paths = []
commands = []
corsika_run = 0

for par in particles:
    for run in range(particles[par]['runs']):

        corsika_run = corsika_run + 1
        name = par+str(run+1)+'_RUN'+str(corsika_run)

        input_card_name = name+'.txt'
        output_name = name+'.dat'
        input_card_path = output_path+'/'+input_card_name

        d={}
        d['RUNNR'] = corsika_run
        d['NSHOW'] = 100
        d['PRMPAR'] = particles[par]['id']
        d['ESLOPE'] = particles[par]['ESLOPE']
        d['ERANGE'] = [particles[par]['Emin'], particles[par]['Emax']]
        d['SEED'] = corsika_run
        d['OBSLEV'] = 5000e2
        d['TELFIL'] = output_path+'/'+output_name
        d['TELESCOPE_radius'] = 55e2
        d['CSCAT_radius'] = 250e2

        input_card = dict2corsika(d)

        f = open(input_card_path,"w")
        f.write(input_card)
        f.close() 

        commands.append(['corsika_iact', '-i', input_card_path, '-c', corsika_path, '-s'])

def run(cmd):
    return cmd, call(cmd)

for cmd, rc in ThreadPool(thread_limit).imap_unordered(run, commands):
    pass


    def createToDoList():
        files = os.listdir(simulation_directory())
        input_card_paths = []
        output_files = []
        for f in files:
            if f[-4:] == '.txt':
                input_card_paths.append(f[:-4])
            elif f[-4:] == '.dat': 
                output_files.append(f[:-4])
            else:
                raise
        input_cards_to_do = []
        for inp in input_card_paths:
            if inp not in output_files:
                input_cards_to_do.append(inp+'.txt')
        return input_card_to_do


