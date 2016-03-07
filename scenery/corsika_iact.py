#!/usr/bin/env python
"""
Creates a temporary working directory for corsika in the output path. 
Removes the writeprotection from the corsika iact output file.
Saves the stdout and stderr of corsika in the output path when corsika 
return code != 0 or when option -s is set.

Usage: corsika_iact -c=CORSIKA_PATH -i=INPUT_CARD_PATH [-s]

Options:
    -c --corsika_path=CORSIKA_PATH         path to corsika executable
    -i --input_card_path=INPUT_CARD_PATH   path to corsika input card
    -s --save_stdout                       will save the std- and stderr in any case 
"""
from __future__ import absolute_import, print_function, division
import docopt
import os
import glob
import subprocess
from subprocess import call

def all_files_in(path):
    return glob.glob(path+'*')

def mkdir(path):
    call(['mkdir', path])

def rm_dir(path):
    call(['rm', '-r', path])

def symlink(src, dest):
    call(['ln', '-s', src, dest])

class Path:
    def __init__(self, path):
        self.path = path
        self.file = os.path.split(self.path)[1]
        self.file_wo_extension = os.path.splitext(self.file)[0]
        self.base_path = os.path.split(self.path)[0]

def supposed_to_store_output_path(line):
    if line[0:6] == 'TELFIL':
        return True
    else:
        return False

def extract_output_path_from(line):
    first_quote = line.find('"')
    second_quote = line[first_quote+1:].find('"') + first_quote+1
    return line[first_quote+1:second_quote]

def output_path_taken_from_corsika_input_card(input_card_path):
    with open(input_card_path) as fileobject:
        for line in fileobject:
            if supposed_to_store_output_path(line):
                return extract_output_path_from(line)

def call_iact_corsika(corsika_path, input_card_path, save_stdout=False):
    
    corsika = Path(corsika_path)
    out = Path(output_path_taken_from_corsika_input_card(input_card_path))

    temp_working_dir = out.file_wo_extension+'_temp'
    
    if out.base_path+'/'+temp_working_dir not in all_files_in(out.base_path+'/'):
        mkdir(out.base_path+'/'+temp_working_dir)

        # symlink all Corsika relevant files to temp working dir
        for f in all_files_in(corsika.base_path+'/'):
            ff = Path(f)
            symlink(
                ff.base_path+'/'+ff.file, 
                out.base_path+'/'+temp_working_dir+'/'+ff.file
            )

        # symlink all IACT Corsika relevant files to temp working dir
        corsika_main_path = os.path.split(corsika.base_path)[0]
        corsika_iact_path = corsika_main_path+'/'+'bernlohr'
        
        for iact_path in all_files_in(corsika_iact_path+'/'):
            iact_file = Path(iact_path)
            if iact_file.file_wo_extension[0:7] == 'atmprof':
                symlink(
                    iact_file.base_path+'/'+iact_file.file, 
                    out.base_path+'/'+temp_working_dir+'/'+iact_file.file
                )

        # call corsika in the temporary working diractory and 
        # pipe the stdout and stderr into files
        stdout_path = out.base_path+'/'+out.file_wo_extension+'_stdout.txt'
        stderr_path = out.base_path+'/'+out.file_wo_extension+'_stderr.txt'
        corsika_stdout = open(stdout_path, 'w')
        corsika_stderr = open(stderr_path, 'w')
        input_card_file = open(input_card_path)
        corsika_return_value = call(
            out.base_path+'/'+temp_working_dir+'/'+corsika.file, 
            stdin=input_card_file, 
            stdout=corsika_stdout, 
            stderr=corsika_stderr,
            cwd=out.base_path+'/'+temp_working_dir
        )
        input_card_file.close()
        corsika_stderr.close()
        corsika_stdout.close()

        # remove the temporary working directory
        rm_dir(out.base_path+'/'+temp_working_dir)

        if not(save_stdout or corsika_return_value != 0):
            call(['rm', stdout_path])
            call(['rm', stderr_path])

        # remove write protection from corsika output
        call(['chmod', '+w', out.base_path+'/'+out.file])

if __name__ == '__main__':

    try:
        arguments = docopt.docopt(__doc__)

        corsika_path = arguments['--corsika_path']
        print("co: ", corsika_path)

        input_card_path = arguments['--input_card_path']
        print("in: ", input_card_path)
        
        save_stdout = arguments['--save_stdout']

        call_iact_corsika(
            corsika_path=corsika_path, 
            input_card_path=input_card_path,
            save_stdout=save_stdout
        )  
    # Handle invalid options
    except docopt.DocoptExit as e:
        print(e.message)
