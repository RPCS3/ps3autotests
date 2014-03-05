import os
import sys

def main():
    if len(sys.argv) <= 2:
        print "This script generates the .expected file from your PS3's debug logs."
        print ""
        print "Usage:   convert-ps3-output.py <input> <output>"
        print "Example: convert-ps3-output.py hello_world.log hello_world.expected"
        return False

    #Parse and check arguments
    inputFile  = sys.argv[1]
    outputFile = sys.argv[2]
    if not os.path.isfile(inputFile):
        print "[!] Input file does not exist"
        return False

    f = open(inputFile, 'rb')
    w = open(outputFile, 'wb')

    data = f.read()
    data = data[data.find("/app_home/"):]
    data = data[data.find("\x0D\x0A")+2:]
    data = data[:data.rindex("END LOG")-12]
    data = data.replace("\x0D\x0A", "\x0A")
    
    w.write(data)
    w.close()

if __name__ == "__main__":
    main()
