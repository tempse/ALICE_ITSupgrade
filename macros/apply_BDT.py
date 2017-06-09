from __future__ import division

import numpy as np
import pandas as pd
import root_numpy

from sklearn.preprocessing import scale
from sklearn.externals import joblib


weights_filename = '/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/sklearn_BDT_analysis/randomForest/plots_sampleWeights/clf_weights.pkl'

data_filename = '/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/inputData/FT2_AnalysisResults_Upgrade_DCAvec_PIDeffs_pairtree_us_part1_1-9-split.root'

branches = [
    #'px1','py1','pz1',
    #'px2','py2','pz2',
    #'phiv',
    'mass',
    #'sumz',
    #'diffz',
    #'opang',
    #'nITS1',
    #'nITS2',
    'nITSshared1',
    'nITSshared2',
    #'nTPC1',
    'nTPC2',
    #'DCAx1','DCAy1',
    'DCAz1',
    #'DCAx2','DCAy2',
    'DCAz2',
    'DCAxy1_norm','DCAxy2_norm',
    #'DCAz1_norm','DCAz2_norm',
    #'ITSchi21',
    'ITSchi22',
    #'TPCchi21',
    'TPCchi22',
    'pt1',
    'pt2',
    #'eta1',
    'eta2',
    #'phi1',
    'phi2'
]

print("Reading file %s..." % data_filename)
dataSample_orig = pd.DataFrame(root_numpy.root2array(data_filename,
                                                     branches=branches))

#print('Setting initial mass cuts...')
#dataSample_orig = dataSample_orig.drop(dataSample_orig[dataSample_orig['mass']<.05].index)


print('Engineering features...')

Xapp = pd.DataFrame()
###Xapp['p'] = np.sqrt((dataSample_orig['px1']+dataSample_orig['px2'])*(dataSample_orig['px1']+dataSample_orig['px2']) +
###                          (dataSample_orig['py1']+dataSample_orig['py2'])*(dataSample_orig['py1']+dataSample_orig['py2']) +
###                          (dataSample_orig['pz1']+dataSample_orig['pz2'])*(dataSample_orig['pz1']+dataSample_orig['pz2']))
###Xapp['phiv'] = dataSample_orig['phiv'] - 1.57
###Xapp['mass'] = dataSample_orig['mass']
###Xapp['pz_diff'] = np.divide(dataSample_orig['pz1'], np.sqrt(dataSample_orig['px1']*dataSample_orig['px1'] + dataSample_orig['py1']*dataSample_orig['py1'])) - np.divide(dataSample_orig['pz2'], np.sqrt(dataSample_orig['px2']*dataSample_orig['px2'] + dataSample_orig['py2']*dataSample_orig['py2']))
###Xapp['sumz'] = dataSample_orig['sumz']
###Xapp['diffz'] = dataSample_orig['diffz'] - 1.57
###Xapp['opang'] = dataSample_orig['opang']
###Xapp['nITS1'] = dataSample_orig['nITS1']
###Xapp['nITS2'] = dataSample_orig['nITS2']
Xapp['nITSshared1'] = dataSample_orig['nITSshared1']
Xapp['nITSshared2'] = dataSample_orig['nITSshared2']
###Xapp['nTPC1'] = dataSample_orig['nTPC1']
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
#Xapp['DCAx1'] = np.abs(dataSample_orig['DCAx1'])
#Xapp['DCAx2'] = np.abs(dataSample_orig['DCAx2'])
#Xapp['DCAy1'] = np.abs(dataSample_orig['DCAy1'])
#Xapp['DCAy2'] = np.abs(dataSample_orig['DCAy2'])
#Xapp['DCAz1'] = np.abs(dataSample_orig['DCAz1'])
#Xapp['DCAz2'] = np.abs(dataSample_orig['DCAz2'])

Xapp['DCAxy1'] = np.log(np.abs(dataSample_orig['DCAxy1_norm']))
Xapp['DCAxy2'] = np.log(np.abs(dataSample_orig['DCAxy2_norm']))
Xapp['DCAz1'] = np.abs(dataSample_orig['DCAz1'])
Xapp['DCAz2'] = np.abs(dataSample_orig['DCAz2'])
###Xapp['ITSchi21'] = dataSample_orig['ITSchi21']
Xapp['ITSchi22'] = dataSample_orig['ITSchi22']
###Xapp['TPCchi21'] = dataSample_orig['TPCchi21']
Xapp['TPCchi22'] = dataSample_orig['TPCchi22']
Xapp['pt1'] = dataSample_orig['pt1']
Xapp['pt2'] = dataSample_orig['pt2']
###Xapp['eta1'] = dataSample_orig['eta1']
Xapp['eta2'] = dataSample_orig['eta2']
###Xapp['phi1'] = dataSample_orig['phi1']
Xapp['phi2'] = dataSample_orig['phi2']


# data preprocessing
doScale = True
if doScale:
    print('Scaling features to zero mean and unit variance...')
    Xapp = scale(Xapp)


print('Loading previously trained weights...')
clf = joblib.load(weights_filename)


print('Applying the trained classifier...')
n_chunks = 100
Yscore = np.empty((0,2), dtype=np.float32)
for i in range(0, n_chunks if (Xapp.shape[0]%n_chunks==0) else n_chunks+1):
    start = i*int(Xapp.shape[0]/(n_chunks*1.0))
    stop = (i+1)*int(Xapp.shape[0]/(n_chunks*1.0)) if (i+1)*int(Xapp.shape[0]/(n_chunks*1.0))<Xapp.shape[0] else Xapp.shape[0]
    Yscore = np.concatenate((Yscore,
                             clf.predict_proba(Xapp[start:stop,:]).astype(np.float32)))
    if i<n_chunks:
        print('%d%% done...' % int((i+1)*100/(n_chunks*1.0)))

        
# tag entries outside the training scope as untrustworthily classified
Yscore[np.where(dataSample_orig['mass']<.05),:] = 999


# export predictions
Yscore = np.array(Yscore[:,1], dtype=[('BDT', np.float32)])

root_numpy.array2root(Yscore,
                      'temp_output/bdt/predictions_BDT.root',
                      treename='pairTree_MVAoutput',
                      mode='recreate')
