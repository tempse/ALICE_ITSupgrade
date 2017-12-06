import sys

import numpy as np

from sklearn.externals import joblib
from sklearn.preprocessing import StandardScaler

from modules.load_data import get_preprocessing_instructions
from modules.get_output_paths import get_output_paths


def preprocess(X, entry_structure, load_fitted_attributes=False):
    """
    Performs preprocessing steps on the data. Handles pandas dataframes.

    Supported values for the variable 'entry_structure' are: 'pairTree', 'singleTree'
    """

    print('\nData preprocessing: Scaling to zero mean and unit variance')

    output_prefix = get_output_paths()[0]
    
    # get lists of column names for different kinds of preprocessing
    column_names = get_preprocessing_instructions(entry_structure)

    
    if not load_fitted_attributes:

        print('  Fitting new scaling attributes...')
        
        scaling_attributes = {}
    
        for column in column_names['standardScale']:
            try:
                mean = X[column].mean()
                std = X[column].std()
                X[column] -= mean
                X[column] /= std
            except KeyError:
                print('  Warning: Trying to preprocess feature {}, ' \
                      'but cannot find it in the data...'.format(column))
                pass

            scaling_attributes[column] = {'mean': mean, 'std': std}

        for column in column_names['scale']:
            try:
                std = X[column].std()
                X[column] /= std
            except KeyError:
                print('  Warning: Trying to preprocess feature {}, ' \
                      'but cannot find it in the data...'.format(column))
                pass

            scaling_attributes[column] = {'std': std}

        np.save(output_prefix + 'scaling_attributes.npy', scaling_attributes)

    else:

        print('  Loading scaling attributes...')
        
        scaling_attributes = np.load(output_prefix + 'scaling_attributes.npy').item()

        for column in column_names['standardScale']:
            try:
                X[column] -= scaling_attributes[column]['mean']
                X[column] /= scaling_attributes[column]['std']
            except KeyError:
                print('  Warning: Trying to preprocess feature {}, ' \
                      'but cannot find it in the data...'.format(column))
                pass

        for column in column_names['scale']:
            try:
                X[column] /= scaling_attributes[column]['std']
            except KeyError:
                print('  Warning: Trying to preprocess feature {}, ' \
                      'but cannot find it in the data...'.format(column))
                pass
                

    return X
