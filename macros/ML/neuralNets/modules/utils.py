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


def print_class_counts(y, name, **kwargs):
    """
    Prints the number of entries of a passed vector and the size of its contained classes.

    Usage example: "print_class_counts(y_train, 'training', background=0, signal=1)"
    """
    
    print('\nNumber of {} samples: {}'.format(name, y.shape[0]))

    for key, item in kwargs.iteritems():
        print('  Number of {}: {} ({:.2f} percent)'.format(
            key,
            y[y==item].shape[0],
            y[y==item].shape[0]*100/y.shape[0]))
