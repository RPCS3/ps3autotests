#!/usr/bin/python

import os
import ntpath
import sys
import subprocess
import signal
from difflib import Differ

# Constants
OUTPUT_STDOUT = 0 # All output is obtained through stdout. Isn't used by rpcs3.
OUTPUT_FILE   = 1 # The test generates a file called 'output.txt'
OUTPUT_TTYLOG = 2 # rpcs3 saves ps3 stdout into TTY.log near executable

COMPARE_BIN   = 0 # Output is validated byte per byte
COMPARE_TEXT  = 1 # Output is validated as text, replacing CRLF with LF
COMPARE_SMART = 2 # Output is validated following rules specified below

# Absolute path to this script
scriptFolder = os.path.dirname(os.path.realpath(__file__))

# List of tests and benchmarks
autotests = (
    ('tests/cpu/basic',                   OUTPUT_TTYLOG,  COMPARE_TEXT),
    ('tests/cpu/ppu_branch',              OUTPUT_FILE,    COMPARE_TEXT, 'output.txt'),
    ('tests/cpu/ppu_float_arithmetic',    OUTPUT_FILE,    COMPARE_TEXT, 'output.txt'),
    ('tests/cpu/ppu_float_compare',       OUTPUT_FILE,    COMPARE_TEXT, 'output.txt'),
    ('tests/cpu/ppu_float_conversion',    OUTPUT_FILE,    COMPARE_TEXT, 'output.txt'),
    ('tests/cpu/ppu_float_load',          OUTPUT_FILE,    COMPARE_TEXT, 'output.txt'),
    ('tests/cpu/ppu_float_store',         OUTPUT_FILE,    COMPARE_TEXT, 'output.txt'),
 #   ('tests/cpu/ppu_gpr',                 OUTPUT_TTYLOG,  COMPARE_TEXT), takes too long
    ('tests/cpu/ppu_integer_arithmetic',  OUTPUT_FILE,    COMPARE_TEXT, 'output.txt'),
    ('tests/cpu/ppu_integer_compare',     OUTPUT_FILE,    COMPARE_TEXT, 'output.txt'),
    ('tests/cpu/ppu_integer_logical',     OUTPUT_FILE,    COMPARE_TEXT, 'output.txt'),
    ('tests/cpu/ppu_integer_rotate',      OUTPUT_FILE,    COMPARE_TEXT, 'output.txt'),
    ('tests/cpu/ppu_integer_shift',       OUTPUT_FILE,    COMPARE_TEXT, 'output.txt'),
    ('tests/cpu/ppu_vector_integer_arithmetic', OUTPUT_FILE, COMPARE_TEXT, 'output.txt'),
    ('tests/cpu/ppu_vpu',                 OUTPUT_TTYLOG,  COMPARE_TEXT, 'output.txt'),
    ('tests/cpu/spu_alu',                 OUTPUT_TTYLOG,  COMPARE_TEXT, 'output.txt'), #.expected isn't correct
    ('tests/cpu/spu_fpu',                 OUTPUT_TTYLOG,  COMPARE_TEXT, 'output.txt'), #.expected isn't correct
    ('tests/cpu/spu_generic',             OUTPUT_TTYLOG,  COMPARE_TEXT, 'output.txt'),
    ('tests/lv2/sys_event_flag',          OUTPUT_TTYLOG,  COMPARE_TEXT, 'output.txt'),
    ('tests/lv2/sys_process',             OUTPUT_TTYLOG,  COMPARE_TEXT, 'output.txt'),
    ('tests/lv2/sys_semaphore',           OUTPUT_TTYLOG,  COMPARE_TEXT, 'output.txt'),
)

def signal_handler(sig, frame):
    print("User aborted testing!!")
    os._exit(1)

# we'll register for the CTRL-C 'abort' signal
signal.signal(signal.SIGINT, signal_handler)

# Main
def runTests(emulator, baseDir):
    errors = False

    # Run tests
    for test in autotests:
    
        # Parameters
        relProjectFolder = test[0]
        absProjectFolder = os.path.join(scriptFolder, relProjectFolder)
        projectName = ntpath.basename(relProjectFolder)
        elfPath = os.path.join(absProjectFolder, projectName) + '.ppu.self'
        if (not os.path.isfile(elfPath)):
            elfPath = os.path.join(absProjectFolder, projectName) + '.ppu.elf'
        expectedPath = os.path.join(absProjectFolder, projectName) + '.expected'
        outputMethod = test[1]
        compareMethod = test[2]
        if outputMethod == OUTPUT_FILE:
            outputPath = os.path.join(absProjectFolder, test[3])
        ttyPath = os.path.join(os.path.dirname(emulator),"TTY.log")

        # Command & Expected file
        cmd = emulator + ' --no-gui ' + '"' + os.path.join(baseDir, elfPath) + '"'
        if os.path.exists(os.path.join(baseDir, expectedPath)):
            if compareMethod == COMPARE_TEXT:
                expected = open(os.path.join(baseDir, expectedPath), 'r')
            else:
                expected = open(os.path.join(baseDir, expectedPath), 'rb')

        try:
            # Get output
            if outputMethod == OUTPUT_STDOUT:
                rpcs3 = subprocess.run(cmd, timeout=60) #we'll give each command 1minute to execute before killing it
            if outputMethod == OUTPUT_FILE:
                rpcs3 = subprocess.run(cmd, timeout=60) #we'll give each command 1minute to execute before killing it
                if compareMethod == COMPARE_TEXT:
                    result = open(outputPath, 'r').read().splitlines()
                else:
                    result = open(outputPath, 'rb').read()
            if outputMethod == OUTPUT_TTYLOG:
                rpcs3 = subprocess.run(cmd, timeout=60) #we'll give each command 1minute to execute before killing it
                result = open(ttyPath, 'r').read().splitlines()
                
            # Compare output
            if compareMethod == COMPARE_TEXT:
                expected = expected.read().splitlines()
                if result == expected:
                    print (" - Success:\t\t", relProjectFolder)
                    # Delete output file only on success
                    if outputMethod == OUTPUT_FILE:
                        os.remove(outputPath)
                else:
                    print (" - Error:\t\t", relProjectFolder)
                    errors = True
                    
                    d = Differ()
                    diff = d.compare(expected, result)
                    #for line in diff:  #python appears to completely lock up here under v3.8.5
                    #    print(line)

            
                
        except Exception as e:
            print (" - Error:\t\t", relProjectFolder, " - ", sys.exc_info()[0])
            
            if type(e) is subprocess.TimeoutExpired:
                #we've given rpcs3 the command to die... but we should give it a little more time
                sleep(5)
            errors = True
        

    # Return errors
    if errors:
        print ("Some tests failed!")
        sys.exit(1)
    else:
        print ("All tests succeeded!")
        sys.exit(0)


if __name__ == '__main__':
    if len(sys.argv) <= 1:
        print ('PS3 Autotests: Tests, benchmarks and demos for emulators')
        print ('Usage: run-tests.py [arguments] path/to/emulator.exe')
        print ('Arguments: (none available)')
    else:
        baseDir = os.path.dirname(os.path.abspath(__file__)).replace('\\', '/')
        emulator = sys.argv[len(sys.argv) - 1]
        runTests(emulator, baseDir)
