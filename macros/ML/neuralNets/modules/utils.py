import sys

import numpy as np


def calculate_pair_sample_weight(weight_1, weight_2):
    """
    Calculates pair weights from individual track weights.
    """
    
    print('Calculate sample weights....')
          
    return (weight_1 * weight_2).values.astype(np.float32)


def print_dict(dict):
    """
    Prints the contents of a dictionary to the terminal, line by line.
    """
    
    for item in dict:
        print('  {:20}: {}'.format(item, dict[item]))

