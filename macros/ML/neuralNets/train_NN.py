from __future__ import division

import sys
import os
import time

import matplotlib
matplotlib.use('agg')
import matplotlib.pyplot as plt
import seaborn as sns
sns.set()

import numpy as np
import pandas as pd

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

class callback_ROC(keras.callbacks.Callback):
    """
    Custom callback class for Keras which calculates and plots ROC AUCs after each training epoch.
    """
    
    def __init__(self):
        self.best=0
        self.wait=0

    def on_train_begin(self, logs={}):
        self.aucs_val = []
        self.aucs_train = []
        self.losses = []
        plt.figure(figsize=(15, 8.44), dpi=150)
        self.interval_evaluate_trainAUC = int(10)
 
    def on_train_end(self, logs={}):
        return
 
    def on_epoch_begin(self, epoch, logs={}):
        global start_time 
        start_time = time.time()
        return
 
    def on_epoch_end(self, epoch, logs={}):
        loss = logs.get('val_loss')
        self.losses.append(loss)
        global roc_auc_val
        if(epoch%self.interval_evaluate_trainAUC != 0):
        
            y_pred_val = self.model.predict(X_val)
            roc_auc_val = roc_auc_score(self.validation_data[1], y_pred_val)
            self.aucs_val.append(roc_auc_val)
            self.aucs_train.append(0)
            
            print("Epoch {} took {:.1f}s".format(epoch, time.time() - start_time)),
            print("   LogLoss: {:.4f}".format(loss)),
            print("   VAL AUC: {:.3f} %".format( roc_auc_val * 100))    
            
        if(epoch%self.interval_evaluate_trainAUC == 0):
            y_pred_val = self.model.predict(X_val)
            
            roc_auc_val = roc_auc_score(self.validation_data[1], y_pred_val)
            self.aucs_val.append(roc_auc_val)

            y_pred_train = self.model.predict(X_train)
            roc_auc_train = roc_auc_score(y_train, y_pred_train) 
            self.aucs_train.append(roc_auc_train)
        
            print("Epoch {} took {:.1f}s".format(epoch, time.time() - start_time)),
            print("   LogLoss: {:.4f}".format(loss)),        
            print("   VAL AUC: {:.3f} %".format( roc_auc_val  * 100)),
            print("   TRAIN AUC: {:.3f} %".format( roc_auc_train * 100))

            plt.clf()
            plt.plot(self.aucs_val, label='validation sample', color='C1')
            plt.plot(self.aucs_train, label='training sample', \
                     marker='o', fillstyle='none', markersize=4, mew=2, linestyle='none', color='C0')
            plt.xlabel("Epochs")
            plt.ylabel("ROC AUC")
            plt.legend(loc='best', fontsize=16)
            plt.grid(True)
            plt.ylim(0.5,1.05)
            plt.tight_layout()
            plt.savefig(output_prefix + 'learningcurve_rocauc_epochs.png')
            plt.savefig(output_prefix + 'learningcurve_rocauc_epochs.pdf')

        current = roc_auc_val
        if current > self.best:
            self.best = current
            self.wait = 0
            self.model.save(output_prefix + keras_models_prefix + 'model_{}_ROC_AUC{:.5f}.hdf5'.format(epoch,current),overwrite=True)
            self.model.save(output_prefix + keras_models_prefix + 'weights_final.hdf5',overwrite=True)

        else:
            if self.wait >= 10:             #patience
                # self.model.stop_training = True
                print('Epoch %05d: early stopping' % (epoch))
                
                
            self.wait += 1 #increment the number of times without improvement
        return

    def on_batch_begin(self, batch, logs={}):
        return
 
    def on_batch_end(self, batch, logs={}):
        return


def create_model(nr_of_layers = 2,
                 first_layer_size = 100,
                 layers_slope_coeff = 1.0,
                 dropout = .5,
                 normalize_batch=True,
                 noise = 1.,
                 activation = 'relu',
                 kernel_initializer = 'glorot_normal',
                 bias_initializer = 'glorot_normal',
                 input_dim = 2):
    
    model = Sequential()
    
    model.add(Dense(first_layer_size,
                   input_dim = input_dim,
                   activation = activation,
                   kernel_initializer = kernel_initializer,
                   bias_initializer = bias_initializer))

    if layers_slope_coeff != 1:
        current_layer_size = int(first_layer_size * layers_slope_coeff) + 1
    else:
        current_layer_size = int(first_layer_size)
    
    for index_of_layer in range(nr_of_layers - 1):
        if normalize_batch==True:
            model.add(BatchNormalization())
            
        model.add(Dropout(dropout))
        #if index_of_layer%2==0: model.add(GaussianNoise(noise))
        model.add(Dense(current_layer_size,
                       activation = activation,
                       kernel_initializer = kernel_initializer,
                       bias_initializer = bias_initializer))
        if layers_slope_coeff != 1:
            current_layer_size = int(current_layer_size * layers_slope_coeff) + 1

    if normalize_batch==True:
        model.add(BatchNormalization())
        
    model.add(Dense(1,
                   kernel_initializer = kernel_initializer,
                   activation = 'sigmoid'))
    
    model.compile(loss = 'binary_crossentropy',
                  optimizer = 'adam',
                  metrics=['accuracy'])

    print(model.summary())
    
    return model


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


if __name__ == '__main__':

    output_prefix, keras_models_prefix = get_output_paths()
    
    # fix random seed for reproducibility
    np.random.seed(7)

    sys.stdout = logger()

    load_pretrained_model = False
    pretrained_model_filename = output_prefix + keras_models_prefix + 'weights_final.hdf5'
    
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
        'IsConv'
    ]
    
    data_orig = load_data("/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/workingData/FT2_AnalysisResults_Upgrade_DCAvec_PIDeffs_pairtree_us_part1_1-9-split_correctedPIDeffs.root",
                          branches=branches_pairTree,
                          start=0,
                          stop=500000,
                          selection='mass>0.05')
    
    # target vector setup
    Y = pd.DataFrame()
    Y = (~((data_orig['IsRP']==0) & (data_orig['IsConv']==1))).astype(int)

    # feature matrix setup
    X = pd.DataFrame()
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

    
    X = preprocess_data(X, output_prefix) # X is a numpy array from now on

    
    # Data Split in Training, Validation and Test Samples
    
    print('Splitting the data in training, validation and test samples...')

    X_train, X_test, y_train, y_test, sample_weight_train, sample_weight_test = train_test_split(X, Y, sample_weight, test_size=1/3., random_state=42)

    X_train, X_val, y_train, y_val, sample_weight_train, sample_weight_val = train_test_split(X_train, y_train, sample_weight_train, test_size=1/2., random_state=43)

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
    
    

    if load_pretrained_model:
        if not os.path.exists(pretrained_model_filename):
            print('Error loading model. File %s does not exist.' % pretrained_model_filename)
            sys.exit()
            
        print('Loading pretrained model from file %s' % pretrained_model_filename)
        model = load_model(pretrained_model_filename)
        print(model.summary())
    else:
        print('Creating the model...')
        model = create_model(nr_of_layers=4,
                             first_layer_size=100,
                             layers_slope_coeff=1.,
                             dropout=0.4,
                             normalize_batch=False,
                             noise=0.,
                             activation='relu',
                             kernel_initializer='glorot_normal',
                             bias_initializer='glorot_normal',
                             input_dim=X_train.shape[1])
        
        print('Fitting the model...')
        hist = model.fit(X_train, y_train,
                         batch_size=1000,
                         epochs=11,
                         callbacks=[callback_ROC()],
                         verbose=0,
                         validation_data=(X_val, y_val))

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
