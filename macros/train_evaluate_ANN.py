from __future__ import division

import time

import matplotlib
matplotlib.use('agg')
import matplotlib.pyplot as plt
import seaborn as sns; sns.set()

import root_numpy
import pandas as pd
import numpy as np
import math

import keras
from keras.models import Sequential
from keras.layers import Dense, Dropout, GaussianNoise
from keras.wrappers.scikit_learn import KerasClassifier
#from keras.utils.visualize_util import plot
import keras.backend as K

from sklearn.model_selection import GridSearchCV
from sklearn.metrics import roc_curve, roc_auc_score, accuracy_score, \
    confusion_matrix, classification_report, precision_recall_curve, \
    average_precision_score
from sklearn.preprocessing import StandardScaler
from sklearn.decomposition import PCA
from sklearn.model_selection import train_test_split, GridSearchCV
from sklearn.externals import joblib
import itertools


# fix random seed for reproducibility
seed = 7
np.random.seed(seed)


# ## Data Import

print('Loading data...')

num_entries = 25000000
start = 0

inputfilename = "~/ITSup_testing_data/FT2_AnalysisResults_Upgrade_addFeat_part2_1-9-split.root"

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
    'DCAx1','DCAy1',
    'DCAz1',
    'DCAx2','DCAy2',
    'DCAz2',
    'DCAxy1_norm','DCAxy2_norm',
    'DCAz1_norm','DCAz2_norm',
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


print("Reading file %s..." % inputfilename)
dataSample_orig = pd.DataFrame(root_numpy.root2array(inputfilename,
                                                     branches=branches_singleTree,
                                                     start=start,
                                                     stop=num_entries+start
))


#print('Setting initial mass cuts...')
#dataSample_orig = dataSample_orig.drop(dataSample_orig[dataSample_orig['mass']<.05].index)


print('Engineering features...')


