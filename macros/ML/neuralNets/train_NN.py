from __future__ import division

import sys
from select import select
import os
import time
import argparse

import matplotlib
matplotlib.use('agg')
import matplotlib.pyplot as plt
import seaborn as sns
sns.set()

import numpy as np
import pandas as pd

from sklearn.utils import shuffle
from sklearn.preprocessing import StandardScaler
from sklearn.model_selection import train_test_split
from sklearn.metrics import roc_auc_score
from sklearn.externals import joblib

import keras
from keras.models import Sequential, load_model
from keras.layers import Dense, Dropout, GaussianNoise, BatchNormalization
from keras.wrappers.scikit_learn import KerasClassifier

from modules.get_output_paths import *
from modules.logger import *
from modules.load_data import *
from modules.run_params import get_input_args
from modules.keras_models import DNNBinaryClassifier
from modules.keras_callbacks import callback_ROC
from modules.evaluation_plots import *


def calculate_pair_sample_weight(weight_1, weight_2):
    """
    Calculates pair weights from individual track weights.
    """
    
    print('Calculate sample weights....')
          
    return (weight_1 * weight_2).values.astype(np.float32)


def preprocess_data(X, output_prefix, load_fitted_attributes=False):
    """
    Performs preprocessing steps on the data and returns it as a numpy array.
    """

    print('Data preprocessing: Scaling to zero mean and unit variance')

    if(load_fitted_attributes and output_prefix==None):
        print('Error: Cannot load fitted attributes during data preprocessing. Variable "output_prefix" is not defined.')
        sys.exit()
    
    scaler_attributes_filename = str(output_prefix) + 'StandardScaler_attributes.pkl'
    
    if load_fitted_attributes:
        print('Loading previously determined scaler attributes...')
        scaler_attributes = joblib.load(scaler_attributes_filename)
        scaler_mean = scaler_attributes[0,:]
        scaler_scale = scaler_attributes[1,:]
        X -= scaler_mean
        X /= scaler_scale
        return X
    
    else:
        scaler = StandardScaler()
        X = scaler.fit_transform(X)
        Xfeats_mean = scaler.mean_
        Xfeats_scale = scaler.scale_
        Xfeats_var = scaler.var_
        # store the (vstacked) array of shape (3,n_feats)
        joblib.dump(np.array([Xfeats_mean, Xfeats_scale, Xfeats_var], dtype=np.float32),
                    scaler_attributes_filename)
        return X



def plot_metrics_history(hist):
    """
    Plots the learning curves for all compiled metrics.
    """

    print('Create plots for metrics history...')
    
    plt.figure()
    plt.xlabel('epochs')
    
    for metric in hist.history:
        plt.plot(hist.history[metric], label=metric)

    plt.legend()
    plt.savefig(output_prefix + 'metrics_history.png')
    plt.savefig(output_prefix + 'metrics_history.pdf')


    
# ----------------------------------------------------------


