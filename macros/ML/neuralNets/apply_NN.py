from __future__ import division

import numpy as np
import pandas as pd
import root_numpy

from sklearn.preprocessing import scale
from sklearn.externals import joblib

from keras.models import load_model

from modules.get_output_paths import *
from modules.load_data import *

from train_NN import preprocess_data

if __name__ == '__main__':

    output_prefix, keras_models_prefix = get_output_paths()
    
    model_filename = output_prefix + keras_models_prefix + 'weights_final.hdf5'
    
    data_filename = '/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/workingData/FT2_AnalysisResults_Upgrade_DCAvec_PIDeffs_pairtree_us_part1_1-9-split_correctedPIDeffs.root'
    
    num_datasplits = 10
    
    scaler_attributes_filename = output_prefix + 'StandardScaler_attributes.pkl'
    
    predictions_filename = output_prefix + 'predictions_NN.root'
    
    branches_pairTree = [
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
        'phi2'
    ]
    
    # singleTree features
    branches_singleTree = [
        'eta',
        'phi',
        'pt',
        'dcaR',
        'dcaZ',
        'particle.fPx',
        'particle.fPy',
        'particle.fPz',
        'nITS',
        'nTPC',
        'nITSshared',
        'ITSchi2',
        'TPCchi2',
        'pdgMother'
    ]


    # determine the number of entries in the tree by importing a single branch
    num_entries = pd.DataFrame(root_numpy.root2array(data_filename, branches=branches_pairTree[0])).shape[0]


    print('Loading previously trained model...')
    clf = load_model(model_filename)


    Yscore = np.empty((0,1))

    num_steps = num_datasplits if (num_entries%num_datasplits)==0 else num_datasplits+1
    
    for split_step in range(num_steps):
        start = split_step*int(num_entries/(num_datasplits*1.0))
        stop = (split_step+1)*int(num_entries/(num_datasplits*1.0)) if (split_step+1)*int(num_entries/(num_datasplits*1.0))<=num_entries else num_entries

        print('\n------------------------- PART %d of %d -------------------------' % (split_step+1, num_datasplits))
        print('Reading file %s...' % data_filename)
        dataSample_orig = load_data(data_filename,
                                    branches=branches_pairTree,
                                    start=start,
                                    stop=stop)
    
    
        print('Engineering features...')
    
    
        # pairTree features
        Xapp = pd.DataFrame()
        Xapp['p'] = np.sqrt((dataSample_orig['px1']+dataSample_orig['px2'])*(dataSample_orig['px1']+dataSample_orig['px2']) +
                            (dataSample_orig['py1']+dataSample_orig['py2'])*(dataSample_orig['py1']+dataSample_orig['py2']) +
                            (dataSample_orig['pz1']+dataSample_orig['pz2'])*(dataSample_orig['pz1']+dataSample_orig['pz2']))
        Xapp['phiv'] = dataSample_orig['phiv'] - 1.57
        Xapp['mass'] = dataSample_orig['mass']
        Xapp['pz_diff'] = np.divide(dataSample_orig['pz1'], np.sqrt(dataSample_orig['px1']*dataSample_orig['px1'] + dataSample_orig['py1']*dataSample_orig['py1'])) - np.divide(dataSample_orig['pz2'], np.sqrt(dataSample_orig['px2']*dataSample_orig['px2'] + dataSample_orig['py2']*dataSample_orig['py2']))
        Xapp['sumz'] = dataSample_orig['sumz']
        Xapp['diffz'] = dataSample_orig['diffz'] - 1.57
        Xapp['opang'] = dataSample_orig['opang']
        Xapp['nITS1'] = dataSample_orig['nITS1']
        Xapp['nITS2'] = dataSample_orig['nITS2']
        Xapp['nITSshared1'] = dataSample_orig['nITSshared1']
        Xapp['nITSshared2'] = dataSample_orig['nITSshared2']
        Xapp['nTPC1'] = dataSample_orig['nTPC1']
        Xapp['nTPC2'] = dataSample_orig['nTPC2']
    
        ## calculate angle between pair DCA vectors
        #temp_DCAprod = ((dataSample_orig['DCAx1']*dataSample_orig['DCAx2']) + (dataSample_orig['DCAy1']*dataSample_orig['DCAy2']) + (dataSample_orig['DCAz1']*dataSample_orig['DCAz2'])) / (np.sqrt(dataSample_orig['DCAx1']*dataSample_orig['DCAx1'] + dataSample_orig['DCAy1']*dataSample_orig['DCAy1'] + dataSample_orig['DCAz1']*dataSample_orig['DCAz1']) * np.sqrt(dataSample_orig['DCAx2']*dataSample_orig['DCAx2'] + dataSample_orig['DCAy2']*dataSample_orig['DCAy2'] + dataSample_orig['DCAz2']*dataSample_orig['DCAz2']))
        #temp_DCAprod[temp_DCAprod>1.] = 1. # removes overfloats due to numerical imprecision
        #temp_DCAprod[temp_DCAprod<-1.] = -1.
        #Xapp['DCAangle'] = np.arccos(temp_DCAprod)
        #
        #Xapp['DCAdiff'] = (dataSample_orig['DCAx1']-dataSample_orig['DCAx2'] + \
            #                     dataSample_orig['DCAy1']-dataSample_orig['DCAy2'] + \
            #                     dataSample_orig['DCAz1']-dataSample_orig['DCAz2'])
        #
        #Xapp['DCAabs'] = (dataSample_orig['DCAx1']-dataSample_orig['DCAx2'])*(dataSample_orig['DCAx1']-dataSample_orig['DCAx2']) + \
            #              (dataSample_orig['DCAy1']-dataSample_orig['DCAy2'])*(dataSample_orig['DCAy1']-dataSample_orig['DCAy2']) + \
            #              (dataSample_orig['DCAz1']-dataSample_orig['DCAz2'])*(dataSample_orig['DCAz1']-dataSample_orig['DCAz2'])
        ##Xapp['DCAfeat'] = 1/(dataSample_orig['DCAx1']-dataSample_orig['DCAx2']) + 1/(dataSample_orig['DCAy1']-dataSample_orig['DCAy2']) + 1/(dataSample_orig['DCAz1']-dataSample_orig['DCAz2'])
        Xapp['DCAx1'] = np.abs(dataSample_orig['DCAx1'])
        Xapp['DCAx2'] = np.abs(dataSample_orig['DCAx2'])
        Xapp['DCAy1'] = np.abs(dataSample_orig['DCAy1'])
        Xapp['DCAy2'] = np.abs(dataSample_orig['DCAy2'])
        Xapp['DCAz1'] = np.abs(dataSample_orig['DCAz1'])
        Xapp['DCAz2'] = np.abs(dataSample_orig['DCAz2'])

        #Xapp['DCAxy1'] = np.log(np.abs(dataSample_orig['DCAxy1_norm']))
        #Xapp['DCAxy2'] = np.log(np.abs(dataSample_orig['DCAxy2_norm']))
        #Xapp['DCAz1'] = np.abs(dataSample_orig['DCAz1'])
        #Xapp['DCAz2'] = np.abs(dataSample_orig['DCAz2'])
        Xapp['ITSchi21'] = dataSample_orig['ITSchi21']
        Xapp['ITSchi22'] = dataSample_orig['ITSchi22']
        Xapp['TPCchi21'] = dataSample_orig['TPCchi21']
        Xapp['TPCchi22'] = dataSample_orig['TPCchi22']
        Xapp['pt1'] = dataSample_orig['pt1']
        Xapp['pt2'] = dataSample_orig['pt2']
        Xapp['eta1'] = dataSample_orig['eta1']
        Xapp['eta2'] = dataSample_orig['eta2']
        Xapp['phi1'] = dataSample_orig['phi1']
        Xapp['phi2'] = dataSample_orig['phi2']

        """
        # singleTree features
        Xapp = pd.DataFrame()
        Xapp['eta'] = dataSample_orig['eta']
        Xapp['phi'] = dataSample_orig['phi']
        Xapp['pt'] = dataSample_orig['pt']
        Xapp['dcaR'] = dataSample_orig['dcaR']
        Xapp['dcaZ'] = dataSample_orig['dcaZ']
        Xapp['p'] = np.sqrt(dataSample_orig['particle.fPx']*dataSample_orig['particle.fPx'] + \
        dataSample_orig['particle.fPy']*dataSample_orig['particle.fPy'] + \
        dataSample_orig['particle.fPz']*dataSample_orig['particle.fPz'])
        Xapp['nITS'] = dataSample_orig['nITS']
        Xapp['nTPC'] = dataSample_orig['nTPC']
        Xapp['nITSshared'] = dataSample_orig['nITSshared']
        Xapp['ITSchi2'] = dataSample_orig['ITSchi2']
        Xapp['TPCchi2'] = dataSample_orig['TPCchi2']
        """

        # data preprocessing
        Xapp = preprocess_data(Xapp, load_fitted_attributes=True, output_prefix=output_prefix)
        
        print('Applying the trained classifier...')
        Yscore_chunk = clf.predict_proba(Xapp.as_matrix()).astype(np.float32)
    
        """
        n_chunks = 20
        Yscore_chunk = np.empty((0,1), dtype=np.float32)
        for i in range(0, n_chunks if (Xapp.shape[0]%n_chunks==0) else n_chunks+1):
        start = i*int(Xapp.shape[0]/(n_chunks*1.0))
        stop = (i+1)*int(Xapp.shape[0]/(n_chunks*1.0)) if (i+1)*int(Xapp.shape[0]/(n_chunks*1.0))<Xapp.shape[0] else Xapp.shape[0]
        if doScale:
        Xapp.iloc[start:stop,:] -= scaler_mean
        Xapp.iloc[start:stop,:] /= scaler_scale
        print(Yscore_chunk.shape, Xapp.iloc[start:stop,:].shape, clf.predict_proba(Xapp.iloc[start:stop,:].as_matrix()).shape)
        Yscore_chunk = np.concatenate((Yscore_chunk,
        clf.predict_proba(Xapp.iloc[start:stop,:].as_matrix()).astype(np.float32)))
        if i<n_chunks:
        print('%d%% done...' % int((i+1)*100/(n_chunks*1.0)))
        """

        print('Tagging entries outside the training scope as untrustworthily classified...')
        Yscore_chunk[np.where(dataSample_orig['mass']<0.05)] = 99
        
        Yscore = np.concatenate((Yscore, Yscore_chunk))
        print('\n')


    # export predictions
    print('\nExporting predictions to file %s...' % predictions_filename)
    Yscore = np.array(Yscore, dtype=[('NN', np.float32)])

    root_numpy.array2root(Yscore,
                          predictions_filename,
                          treename='pairTree_MVAoutput',
                          mode='recreate')