"""
# pairTree features
X = pd.DataFrame()
X['p'] = np.sqrt((dataSample_orig['px1']+dataSample_orig['px2'])*(dataSample_orig['px1']+dataSample_orig['px2']) +
                          (dataSample_orig['py1']+dataSample_orig['py2'])*(dataSample_orig['py1']+dataSample_orig['py2']) +
                          (dataSample_orig['pz1']+dataSample_orig['pz2'])*(dataSample_orig['pz1']+dataSample_orig['pz2']))
X['phiv'] = dataSample_orig['phiv'] - 1.57
X['mass'] = dataSample_orig['mass']
X['pz_diff'] = np.divide(dataSample_orig['pz1'], np.sqrt(dataSample_orig['px1']*dataSample_orig['px1'] + dataSample_orig['py1']*dataSample_orig['py1'])) - np.divide(dataSample_orig['pz2'], np.sqrt(dataSample_orig['px2']*dataSample_orig['px2'] + dataSample_orig['py2']*dataSample_orig['py2']))
X['sumz'] = dataSample_orig['sumz']
X['diffz'] = dataSample_orig['diffz'] - 1.57
X['opang'] = dataSample_orig['opang']
X['nITS1'] = dataSample_orig['nITS1']
X['nITS2'] = dataSample_orig['nITS2']
X['nITSshared1'] = dataSample_orig['nITSshared1']
X['nITSshared2'] = dataSample_orig['nITSshared2']
X['nTPC1'] = dataSample_orig['nTPC1']
X['nTPC2'] = dataSample_orig['nTPC2']

## calculate angle between pair DCA vectors
#temp_DCAprod = ((dataSample_orig['DCAx1']*dataSample_orig['DCAx2']) + (dataSample_orig['DCAy1']*dataSample_orig['DCAy2']) + (dataSample_orig['DCAz1']*dataSample_orig['DCAz2'])) / (np.sqrt(dataSample_orig['DCAx1']*dataSample_orig['DCAx1'] + dataSample_orig['DCAy1']*dataSample_orig['DCAy1'] + dataSample_orig['DCAz1']*dataSample_orig['DCAz1']) * np.sqrt(dataSample_orig['DCAx2']*dataSample_orig['DCAx2'] + dataSample_orig['DCAy2']*dataSample_orig['DCAy2'] + dataSample_orig['DCAz2']*dataSample_orig['DCAz2']))
#temp_DCAprod[temp_DCAprod>1.] = 1. # removes overfloats due to numerical imprecision
#temp_DCAprod[temp_DCAprod<-1.] = -1.
#X['DCAangle'] = np.arccos(temp_DCAprod)
#
#X['DCAdiff'] = (dataSample_orig['DCAx1']-dataSample_orig['DCAx2'] + \
#                     dataSample_orig['DCAy1']-dataSample_orig['DCAy2'] + \
#                     dataSample_orig['DCAz1']-dataSample_orig['DCAz2'])
#
#X['DCAabs'] = (dataSample_orig['DCAx1']-dataSample_orig['DCAx2'])*(dataSample_orig['DCAx1']-dataSample_orig['DCAx2']) + \
#              (dataSample_orig['DCAy1']-dataSample_orig['DCAy2'])*(dataSample_orig['DCAy1']-dataSample_orig['DCAy2']) + \
#              (dataSample_orig['DCAz1']-dataSample_orig['DCAz2'])*(dataSample_orig['DCAz1']-dataSample_orig['DCAz2'])
##X['DCAfeat'] = 1/(dataSample_orig['DCAx1']-dataSample_orig['DCAx2']) + 1/(dataSample_orig['DCAy1']-dataSample_orig['DCAy2']) + 1/(dataSample_orig['DCAz1']-dataSample_orig['DCAz2'])
#X['DCAx1'] = np.abs(dataSample_orig['DCAx1'])
#X['DCAx2'] = np.abs(dataSample_orig['DCAx2'])
#X['DCAy1'] = np.abs(dataSample_orig['DCAy1'])
#X['DCAy2'] = np.abs(dataSample_orig['DCAy2'])
#X['DCAz1'] = np.abs(dataSample_orig['DCAz1'])
#X['DCAz2'] = np.abs(dataSample_orig['DCAz2'])

X['DCAxy1'] = np.log(np.abs(dataSample_orig['DCAxy1_norm']))
X['DCAxy2'] = np.log(np.abs(dataSample_orig['DCAxy2_norm']))
X['DCAz1'] = np.abs(dataSample_orig['DCAz1'])
X['DCAz2'] = np.abs(dataSample_orig['DCAz2'])
X['ITSchi21'] = dataSample_orig['ITSchi21']
X['ITSchi22'] = dataSample_orig['ITSchi22']
X['TPCchi21'] = dataSample_orig['TPCchi21']
X['TPCchi22'] = dataSample_orig['TPCchi22']
X['pt1'] = dataSample_orig['pt1']
X['pt2'] = dataSample_orig['pt2']
X['eta1'] = dataSample_orig['eta1']
X['eta2'] = dataSample_orig['eta2']
X['phi1'] = dataSample_orig['phi1']
X['phi2'] = dataSample_orig['phi2']
"""

# singleTree features
X = pd.DataFrame()
X['eta'] = dataSample_orig['eta']
X['phi'] = dataSample_orig['phi']
X['pt'] = dataSample_orig['pt']
X['dcaR'] = dataSample_orig['dcaR']
X['dcaZ'] = dataSample_orig['dcaZ']
X['p'] = np.sqrt(dataSample_orig['particle.fPx']*dataSample_orig['particle.fPx'] + \
                 dataSample_orig['particle.fPy']*dataSample_orig['particle.fPy'] + \
                 dataSample_orig['particle.fPz']*dataSample_orig['particle.fPz'])
X['nITS'] = dataSample_orig['nITS']
X['nTPC'] = dataSample_orig['nTPC']
X['nITSshared'] = dataSample_orig['nITSshared']
X['ITSchi2'] = dataSample_orig['ITSchi2']
X['TPCchi2'] = dataSample_orig['TPCchi2']

X_featureNames = list(X)
print('Selected features:', X_featureNames)
joblib.dump(X_featureNames, 'temp_output/ann/featureNames.pkl')


#print('Calculating sample weights...')
#sample_weight = (dataSample_orig['PIDeff1']*dataSample_orig['PIDeff2']).values.astype(np.float32)

    

