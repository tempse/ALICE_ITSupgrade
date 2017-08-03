import os
import sys

import root_numpy
import pandas as pd


def load_data(filename, branches=None, start=None, stop=None, selection=None):
    """
    Loads data and returns pandas dataframes in form of train/val/test splits.
    """
    
    if not os.path.exists(filename):
        print('Error when loading the data. File %s is not a regular file.' % filename)
        sys.exit()

    print('Reading file %s...' % filename)

    data = pd.DataFrame(root_numpy.root2array(filename,
                                              branches=branches,
                                              start=start,
                                              stop=stop,
                                              selection=selection))

    print('Imported data: %d rows, %d columns' % (data.shape[0], data.shape[1]))
    print('Imported data: branches = %s' % branches)
    print('Imported data: selection criteria = %s' % selection)

    return data
