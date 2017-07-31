import sys

from modules.get_output_paths import *


class logger(object):
    """
    Writes output both to terminal and to file.
    """
    
    def __init__(self):
        self.terminal = sys.stdout
        output_prefix = get_output_paths()[0]
        self.log = open(output_prefix + 'stdout.log', 'w')

    def write(self, message):
        self.terminal.write(message)
        self.log.write(message)

    def flush(self):
        #this flush method is needed for python 3 compatibility.
        #this handles the flush command by doing nothing.
        #you might want to specify some extra behavior here.
        pass