# Preparation of the target vector (assign `1` and `0` to the entries, representing the signal and background classes, respectively):
Y = pd.DataFrame()
Y = (dataSample_orig['pdgMother']!=22).astype(int)
#Y = (~((dataSample_orig['IsRP']==0) & (dataSample_orig['IsConv']==1))).astype(int)

print('Total number of events in data sample: %d' % X.shape[0])
print('Number of signal events in data sample: %d (%.2f percent)' % (Y[Y==1].shape[0], Y[Y==1].shape[0]*100/Y.shape[0]))
print('Number of backgr events in data sample: %d (%.2f percent)' % (Y[Y==0].shape[0], Y[Y==0].shape[0]*100/Y.shape[0]))


del dataSample_orig



# ## Correlation Matrix

print('Computing correlation matrix...')
plt.figure()
plt.subplots(figsize=(12,9))
sns.heatmap(X.corr(), vmax=.8, square=True, linewidths=1, linecolor='#eeeeee')
plt.xticks(rotation=90)
plt.yticks(rotation=0)
plt.tight_layout();
plt.savefig('temp_output/ann/correlation_matrix.png')



# ## Data Preprocessing

print('Preprocessing data...')


# ### Feature Scaling to Zero Mean and Unit Variance

doScale = True

if doScale:
    print('Scaling features to zero mean and unit variance...')
    scaler = StandardScaler()
    X = scaler.fit_transform(X)
    Xfeats_mean = scaler.mean_
    Xfeats_scale = scaler.scale_
    Xfeats_var = scaler.var_
    
    # store the (vstacked) array of shape (3,n_feats)
    joblib.dump(np.array([Xfeats_mean, Xfeats_scale, Xfeats_var], dtype=np.float32),
                'temp_output/ann/StandardScaler_attributes.pkl')
    

# ### Principal Component Analysis

pca = PCA().fit(X)
plt.figure()
plt.plot(np.cumsum(pca.explained_variance_ratio_))
plt.xlabel('number of components')
plt.ylabel('cumulative explained variance')
plt.savefig('temp_output/ann/pca_cumvar_ndim.png')


doPCA = False

if doPCA:
    print('Performing principal component analysis...')
    n_components = int(X.shape[1]*.5)

    print("original shape:    ", X.shape)
    pca_train = PCA(n_components=n_components)
    pca_train.fit(X)
    X = pca_train.transform(X)
    print("transformed shape: ", X.shape)


    
# ## Data Split in Training, Validation and Test Samples

print('Splitting the data in training, validation and test samples...')

X, Xtest = np.array_split(X,2)
Y, Ytest = np.array_split(Y,2)
#sample_weight, sample_weight_test = np.array_split(sample_weight,2)

#testing
#Xtest, Xtest2 = np.array_split(Xtest,2)
#Ytest, Ytest2 = np.array_split(Ytest,2)
#sample_weight_test, sample_weight_test2 = np.array_split(sample_weight_test,2)
Xtest = np.resize(Xtest, (int(Xtest.shape[0]/2),Xtest.shape[1]))
Ytest = np.resize(Ytest, (int(Ytest.shape[0]/2)))
#sample_weight_test = np.resize(sample_weight_test, (int(sample_weight_test.shape[0]/2)))


Xtrain, Xval, Ytrain, Yval = train_test_split(X, Y, test_size=.5)
#Xtrain, Xval, Ytrain, Yval, sample_weight_train, sample_weight_val = train_test_split(X, Y, sample_weight, test_size=.5)

print('Number of signal events in training sample: %d (%.2f percent)' % (Ytrain[Ytrain==1].shape[0], Ytrain[Ytrain==1].shape[0]*100/Ytrain.shape[0]))
print('Number of backgr events in training sample: %d (%.2f percent)' % (Ytrain[Ytrain==0].shape[0], Ytrain[Ytrain==0].shape[0]*100/Ytrain.shape[0]))

print('Number of signal events in validation sample: %d (%.2f percent)' % (Yval[Yval==1].shape[0], Yval[Yval==1].shape[0]*100/Yval.shape[0]))
print('Number of backgr events in validation sample: %d (%.2f percent)' % (Yval[Yval==0].shape[0], Yval[Yval==0].shape[0]*100/Yval.shape[0]))



