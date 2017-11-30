import sys

import numpy as np


def calculate_pair_sample_weight(weight_1, weight_2):
    """
    Calculates pair weights from individual track weights.
    """
    
    print('\nCalculate sample weights....')
          
    return (weight_1 * weight_2).values.astype(np.float32)


def print_dict(dict, headline=None):
    """
    Prints the contents of a dictionary to the terminal, line by line.
    """

    if headline:
        print('\n{}'.format(headline))
    
    for item in dict:
        print('  {:20}: {}'.format(item, dict[item]))


def print_class_counts(y, name, **kwargs):
    """
    Prints the number of entries of a passed vector and the size of its contained classes.

    Usage example: "print_class_counts(y_train, 'training', background=0, signal=1)"
    """
    
    print('\nNumber of {} samples: {}'.format(name, y.shape[0]))

    class_list = []

    try:
        class_list = kwargs.iteritems()
    except AttributeError:
        # iteritems() does not work in Python 3
        pass

    try:
        class_list = list(kwargs.items())
    except:
        print('Error in function print_class_counts: ' \
              'Could not iterate over the passed items. ' \
              '(Maybe a Python version conflict?)')
        print('\n Skip printing class counts.')
        
        return
        
        
    for key, item in class_list:
        print('  Number of {}: {} ({:.2f} percent)'.format(
            key,
            y[y==item].shape[0],
            y[y==item].shape[0]*100/y.shape[0]))
