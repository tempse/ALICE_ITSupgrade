import os
import sys

import root_numpy
import numpy as np
import pandas as pd


def load_data(filename, branches=None, start=None, stop=None, selection=None):
    """
    Loads data and returns pandas dataframes in form of train/val/test splits.
    """
    
    if not os.path.exists(filename):
        print('Error when loading the data. File %s is not a regular file.' % filename)
        sys.exit()

    print('\nReading file %s...' % filename)

    data = pd.DataFrame(root_numpy.root2array(filename,
                                              branches=branches,
                                              start=start,
                                              stop=stop,
                                              selection=selection))

    print('Imported data: %d rows, %d columns' % (data.shape[0], data.shape[1]))
    print('Imported data: branches = %s' % branches)
    print('Imported data: selection criteria = %s' % selection)

    return data


def get_branches(identifier):
    """
    Returns a list of branches according to the passed keyword argument.

    Supported values for the identifier variable: 'pairTree', 'singleTree'
    """

    branches = []

    if identifier == 'pairTree':
        
        branches = [
            'px1','py1','pz1',
            'px2','py2','pz2',
            'phiv',
            'mass',
            'sumz',
            'diffz',
            'opang',
            'nITS1',
            'nITS2',
            'nITSshared1',
            'nITSshared2',
            'nTPC1',
            'nTPC2',
            'DCAx1',
            'DCAy1',
            'DCAz1',
            'DCAx2',
            'DCAy2',
            'DCAz2',
            'ITSchi21',
            'ITSchi22',
            'TPCchi21',
            'TPCchi22',
            'pt1',
            'pt2',
            'eta1',
            'eta2',
            'phi1',
            'phi2',
            'PIDeff1',
            'PIDeff2',
            'IsRP',
            'IsConv',
            'generator',
            'firstMothersInfo1',
            'electronsWithHFMother'
        ]

    elif identifier == 'singleTree':

        branches = [
            'eta',
            'phi',
            'pt',
            'dcaX',
            'dcaY',
            'dcaZ',
            'particle.fPx',
            'particle.fPy',
            'particle.fPz',
            'nITS',
            'nTPC',
            'nITSshared',
            'ITSchi2',
            'TPCchi2',
            'pdgMother',
            'PIDeff',
            'generator',
            'firstMothersInfo',
            'electronsWithHFMother'
        ]

    else:
        print('Error: Could not get list of branches. ' \
              'Unknown identifier: {}'.format(identifier))
        sys.exit(1)

    return branches


def get_preprocessing_instructions(identifier):
    """
    Returns a dictionary with lists of column names containing info about how they should be preprocessed.

    Supported values for the identifier variable: 'pairTree', 'singleTree'
    """

    column_names = {}

    if identifier == 'pairTree':

        columns_to_standardScale = [
            #'px1', 'py1', 'pz1', 'px2', 'py2', 'pz2',
            #'sumz',
            #'diffz',
            #'DCAx1', 'DCAy1', 'DCAz1', 'DCAx2', 'DCAy2', 'DCAz2',
            #'eta1', 'eta2'
            'p',
            'pz_diff',
            'sumz',
            'diffz',
            'DCAx1', 'DCAy1', 'DCAz1', 'DCAx2', 'DCAy2', 'DCAz2',
            'eta1', 'eta2'
        
        ]
        
        columns_to_scale = [
            'phiv',
            'mass',
            'opang',
            'nTPC1', 'nTPC2',
            'ITSchi21', 'ITSchi22',
            'TPCchi21', 'TPCchi22',
            'pt1', 'pt2',
            'phi1', 'phi2'
        ]
        
        columns_to_skip = [
            'nITS1', 'nITS2',
            'nITSshared1', 'nITSshared2',
            'PIDeff1', 'PIDeff2',
            'IsRP', 'IsConv',
            'generator', 'firstMotherInfo1', 'electronsWithHFMother'
        ]

        column_names['standardScale'] = columns_to_standardScale
        column_names['scale'] = columns_to_scale
        column_names['skip'] = columns_to_skip

        
    elif identifier == 'singleTree':

        columns_to_standardScale = [
            'eta',
            'DCAx', 'DCAy', 'DCAz',
            'p',
            
        ]

        columns_to_scale = [
            'phi',
            'pt',
            'nTPC',
            'ITSchi2',
            'TPCchi2',
        ]

        columns_to_skip = [
            'nITS',
            'nITSshared',
            'pdgMother',
            'PIDeff',
            'generator', 'firstMotherInfo', 'electronsWithHFMother'
        ]

        column_names['standardScale'] = columns_to_standardScale
        column_names['scale'] = columns_to_scale
        column_names['skip'] = columns_to_skip

        
    else:
        print('Error: Could not get preprocessing instructions. ' \
              'Unknown identifier: {}'.format(identifier))
        sys.exit(1)

    return column_names
            
    
