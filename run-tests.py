#!/usr/bin/python

import os
import ntpath
import sys
import subprocess

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
    ('tests/cpu/ppu_branch',              OUTPUT_FILE,    COMPARE_TEXT),
    ('tests/cpu/ppu_float_arithmetic',    OUTPUT_FILE,    COMPARE_TEXT),
    ('tests/cpu/ppu_float_compare',       OUTPUT_FILE,    COMPARE_TEXT),
    ('tests/cpu/ppu_float_conversion',    OUTPUT_FILE,    COMPARE_TEXT),
    ('tests/cpu/ppu_float_load',          OUTPUT_FILE,    COMPARE_TEXT),
    ('tests/cpu/ppu_float_store',         OUTPUT_FILE,    COMPARE_TEXT),
 #   ('tests/cpu/ppu_gpr',                 OUTPUT_TTYLOG,  COMPARE_TEXT), takes too long
    ('tests/cpu/ppu_integer_arithmetic',  OUTPUT_FILE,    COMPARE_TEXT),
    ('tests/cpu/ppu_integer_compare',     OUTPUT_FILE,    COMPARE_TEXT),
    ('tests/cpu/ppu_integer_logical',     OUTPUT_FILE,    COMPARE_TEXT),
    ('tests/cpu/ppu_integer_rotate',      OUTPUT_FILE,    COMPARE_TEXT),
    ('tests/cpu/ppu_integer_shift',       OUTPUT_FILE,    COMPARE_TEXT),
    ('tests/cpu/ppu_vector_integer_arithmetic', OUTPUT_FILE, COMPARE_TEXT),
#    ('tests/cpu/ppu_vpu',                 OUTPUT_TTYLOG,  COMPARE_TEXT),
#    ('tests/cpu/spu_alu',                 OUTPUT_TTYLOG,  COMPARE_TEXT), .expected isn't correct
#    ('tests/cpu/spu_fpu',                 OUTPUT_TTYLOG,  COMPARE_TEXT), .expected isn't correct
#    ('tests/cpu/spu_generic',             OUTPUT_TTYLOG,  COMPARE_TEXT),
    ('tests/lv2/sys_event_flag',          OUTPUT_TTYLOG,  COMPARE_TEXT),
    ('tests/lv2/sys_process',             OUTPUT_TTYLOG,  COMPARE_TEXT),
    ('tests/lv2/sys_semaphore',           OUTPUT_TTYLOG,  COMPARE_TEXT),
)

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
        outputPath = os.path.join(absProjectFolder, 'output.txt')
        ttyPath = os.path.join(os.path.dirname(emulator),"TTY.log")
        outputMethod = test[1]
        compareMethod = test[2]

        # Command & Expected file
        cmd = emulator + ' ' + os.path.join(baseDir, elfPath)
        expected = open(os.path.join(baseDir, expectedPath), 'rb')

        # Get output
        if outputMethod == OUTPUT_STDOUT:
            result = subprocess.check_output(cmd, shell=True)
        if outputMethod == OUTPUT_FILE:
            subprocess.check_output(cmd, shell=True);
            result = open(outputPath, 'rb').read()
        if outputMethod == OUTPUT_TTYLOG:
            subprocess.check_output(cmd, shell=True);
            result = open(ttyPath, 'rb').read()

        # Compare output
        if compareMethod == COMPARE_TEXT:
            result = result.replace('\r\n', '\n')
            if result == expected.read():
                print " - Success: ", relProjectFolder
            else:
                print " - Error:   ", relProjectFolder
                errors = True

        # Delete output file if necessary
        if outputMethod == OUTPUT_FILE:
            os.remove(outputPath)

    # Return errors
    if errors:
        print "Some tests failed!"
        exit(1)
    else:
        print "All tests succeeded!"
        exit(0)


if __name__ == '__main__':
    if len(sys.argv) <= 1:
        print 'PS3 Autotests: Tests, benchmarks and demos for emulators'
        print 'Usage: run-tests.py [arguments] path/to/emulator.exe'
        print 'Arguments: (none available)'
    else:
        baseDir = os.path.dirname(os.path.abspath(__file__)).replace('\\', '/')
        emulator = sys.argv[len(sys.argv) - 1]
        runTests(emulator, baseDir)