def main():

    sys.stdout = logger()
    
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
        'phi2',
        'PIDeff1',
        'PIDeff2',
        'IsRP',
        'IsConv',
        'generator',
        'firstMothersInfo1',
        'electronsWithHFMother'
    ]

    branches_singleTree = [
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

    data_HFenh = load_data("/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/workingData/HFAnalysis/CA_AnalysisResults_Upgrade_HFenh_pairTree-us_part1_0.2nEvents.root",
                           branches=branches_pairTree,
                           start=0,
                           stop=num_rows,
                           selection="((firstMothersInfo1==1 && generator==3) || (firstMothersInfo1==2 && generator==4) || (firstMothersInfo1==2 && generator==5)) && electronsWithHFMother <= 2")

    # add new column with class label
    data_HFenh['class_HF'] = 1
    
    data_GP = load_data("/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/workingData/HFAnalysis/CA_AnalysisResults_Upgrade_GP_pairTree-us_part1_0.2nEvents.root",
                        branches=branches_pairTree,
                        start=0,
                        stop=num_rows,
                        selection="IsRP==1 && IsConv==0")

    # add new column with class label
    data_GP['class_HF'] = 0

    data_orig = shuffle(pd.concat([data_HFenh, data_GP]))
    
    #data_orig = load_data("/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/workingData/DNNAnalysis/FT2_ITSup_pairTree-us_part1_424650tightCutEvents.root",
    #                      branches=branches_pairTree,
    #                      start=0,
    #                      stop=num_rows,
    #                      selection='IsRP==1 && TrackCut1==2 && TrackCut2==2')
    
    # target vector setup
    Y = pd.DataFrame()
    Y = (data_orig['class_HF']==1).astype(int)

    # feature matrix setup
    X = pd.DataFrame()
    
    # pair tree features
    X['p'] = np.sqrt((data_orig['px1']+data_orig['px2'])*(data_orig['px1']+data_orig['px2']) +
                     (data_orig['py1']+data_orig['py2'])*(data_orig['py1']+data_orig['py2']) +
                     (data_orig['pz1']+data_orig['pz2'])*(data_orig['pz1']+data_orig['pz2']))
    X['phiv'] = data_orig['phiv'] - 1.57
    X['mass'] = data_orig['mass']
    X['pz_diff'] = np.divide(data_orig['pz1'], np.sqrt(data_orig['px1']*data_orig['px1'] + data_orig['py1']*data_orig['py1'])) - np.divide(data_orig['pz2'], np.sqrt(data_orig['px2']*data_orig['px2'] + data_orig['py2']*data_orig['py2']))
    X['sumz'] = data_orig['sumz']
    X['diffz'] = data_orig['diffz'] - 1.57
    X['opang'] = data_orig['opang']
    X['nITS1'] = data_orig['nITS1']
    X['nITS2'] = data_orig['nITS2']
    X['nITSshared1'] = data_orig['nITSshared1']
    X['nITSshared2'] = data_orig['nITSshared2']
    X['nTPC1'] = data_orig['nTPC1']
    X['nTPC2'] = data_orig['nTPC2']

    ## calculate angle between pair DCA vectors
    #temp_DCAprod = ((data_orig['DCAx1']*data_orig['DCAx2']) + (data_orig['DCAy1']*data_orig['DCAy2']) + (data_orig['DCAz1']*data_orig['DCAz2'])) / (np.sqrt(data_orig['DCAx1']*data_orig['DCAx1'] + data_orig['DCAy1']*data_orig['DCAy1'] + data_orig['DCAz1']*data_orig['DCAz1']) * np.sqrt(data_orig['DCAx2']*data_orig['DCAx2'] + data_orig['DCAy2']*data_orig['DCAy2'] + data_orig['DCAz2']*data_orig['DCAz2']))
    #temp_DCAprod[temp_DCAprod>1.] = 1. # removes overfloats due to numerical imprecision
    #temp_DCAprod[temp_DCAprod<-1.] = -1.
    #X['DCAangle'] = np.arccos(temp_DCAprod)
    #
    #X['DCAdiff'] = (data_orig['DCAx1']-data_orig['DCAx2'] + \
        #                     data_orig['DCAy1']-data_orig['DCAy2'] + \
        #                     data_orig['DCAz1']-data_orig['DCAz2'])
    #
    #X['DCAabs'] = (data_orig['DCAx1']-data_orig['DCAx2'])*(data_orig['DCAx1']-data_orig['DCAx2']) + \
        #              (data_orig['DCAy1']-data_orig['DCAy2'])*(data_orig['DCAy1']-data_orig['DCAy2']) + \
        #              (data_orig['DCAz1']-data_orig['DCAz2'])*(data_orig['DCAz1']-data_orig['DCAz2'])
    ##X['DCAfeat'] = 1/(data_orig['DCAx1']-data_orig['DCAx2']) + 1/(data_orig['DCAy1']-data_orig['DCAy2']) + 1/(data_orig['DCAz1']-data_orig['DCAz2'])
    X['DCAx1'] = np.abs(data_orig['DCAx1'])
    X['DCAx2'] = np.abs(data_orig['DCAx2'])
    X['DCAy1'] = np.abs(data_orig['DCAy1'])
    X['DCAy2'] = np.abs(data_orig['DCAy2'])
    X['DCAz1'] = np.abs(data_orig['DCAz1'])
    X['DCAz2'] = np.abs(data_orig['DCAz2'])

    #X['DCAxy1'] = np.log(np.abs(data_orig['DCAxy1_norm']))
    #X['DCAxy2'] = np.log(np.abs(data_orig['DCAxy2_norm']))
    #X['DCAz1'] = np.abs(data_orig['DCAz1'])
    #X['DCAz2'] = np.abs(data_orig['DCAz2'])
    X['ITSchi21'] = data_orig['ITSchi21']
    X['ITSchi22'] = data_orig['ITSchi22']
    X['TPCchi21'] = data_orig['TPCchi21']
    X['TPCchi22'] = data_orig['TPCchi22']
    X['pt1'] = data_orig['pt1']
    X['pt2'] = data_orig['pt2']
    X['eta1'] = data_orig['eta1']
    X['eta2'] = data_orig['eta2']
    X['phi1'] = data_orig['phi1']
    X['phi2'] = data_orig['phi2']
    
    """
    # single tree features
    X['eta'] = data_orig['eta']
    X['phi'] = data_orig['phi']
    X['pt'] = data_orig['pt']
    X['dcaX'] = data_orig['dcaX']
    X['dcaY'] = data_orig['dcaY']
    X['dcaZ'] = data_orig['dcaZ']
    X['p'] = np.sqrt(data_orig['particle.fPx']*data_orig['particle.fPx'] + \
                     data_orig['particle.fPy']*data_orig['particle.fPy'] + \
                     data_orig['particle.fPz']*data_orig['particle.fPz'])
    X['nITS'] = data_orig['nITS']
    X['nTPC'] = data_orig['nTPC']
    X['nITSshared'] = data_orig['nITSshared']
    X['ITSchi2'] = data_orig['ITSchi2']
    X['TPCchi2'] = data_orig['TPCchi2']
    """
    
    # save feature names for later purposes
    X_featureNames = list(X)
    print('Selected features:', X_featureNames)
    joblib.dump(X_featureNames, output_prefix+'featureNames.pkl')

    
    sample_weight = calculate_pair_sample_weight(data_orig['PIDeff1'], data_orig['PIDeff2'])


    print('Total number of events in data sample: %d' % X.shape[0])
    print('Number of signal events in data sample: %d (%.2f percent)' %
          (Y[Y==1].shape[0], Y[Y==1].shape[0]*100/Y.shape[0]))
    print('Number of backgr events in data sample: %d (%.2f percent)' %
          (Y[Y==0].shape[0], Y[Y==0].shape[0]*100/Y.shape[0]))

    del data_orig

    
    # Data Split in Training, Validation and Test Samples
    
    print('Splitting the data in training, validation and test samples...')
    
    X_train, X_test, y_train, y_test, sample_weight_train, sample_weight_test = train_test_split(X, Y, sample_weight, test_size=test_sample_size, random_state=42)

    X_train, X_val, y_train, y_val, sample_weight_train, sample_weight_val = train_test_split(X_train, y_train, sample_weight_train, test_size=val_sample_size, random_state=43)

    print('Number of signal events in training sample: %d (%.2f percent)' %
          (y_train[y_train==1].shape[0], y_train[y_train==1].shape[0]*100/y_train.shape[0]))
    print('Number of backgr events in training sample: %d (%.2f percent)' %
          (y_train[y_train==0].shape[0], y_train[y_train==0].shape[0]*100/y_train.shape[0]))
    print('Number of signal events in validation sample: %d (%.2f percent)' %
          (y_val[y_val==1].shape[0], y_val[y_val==1].shape[0]*100/y_val.shape[0]))
    print('Number of backgr events in validation sample: %d (%.2f percent)' %
          (y_val[y_val==0].shape[0], y_val[y_val==0].shape[0]*100/y_val.shape[0]))
    print('Number of signal events in test sample: %d (%.2f percent)' %
          (y_test[y_test==1].shape[0], y_test[y_test==1].shape[0]*100/y_test.shape[0]))
    print('Number of backgr events in test sample: %d (%.2f percent)' %
          (y_test[y_test==0].shape[0], y_test[y_test==0].shape[0]*100/y_test.shape[0]))


    # Data preprocessing
    X_train = preprocess_data(X_train, output_prefix)
    X_val = preprocess_data(X_val, output_prefix, load_fitted_attributes=True)
    X_test = preprocess_data(X_test, output_prefix, load_fitted_attributes=True)

    # Convert pandas to numpy arrays
    X_train = np.array(X_train)
    X_val = np.array(X_val)
    X_test = np.array(X_test)
    y_train = np.array(y_train)
    y_val = np.array(y_val)
    y_test = np.array(y_test)
    

    if load_pretrained_model:
        if not os.path.exists(pretrained_model_filename):
            print('Error loading model. File %s does not exist.' % pretrained_model_filename)
            sys.exit()
            
        print('Loading pretrained model from file %s' % pretrained_model_filename)
        model = load_model(pretrained_model_filename)
        print(model.summary())
    else:
        
        timeout = 5
        print('\nInfo: The model will be trained anew. Existing saves will be overwritten.')
        print('      (Program paused for %d seconds. Enter an arbitrary string to abort the training.)' % timeout)
        rlist = select([sys.stdin], [], [], timeout)[0]

        if rlist:
            print('Input received. Exit the program...')
            sys.exit()
        else:
            print('No input received. Continue running the program...')
        
            print('Creating the model...')
            model_args = {
                "nr_of_layers": 4,
                "first_layer_size": 50,
                "layers_slope_coeff": 1.0,
                "dropout": 0.5,
                "normalize_batch": False,
                "noise": 0.0,
                "activation": "relu",
                "kernel_initializer": "glorot_normal",
                "bias_initializer": "zeros",
                "input_dim": X_train.shape[1]
            }
            print("Model parameters: ", model_args)
            
            model = DNNBinaryClassifier(**model_args)
            
            print('Fitting the model...')
            hist = model.fit(X_train, y_train,
                             batch_size=batchsize,
                             epochs=num_epochs,
                             callbacks=[callback_ROC(X_train,
                                                     y_train,
                                                     sample_weight_train,
                                                     output_prefix,
                                                     keras_models_prefix)],
                             verbose=verbose_setting,
                             validation_data=(X_val, y_val, sample_weight_val))
            
            plot_metrics_history(hist)
            print('Finished training.')
            
            print('Loading model with the highest VAL AUC...')
            model = load_model(output_prefix + keras_models_prefix + 'weights_final.hdf5')


        
    # model evaluation (training sample)

    num_process = X_train.shape[0] if X_train.shape[0]<=X_val.shape[0] else X_val.shape[0]

    print('Evaluating the model on the training sample...')
    y_train_score = model.predict_proba(X_train[0:num_process,:])
    print('\n')

    num_trueSignal, num_trueBackgr = plot_MVAoutput(y_train[0:num_process], y_train_score,
                                                    label='train')

    MVAcut_opt = plot_cut_efficiencies(num_trueSignal, num_trueBackgr)

    plot_ROCcurve(y_train[0:num_process], y_train_score, sample_weight_train[0:num_process],
                  label='train', workingpoint=MVAcut_opt)

    del y_train_score, num_trueSignal, num_trueBackgr
    
    
    # model evaluation (validation sample)

    print('Evaluating the model on the validation sample...')
    y_val_score = model.predict_proba(X_val)
    print('\n')

    plot_ROCcurve(y_val, y_val_score, sample_weight_val, label='val')

    del y_val_score

    
    # model evaluation (test sample)

    print('Evaluating the model on the test sample...')
    y_test_score = model.predict_proba(X_test)
    print('\n')

    plot_ROCcurve(y_test, y_test_score, sample_weight_test, label='test')

    del y_test_score


    
if __name__ == "__main__":
    
    # fix random seed for reproducibility
    np.random.seed(7)

    user_argv = None

    if(len(sys.argv) > 1):
        user_argv = sys.argv[1:]
    
    parser_results = get_input_args(user_argv)
    print('Received input arguments: ', parser_results)
    
    load_pretrained_model = parser_results.load_pretrained_model
    num_rows = parser_results.num_rows
    num_val_sample = parser_results.num_val_sample
    num_test_sample = parser_results.num_test_sample
    frac_val_sample = parser_results.frac_val_sample
    frac_test_sample = parser_results.frac_test_sample
    num_epochs = parser_results.num_epochs
    batchsize = parser_results.batchsize
    # num_jobs = parser_results.num_jobs
    verbose_setting = parser_results.verbose_setting

    if(num_val_sample == -1):
        val_sample_size = frac_val_sample
    else:
        val_sample_size = num_val_sample

    if(num_test_sample == -1):
        test_sample_size = frac_test_sample
    else:
        test_sample_size = num_test_sample
    
    output_prefix, keras_models_prefix = get_output_paths()
    pretrained_model_filename = output_prefix + keras_models_prefix + 'weights_final.hdf5'

    main()
    