def engineer_features(data, identifier):
    """
    Engineers features from a passed dataframe, according to the passed keyword argument.

    Supported values for the identifier variable: 'pairTree', 'singleTree'
    Returns a pandas dataframe.
    """

    X = pd.DataFrame()

    if identifier == 'pairTree':

        X['p'] = np.sqrt((data['px1']+data['px2'])*(data['px1']+data['px2']) +
                         (data['py1']+data['py2'])*(data['py1']+data['py2']) +
                         (data['pz1']+data['pz2'])*(data['pz1']+data['pz2']))
        X['phiv'] = data['phiv'] - 1.57
        X['mass'] = data['mass']
        X['pz_diff'] = np.divide(data['pz1'], np.sqrt(data['px1']*data['px1'] + data['py1']*data['py1'])) - np.divide(data['pz2'], np.sqrt(data['px2']*data['px2'] + data['py2']*data['py2']))
        X['sumz'] = data['sumz']
        X['diffz'] = data['diffz'] - 1.57
        X['opang'] = data['opang']
        X['nITS1'] = data['nITS1']
        X['nITS2'] = data['nITS2']
        X['nITSshared1'] = data['nITSshared1']
        X['nITSshared2'] = data['nITSshared2']
        X['nTPC1'] = data['nTPC1']
        X['nTPC2'] = data['nTPC2']
        
        ## calculate angle between pair DCA vectors
        #temp_DCAprod = ((data['DCAx1']*data['DCAx2']) + (data['DCAy1']*data['DCAy2']) + (data['DCAz1']*data['DCAz2'])) / (np.sqrt(data['DCAx1']*data['DCAx1'] + data['DCAy1']*data['DCAy1'] + data['DCAz1']*data['DCAz1']) * np.sqrt(data['DCAx2']*data['DCAx2'] + data['DCAy2']*data['DCAy2'] + data['DCAz2']*data['DCAz2']))
        #temp_DCAprod[temp_DCAprod>1.] = 1. # removes overfloats due to numerical imprecision
        #temp_DCAprod[temp_DCAprod<-1.] = -1.
        #X['DCAangle'] = np.arccos(temp_DCAprod)
        #
        #X['DCAdiff'] = (data['DCAx1']-data['DCAx2'] + \
            #                     data['DCAy1']-data['DCAy2'] + \
            #                     data['DCAz1']-data['DCAz2'])
        #
        #X['DCAabs'] = (data['DCAx1']-data['DCAx2'])*(data['DCAx1']-data['DCAx2']) + \
            #              (data['DCAy1']-data['DCAy2'])*(data['DCAy1']-data['DCAy2']) + \
            #              (data['DCAz1']-data['DCAz2'])*(data['DCAz1']-data['DCAz2'])
        ##X['DCAfeat'] = 1/(data['DCAx1']-data['DCAx2']) + 1/(data['DCAy1']-data['DCAy2']) + 1/(data['DCAz1']-data['DCAz2'])
        X['DCAx1'] = np.abs(data['DCAx1'])
        X['DCAx2'] = np.abs(data['DCAx2'])
        X['DCAy1'] = np.abs(data['DCAy1'])
        X['DCAy2'] = np.abs(data['DCAy2'])
        X['DCAz1'] = np.abs(data['DCAz1'])
        X['DCAz2'] = np.abs(data['DCAz2'])
        
        #X['DCAxy1'] = np.log(np.abs(data['DCAxy1_norm']))
        #X['DCAxy2'] = np.log(np.abs(data['DCAxy2_norm']))
        #X['DCAz1'] = np.abs(data['DCAz1'])
        #X['DCAz2'] = np.abs(data['DCAz2'])
        X['ITSchi21'] = data['ITSchi21']
        X['ITSchi22'] = data['ITSchi22']
        X['TPCchi21'] = data['TPCchi21']
        X['TPCchi22'] = data['TPCchi22']
        X['pt1'] = data['pt1']
        X['pt2'] = data['pt2']
        X['eta1'] = data['eta1']
        X['eta2'] = data['eta2']
        X['phi1'] = data['phi1']
        X['phi2'] = data['phi2']

        
    elif identifier == 'singleTree':

        X['eta'] = data['eta']
        X['phi'] = data['phi']
        X['pt'] = data['pt']
        X['dcaX'] = data['dcaX']
        X['dcaY'] = data['dcaY']
        X['dcaZ'] = data['dcaZ']
        X['p'] = np.sqrt(data['particle.fPx']*data['particle.fPx'] + \
                         data['particle.fPy']*data['particle.fPy'] + \
                         data['particle.fPz']*data['particle.fPz'])
        X['nITS'] = data['nITS']
        X['nTPC'] = data['nTPC']
        X['nITSshared'] = data['nITSshared']
        X['ITSchi2'] = data['ITSchi2']
        X['TPCchi2'] = data['TPCchi2']


    else:
        print('Error: Could not engineer features. ' \
              'Unknown identifier: {}'.format(identifier))
        sys.exit(1)

    return X