# callbacks definitions

earlyStopping = keras.callbacks.EarlyStopping(monitor='binary_accuracy',
                                              min_delta=.00001,
                                              patience=2)

class ROC(keras.callbacks.Callback):
    def __init__(self):
        self.best=0
        self.wait=0

    def on_train_begin(self, logs={}):
        self.aucs_val = []
        self.aucs_train = []
        self.losses = []
 
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
        if(epoch%5 != 0):
        
            y_pred_val = self.model.predict(Xval)
            roc_auc_val = roc_auc_score(self.validation_data[1], y_pred_val)
            self.aucs_val.append(roc_auc_val)
            self.aucs_train.append(0)
            
            print("Epoch {} took {:.1f}s".format(epoch, time.time() - start_time)),
            print("   LogLoss: {:.4f}".format(loss)),
            print("   VAL AUC: {:.3f} %".format( roc_auc_val * 100))    
            
        if(epoch%5 == 0):
            y_pred_val = self.model.predict(Xval)
            
            roc_auc_val = roc_auc_score(self.validation_data[1], y_pred_val)
            self.aucs_val.append(roc_auc_val)

            y_pred_train = self.model.predict(Xtrain)
            roc_auc_train = roc_auc_score(Ytrain, y_pred_train) 
            self.aucs_train.append(roc_auc_train)
            
            #test
#            roc_auc_val_test = roc_auc_score(self.model.validation_data[1], y_pred_val)

            print("Epoch {} took {:.1f}s".format(epoch, time.time() - start_time)),
            print("   LogLoss: {:.4f}".format(loss)),        
            print("   VAL AUC: {:.3f} %".format( roc_auc_val  * 100)),
            print("   TRAIN AUC: {:.3f} %".format( roc_auc_train * 100))
#            print(" VAL AUC unw.: {:.3f} %".format( roc_auc_val_test  * 100))
        
#            print("Save Learning Curve")
            plt.clf()
            plt.plot(self.aucs_val, label='validation sample')
#            aucs_train=zero_to_nan(self.aucs_train)     #remove 0 entries for plotting
            plt.plot(self.aucs_train, label='training sample')
            plt.xlabel("Epochs")
            plt.ylabel("ROC AUC")
            plt.legend(loc='best')
            plt.grid(True)
            plt.ylim(0.5,1.05);
            plt.savefig('temp_output/ann/learningcurve_rocauc_epochs.png')
            
#            print("Save ROC curve")        
#            fpr, tpr, thresholds = roc_curve(self.model.validation_data[1][:,1], y_pred_val[:,1], sample_weight=pairweights_val)
#            roc_auc = auc(fpr, tpr)
#            plt.figure()
#            plt.plot(tpr, 1-fpr, lw=1, label='ROC (area = %0.4f)'%(roc_auc))
#            plt.locator_params(nbins=11)
#            plt.xlim([-0.0, 1.0])
#            plt.ylim([-0.0, 1.0])
#            plt.ylabel('1-False Positive Rate = BG Rejection')
#            plt.xlabel('True Positive Rate = Sig Eff')
#            plt.title('Receiver operating characteristic')
#            plt.legend(loc="lower right")
#            plt.grid()    
#            plt.show()
#            plt.savefig('ROC_pref_train.png')
        current = roc_auc_val
        if current > self.best:
            self.best = current
            self.wait = 0
            self.model.save("temp_output/ann/keras_weights/model_{}_ROC_AUC{:.5f}.hdf5".format(epoch,current),overwrite=True)
            self.model.save("temp_output/ann/keras_weights/weights_final.hdf5",overwrite=True)

        else:
            if self.wait >= 10:             #patience
#                self.model.stop_training = True
                print('Epoch %05d: early stopping' % (epoch))
                
                
            self.wait += 1 #incremental the number of times without improvement
        return

    def on_batch_begin(self, batch, logs={}):
        return
 
    def on_batch_end(self, batch, logs={}):
        return


# metrics definitions
def precision(y_true, y_pred):
    """Precision metric.

    Only computes a batch-wise average of precision.

    Computes the precision, a metric for multi-label classification of
    how many selected items are relevant.
    """
    true_positives = K.sum(K.round(K.clip(y_true * y_pred, 0, 1)))
    predicted_positives = K.sum(K.round(K.clip(y_pred, 0, 1)))
    precision = true_positives / (predicted_positives + K.epsilon())
    return precision

def recall(y_true, y_pred):
    """Recall metric.

    Only computes a batch-wise average of recall.

    Computes the recall, a metric for multi-label classification of
    how many relevant items are selected.
    """
    true_positives = K.sum(K.round(K.clip(y_true * y_pred, 0, 1)))
    possible_positives = K.sum(K.round(K.clip(y_true, 0, 1)))
    recall = true_positives / (possible_positives + K.epsilon())
    return recall

    
# model definition and training

def create_model(nr_of_layers = 2,
                 first_layer_size = 10,
                 layers_slope_coeff = 0.8,
                 dropout = .5,
                 noise = 1.,
                 activation = 'tanh',
                 kernel_initializer = 'glorot_normal',
                 bias_initializer = 'uniform',
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
        model.add(Dropout(dropout))
        if index_of_layer%2==0: model.add(GaussianNoise(noise))
        model.add(Dense(current_layer_size,
                       activation = activation,
                       kernel_initializer = kernel_initializer,
                       bias_initializer = bias_initializer))
        if layers_slope_coeff != 1:
            current_layer_size = int(current_layer_size * layers_slope_coeff) + 1
    
    model.add(Dense(1,
                   kernel_initializer = kernel_initializer,
                   activation = 'sigmoid'))
    
    model.compile(loss = 'binary_crossentropy',
                 optimizer = 'adam',
                 metrics=[precision])

    print(model.summary())
    
    return model




model = KerasClassifier(build_fn=create_model,
                        epochs=30,
                        batch_size=1000000,
                        kernel_initializer='glorot_normal',
                        bias_initializer='uniform',
                        activation='tanh',
                        nr_of_layers=4,
                        first_layer_size=100,
                        layers_slope_coeff=1.,
                        input_dim = Xtrain.shape[1],
                        dropout=.1,
                        noise=.1,
                        verbose=0)


# model training

doGridSearch = False


roc_call = ROC()

if not doGridSearch:
    print('Fitting the model...')
    hist = model.fit(Xtrain, Ytrain,
                     #batch_size=250000,
                     epochs=400,
                     callbacks=[roc_call],
                     verbose=0,
                     validation_data=(Xval, Yval))
    


if doGridSearch:    
    param_grid = {
        'epochs': [5, 10, 50],
        'batch_size': [500],
        'nr_of_layers': [2,8],
        'first_layer_size': [5, int(Xtrain.shape[1]/2)],
        'layers_slope_coeff': [.3,.6],
        'dropout': [.5,.75],
        'kernel_initializer': ['uniform', 'glorot_normal'], #['uniform', 'normal', 'glorot_normal', 'glorot_uniform', 'he_normal', 'he_uniform'],
        'bias_initializer': ['uniform'], #['uniform', 'normal', 'glorot_normal', 'glorot_uniform', 'he_normal', 'he_uniform'],
        'activation': ['tanh'] #['softmax', 'softplus', 'softsign', 'relu', 'tanh', 'sigmoid', 'hard_sigmoid', 'linear']
    }
    
    grid = GridSearchCV(estimator=model, param_grid=param_grid, cv=3,
                       n_jobs=-1, pre_dispatch=2, verbose=0)
    
    grid_result = grid.fit(Xtrain, Ytrain)
        
    print("Best: %f using %s" % (grid_result.best_score_, grid_result.best_params_))
    means = grid_result.cv_results_['mean_test_score']
    stds = grid_result.cv_results_['std_test_score']
    params = grid_result.cv_results_['params']
    for mean, stdev, param in zip(means, stds, params):
        print("%f (%f) with: %r" % (mean, stdev, param))
    exit()



# evaluation of the trained model

Yscore_val = model.predict_proba(Xval)


Yscore_val_backgr, Yscore_val_signal = np.split(Yscore_val, 2, axis=1)

Yscore_val_signal_trueSignal = Yscore_val_signal[np.array(Yval==1)]
Yscore_val_signal_trueBackgr = Yscore_val_signal[np.array(Yval==0)]

nbins = 100
plt.figure()
n_total, bins_total, patches_total = plt.hist(Yscore_val[:,1],
                                              bins=nbins,
                                              alpha=.25,
                                              color='black',
                                              label='MVA output')
n_B, bins_B, patches_B = plt.hist(Yscore_val_signal_trueBackgr,
                                  bins=nbins,
                                  alpha=.5,
                                  color='#dd0000',
                                  label='true background')
n_S, bins_S, patches_S = plt.hist(Yscore_val_signal_trueSignal,
                                  bins=nbins,
                                  alpha=.5,
                                  color='green',
                                  label='true signal')
plt.title('Signal probability distribution')
plt.xlabel('Signal probability')
plt.ylabel('Entries')
plt.xlim(0,1)
plt.legend()
plt.savefig('temp_output/ann/MVAoutput_distr_val.png')


# calculation of significance

significance = n_S/np.sqrt(n_S+n_B)
pos_maxSignificance = np.argmax(significance)
plt.figure()
plt.plot(significance, label='significance')
plt.plot(pos_maxSignificance, significance[pos_maxSignificance], 'o',
        markersize=10, fillstyle='none', mew=2,
        label='max. significance for cut at %.2f' % (pos_maxSignificance/nbins))
plt.title('Significance distribution')
plt.xlabel('Signal probability [%]')
plt.ylabel('$S / \sqrt{(S+B)}$')
plt.legend()
plt.savefig('temp_output/ann/MVAoutput_significance_val.png')


# calculation of signal over background

SoverB = n_S/n_B
#pos_maxSoverB = np.argmax(SoverB)
plt.figure()
plt.plot(SoverB, label='significance')
#plt.plot(pos_maxSoverB, SoverB[pos_maxSoverB], 'o',
#        markersize=10, fillstyle='none', mew=2,
#        label='max. signal-over-backgr. for cut at %.2f' % (pos_maxSoverB/nbins))
plt.title('Signal-over-background distribution')
plt.xlabel('Signal probability [%]')
plt.ylabel('$S / B$')
#plt.legend()
plt.savefig('temp_output/ann/MVAoutput_SoverB_val.png')


#Summarise history for accuracy

#print(hist.history.keys())
plt.figure()
plt.plot(hist.history['precision'], label='train')
plt.plot(hist.history['val_precision'], label='validate')
plt.title('model accuracy')
plt.ylabel('accuracy')
plt.xlabel('epoch')
plt.legend()
#plt.show()
plt.savefig('temp_output/ann/learningCurve_acc_val.png')


# summarize history for loss

plt.figure()
plt.plot(hist.history['loss'])
plt.plot(hist.history['val_loss'])
plt.title('model loss')
plt.ylabel('loss')
plt.xlabel('epoch')
plt.legend(['train', 'validate'])
#plt.show()
plt.savefig('temp_output/ann/learningCurve_loss_val.png')



# ROC curve

# probability threshold above which events are classified as 'signal':
threshold_proba = pos_maxSignificance/nbins

fpr, tpr, thresholds = roc_curve(Yval, Yscore_val[:,1], pos_label=1)
roc_auc = roc_auc_score(Yval, Yscore_val[:,1])#, sample_weight=sample_weight_val)

plt.figure()
plt.plot(fpr, tpr, label='ROC curve (area = %0.3f)' % roc_auc)
plt.plot([0, 1], [0, 1], 'k--')
plt.xlim([0.0, 1.05])
plt.ylim([0.0, 1.05])
plt.xlabel('False Positive Rate')
plt.ylabel('True Positive Rate')
plt.title('Receiver operating characteristic curve')

# find point on ROC curve closest to the upper left corner
#close_optimum = np.argmin(fpr*fpr + (tpr-1)*(tpr-1))
#plt.plot(fpr[close_optimum], tpr[close_optimum], 'o',
#         markersize=10,
#         label="threshold at %.2f (optimal operating point)" % thresholds[close_optimum],
#         fillstyle="none", mew=2)
    
# find and plot threshold closest to threshold_proba
close_threshold_proba = np.argmin(np.abs(thresholds-threshold_proba))
plt.plot(fpr[close_threshold_proba], tpr[close_threshold_proba], 'o', markersize=10,
        label="threshold at %.2f" % threshold_proba, fillstyle="none",
        mew=2)

plt.legend(loc=4)
#plt.show()
print('AUC: %f' % roc_auc)
plt.savefig('temp_output/ann/ROCcurve_val.png')


# ### Precision-Recall Curve

print('Generating precision-recall curve...')


# compute Precision-Recall

precision = dict()
recall = dict()
average_precision = dict()
precision, recall, thresholds_PRC = precision_recall_curve(Yval,
                                                           Yscore_val[:,1])
average_precision = average_precision_score(Yval, Yscore_val[:,1])


# Plot Precision-Recall curve

lw=2
n_classes=1
plt.clf()

plt.plot(recall, precision, lw=lw,
         label='Precision-recall curve of signal class (area = {1:0.2f})'
                ''.format(1, average_precision))

# find threshold closest to threshold_proba
close_optimum = np.argmin(np.abs(thresholds_PRC-threshold_proba))
plt.plot(recall[close_optimum], precision[close_optimum],
         'o',
         markersize=10,
         label="threshold at %.2f" % threshold_proba,
         fillstyle="none",
         mew=2)

# find threshold closest to zero
close_zero = np.argmin(np.abs(thresholds_PRC))
plt.plot(recall[close_zero], precision[close_zero],
         'o',
         markersize=10,
         label="threshold zero",
         fillstyle="none",
         mew=2)

plt.xlim([-0.05, 1.05])
plt.ylim([-0.05, 1.05])
plt.xlabel(r'Recall $R=T_p / (T_p+F_n)$')
plt.ylabel(r'Precision $P=T_p / (T_p+F_p)$')
plt.title('Precision-Recall Curve')
plt.legend(loc="lower right")
#plt.show()
plt.savefig('temp_output/ann/precision_recall_val.png')


# confusion matrix

def plot_confusion_matrix(cm, classes,
                          normalize=False,
                          title='Confusion matrix',
                          cmap=plt.cm.Blues,
                          label='val'):
    """
    This function prints and plots the confusion matrix.
    Normalization can be applied by setting `normalize=True`.
    """
    
    print('Generating confusion matrix...')
    
    plt.imshow(cm, interpolation='nearest', cmap=cmap)
    plt.title(title)
    plt.colorbar()
    tick_marks = np.arange(len(classes))
    plt.xticks(tick_marks, classes, rotation=45)
    plt.yticks(tick_marks, classes, rotation=45)

    if normalize:
        cm = cm.astype('float') / cm.sum(axis=1)[:, np.newaxis]
        print("Normalized confusion matrix")
    else:
        print('Confusion matrix, without normalization')

    print(cm)

    thresh = cm.max() / 2.
    for i, j in itertools.product(range(cm.shape[0]), range(cm.shape[1])):
        plt.text(j, i, cm[i, j],
                 horizontalalignment="center",
                 color="white" if cm[i, j] > thresh else "black")

    plt.tight_layout()
    plt.ylabel('True label')
    plt.xlabel('Predicted label')
    plt.tight_layout()
    
    if normalize:
        outfile_name = 'temp_output/ann/confusion_matrix_%s.png' % label
        plt.savefig(outfile_name)
    else:
        plt.savefig('temp_output/ann/confusion_matrix_normalized_%s.png' % label)


# Compute confusion matrix

Yscore_val_labels = (Yscore_val[:,1]>pos_maxSignificance/nbins) #thresholds[close_optimum]).astype(int)
cnf_matrix = confusion_matrix(Yval, Yscore_val_labels)
np.set_printoptions(precision=2)


# Plot non-normalized confusion matrix

plt.figure()
plot_confusion_matrix(cnf_matrix, classes=['background','signal'],
                      title='Confusion matrix, without normalization', label='val')
#plt.show()


# Plot normalized confusion matrix

plt.figure()
plot_confusion_matrix(cnf_matrix, classes=['background','signal'],
                      normalize=True, title='Normalized confusion matrix', label='val')


# classification report

print(classification_report(Yval, Yscore_val_labels,
                            target_names=['background','signal']))

