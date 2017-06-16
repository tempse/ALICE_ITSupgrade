from __future__ import division

import numpy as np
import matplotlib
matplotlib.use('agg')
import matplotlib.pyplot as plt
import seaborn as sns; sns.set()
import pandas as pd
import root_numpy

from sklearn.preprocessing import StandardScaler
from sklearn.decomposition import PCA
from sklearn.model_selection import cross_val_score, train_test_split
#from sklearn import metrics
from sklearn.metrics import roc_auc_score, roc_curve, auc
from sklearn.metrics import precision_recall_curve, average_precision_score
from sklearn.metrics import confusion_matrix, classification_report
from sklearn.ensemble import RandomForestClassifier
#from sklearn.ensemble import BaggingClassifier
from sklearn.externals import joblib
import itertools



# fix random seed for reproducibility
seed = 7
np.random.seed(seed)


# ## Data Import

print('Loading data...')

num_entries = 1000000
start = 0

inputfilename = "~/ITSup_testing_data/FT2_AnalysisResults_Upgrade_addFeat_part1_1-9-split.root"

branches_pairTree = [
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
                                                     #start=start,
                                                     #stop=num_entries+start
))


#print('Setting initial mass cuts...')
#dataSample_orig = dataSample_orig.drop(dataSample_orig[dataSample_orig['mass']<.05].index)


print('Engineering features...')

"""
# pairTree features
X = pd.DataFrame()
###X['p'] = np.sqrt((dataSample_orig['px1']+dataSample_orig['px2'])*(dataSample_orig['px1']+dataSample_orig['px2']) +
###                          (dataSample_orig['py1']+dataSample_orig['py2'])*(dataSample_orig['py1']+dataSample_orig['py2']) +
###                          (dataSample_orig['pz1']+dataSample_orig['pz2'])*(dataSample_orig['pz1']+dataSample_orig['pz2']))
###X['phiv'] = dataSample_orig['phiv'] - 1.57
###X['mass'] = dataSample_orig['mass']
###X['pz_diff'] = np.divide(dataSample_orig['pz1'], np.sqrt(dataSample_orig['px1']*dataSample_orig['px1'] + dataSample_orig['py1']*dataSample_orig['py1'])) - np.divide(dataSample_orig['pz2'], np.sqrt(dataSample_orig['px2']*dataSample_orig['px2'] + dataSample_orig['py2']*dataSample_orig['py2']))
###X['sumz'] = dataSample_orig['sumz']
###X['diffz'] = dataSample_orig['diffz'] - 1.57
###X['opang'] = dataSample_orig['opang']
###X['nITS1'] = dataSample_orig['nITS1']
###X['nITS2'] = dataSample_orig['nITS2']
X['nITSshared1'] = dataSample_orig['nITSshared1']
X['nITSshared2'] = dataSample_orig['nITSshared2']
###X['nTPC1'] = dataSample_orig['nTPC1']
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
###X['ITSchi21'] = dataSample_orig['ITSchi21']
X['ITSchi22'] = dataSample_orig['ITSchi22']
###X['TPCchi21'] = dataSample_orig['TPCchi21']
X['TPCchi22'] = dataSample_orig['TPCchi22']
X['pt1'] = dataSample_orig['pt1']
X['pt2'] = dataSample_orig['pt2']
###X['eta1'] = dataSample_orig['eta1']
X['eta2'] = dataSample_orig['eta2']
###X['phi1'] = dataSample_orig['phi1']
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
joblib.dump(X_featureNames, 'temp_output/bdt/featureNames.pkl')


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
plt.savefig('temp_output/bdt/correlation_matrix.png')



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
                'temp_output/bdt/StandardScaler_attributes.pkl')
    

# ### Principal Component Analysis

pca = PCA().fit(X)
plt.figure()
plt.plot(np.cumsum(pca.explained_variance_ratio_))
plt.xlabel('number of components')
plt.ylabel('cumulative explained variance')
plt.savefig('temp_output/bdt/pca_cumvar_ndim.png')


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




# ## Model Definition and Training

# ### Boosted Decision Trees (Random Forest)



# #### Classifier parameter setting:

n_jobs = 3
pre_dispatch = 1
n_estimators = 300
max_depth = None
min_samples_split = 500
class_weight = 'balanced'
criterion = 'gini'
max_features = None


print('Creating random forest classifier...')

clf = RandomForestClassifier(n_jobs=n_jobs,
                             n_estimators=n_estimators,
                             max_depth=max_depth,
                             min_samples_split=min_samples_split,
                             class_weight=class_weight,
                             criterion=criterion,
                             max_features=max_features
                            )

#clf = BaggingClassifier(rfc,
#                        bootstrap=False,
#                        n_estimators=15,
#                        max_samples=0.5)


doGridSearch = False

if not doGridSearch:
    print('Training the classifier...')
    
    plt.figure()
    #plt.xticks(rotation=90)
    plt.xlabel('Number of training events (in millions)')
    plt.ylabel('ROC AUC')
    plt.ylim(.65,1.05)
    for i in [1]:# [.1,.25,.4,.55,.7,.85,1]:
        #scores = cross_val_score(clf,
        #                         Xtrain[0:int(Xtrain.shape[0]*i),:],
        #                         Ytrain[0:int(Ytrain.shape[0]*i)],
        #                         cv=2,
        #                         scoring='f1_macro',
        #                         n_jobs=n_jobs,
        #                         pre_dispatch=pre_dispatch
        #                         )
        clf = clf.fit(Xtrain[0:int(Xtrain.shape[0]*i),:],
                      Ytrain[0:int(Ytrain.shape[0]*i)],
                      #sample_weight=sample_weight_train[0:int(sample_weight_train.shape[0]*i)]
        )
        joblib.dump(clf, 'temp_output/bdt/clf_weights.pkl')
        current_auc = roc_auc_score(Yval[0:int(Yval.shape[0]*i)],
                                    clf.predict_proba(Xval[0:int(Xval.shape[0]*i),:])[:,1],
                                    #sample_weight=sample_weight_val[0:int(sample_weight_val.shape[0]*i)]
        )
        print('Relative size of training sample: %.2f,\tROC AUC = %.3f' % (i, current_auc))
        plt.plot(int(Xtrain.shape[0]*i)/(1000000.0), current_auc,
                 marker='o',
                 markersize=10,
                 color='#1f77b4',
                 linestyle='none')
        plt.tight_layout()
        plt.savefig('temp_output/bdt/ROCAUC_sampleSize.png')
        
    #print("Accuracy: %0.2f (+/- %0.2f)" % (scores.mean(), scores.std() * 2))

    print("Accuracy on training set:   {:.3f}".format(clf.score(Xtrain, Ytrain)))
    print("Accuracy on validation set: {:.3f}".format(clf.score(Xval, Yval)))

# ### Variable Importances

if not doGridSearch:
    print('Calculating feature importances...')
    importances = clf.feature_importances_
    std = np.std([tree.feature_importances_ for tree in clf.estimators_],
                 axis=0)
    indices = np.argsort(importances)[::-1]
    
    # Print the feature ranking
    print("Feature ranking:")
    
    indices_featureNames = np.empty([Xtrain.shape[1]], dtype=object)
    
    for f in range(Xtrain.shape[1]):
        if not doPCA:
            indices_featureNames[f] = X_featureNames[indices[f]]
            print("\t%d. %s \t(%f)" % (f + 1,
                                     indices_featureNames[f],
                                     importances[indices[f]]))
        if doPCA:
            print("\t%d. feature %d \t(%f)" % (f + 1,
                                            indices[f],
                                            importances[indices[f]]))
    
    # Plot the feature importances of the forest
    plt.figure()
    plt.title("Feature importances")
    plt.bar(range(Xtrain.shape[1]), importances[indices],
           color="r", yerr=std[indices], align="center")
    if doPCA:
        plt.xticks(range(Xtrain.shape[1]), indices)
    else:
        plt.xticks(range(Xtrain.shape[1]), indices_featureNames, rotation=90)
    plt.xlim([-1, Xtrain.shape[1]])
    plt.tight_layout()
    #plt.show()
    plt.savefig('temp_output/bdt/var_importance.png')


# ## Grid Search

if doGridSearch:
    print('Performing grid search...')
    from sklearn.model_selection import GridSearchCV
    
    param_grid = {
        'n_estimators': [100,200],
        'max_depth': [15, None],
        'min_samples_split': [2000,5000],
        'max_features': ['auto',None],
        'class_weight': ['balanced'],
        'criterion': ['entropy','gini']
    }
    scoring = 'f1_macro'
    
    grid = GridSearchCV(estimator=clf, param_grid=param_grid, cv=3,
                        n_jobs=n_jobs, pre_dispatch=pre_dispatch,
                        scoring=scoring)
    
    grid_result = grid.fit(Xtrain, Ytrain)
    
    print("Best: %f using %s" % (grid_result.best_score_, grid_result.best_params_))
    means = grid_result.cv_results_['mean_test_score']
    stds = grid_result.cv_results_['std_test_score']
    params = grid_result.cv_results_['params']
    for mean, stdev, param in zip(means, stds, params):
        print("%f (%f) with: %r" % (mean, stdev, param))
    print('Scoring method: %s' % scoring)
    exit()




del Xtrain, Ytrain#, sample_weight_train


# ## Evaluation of Trained Model

print('Evaluating the trained model on the validation set...')

Yscore_val = clf.predict_proba(Xval)


# ### MVA Output Distributions

print('Creating MVA output distributions...')

Yscore_val_backgr, Yscore_val_signal = np.split(Yscore_val, 2, axis=1)

Yscore_val_signal_trueSignal = Yscore_val_signal[np.array(Yval==1)]
Yscore_val_signal_trueBackgr = Yscore_val_signal[np.array(Yval==0)]

plt.figure()
nbins = 100
n_total, bins_total, patches_total = plt.hist(Yscore_val[:,1], bins=nbins, alpha=.25, color='black', label='MVA output')
n_B, bins_B, patches_B = plt.hist(Yscore_val_signal_trueBackgr, bins=nbins, alpha=.5, color='#dd0000', label='true background')
n_S, bins_S, patches_S = plt.hist(Yscore_val_signal_trueSignal, bins=nbins, alpha=.5, color='green', label='true signal')
plt.title('MVA output distribution (signal class)')
plt.xlabel('MVA output')
plt.ylabel('Entries')
plt.legend()
plt.savefig('temp_output/bdt/MVAoutput_distr_val.png')


# ### Calculation of Significance

significance = n_S/np.sqrt(n_S+n_B)
pos_maxSignificance = np.argmax(significance)

# probability threshold above which events are classified as 'signal':
threshold_proba = pos_maxSignificance/(nbins*1.0)

plt.figure()
plt.plot(significance, label='significance')
plt.plot(pos_maxSignificance, significance[pos_maxSignificance], 'o',
        markersize=10, fillstyle='none', mew=2,
        label='max. significance for cut at %.2f' % (threshold_proba))
plt.title('Significance distribution')
plt.xlabel('MVA output')
plt.ylabel('$S / \sqrt{(S+B)}$')
plt.legend()
plt.savefig('temp_output/bdt/MVAoutput_significance_val.png')


# ### Calculation of Signal-Background Ratio

SoverB = n_S/(n_B*1.0)
#pos_maxSoverB = np.argmax(SoverB)
plt.figure()
plt.plot(SoverB, label='signal over background')
#plt.plot(pos_maxSoverB, SoverB[pos_maxSoverB], 'o',
#        markersize=10, fillstyle='none', mew=2,
#        label='max. signal-over-backgr. for cut at %.2f' % (pos_maxSoverB/nbins))
plt.title('Signal-over-background distribution')
plt.xlabel('MVA output')
plt.ylabel('$S / B$')
#plt.legend()
plt.savefig('temp_output/bdt/MVAoutput_SoverB_val.png')


# ### ROC Curve

print('Generating ROC curve...')

fpr, tpr, thresholds = roc_curve(Yval, Yscore_val[:,1], pos_label=1)
#roc_auc = roc_auc_score(fpr, tpr)
roc_auc = roc_auc_score(Yval, Yscore_val[:,1])

plt.figure()
plt.plot(fpr, tpr, label='ROC curve (area = %0.3f)' % roc_auc)
plt.plot([0, 1], [0, 1], 'k--')
plt.xlim([0.0, 1.05])
plt.ylim([0.0, 1.05])
plt.xlabel('False Positive Rate')
plt.ylabel('True Positive Rate')
plt.title('Receiver operating characteristic curve')

## find point on ROC curve closest to the upper left corner
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
plt.savefig('temp_output/bdt/roc_curve_val.png')
print('AUC: %f' % roc_auc)


# ### Precision-Recall Curve

print('Generating precision-recall curve...')


# Compute Precision-Recall
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
plt.savefig('temp_output/bdt/precision_recall_val.png')


# ### Confusion Matrix


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
        outfile_name = 'temp_output/bdt/confusion_matrix_%s.png' % label
        plt.savefig(outfile_name)
    else:
        plt.savefig('temp_output/bdt/confusion_matrix_normalized_%s.png' % label)


# Compute confusion matrix
Yscore_val_labels = (Yscore_val[:,1]>threshold_proba)
cnf_matrix = confusion_matrix(Yval, Yscore_val_labels)
np.set_printoptions(precision=2)


# Plot non-normalized confusion matrix
plt.figure()
plot_confusion_matrix(cnf_matrix, classes=['background','signal'],
                      title='Confusion matrix (non-normalized)', label='val')
#plt.show()


# Plot normalized confusion matrix
plt.figure()
plot_confusion_matrix(cnf_matrix, classes=['background','signal'],
                      normalize=True, title='Normalized confusion matrix', label='val')


# ### Classification Report


print('Classification report (validation sample):')
print(classification_report(Yval, Yscore_val_labels,
                            target_names=['background','signal']))


del Yscore_val

# ---

# ## Test Model on New Data and Evaluate It

print('Evaluating the trained model on the test sample...')

Ytest_score = clf.predict_proba(Xtest)


# ### MVA Output Distribution

print('Creating MVA output distributions...')

Yscore_test_backgr, Yscore_test_signal = np.split(Ytest_score, 2, axis=1)

Yscore_test_signal_trueSignal = Yscore_test_signal[np.array(Ytest==1)]
Yscore_test_signal_trueBackgr = Yscore_test_signal[np.array(Ytest==0)]

plt.figure()
nbins = 100
n_total, bins_total, patches_total = plt.hist(Ytest_score[:,1], bins=nbins, alpha=.25, color='black', label='MVA output')
n_B, bins_B, patches_B = plt.hist(Yscore_test_signal_trueBackgr, bins=nbins, alpha=.5, color='#dd0000', label='true background')
n_S, bins_S, patches_S = plt.hist(Yscore_test_signal_trueSignal, bins=nbins, alpha=.5, color='green', label='true signal')
plt.title('MVA output distribution (signal class)')
plt.xlabel('MVA output')
plt.ylabel('Entries')
plt.legend()
plt.savefig('temp_output/bdt/MVAoutput_distr_test.png')


# ### Calculation of Significance

significance_test = n_S/np.sqrt(n_S+n_B)
pos_maxSignificance = np.argmax(significance_test)
plt.figure()
plt.plot(significance_test, label='significance')
plt.plot(pos_maxSignificance, significance_test[pos_maxSignificance], 'o',
        markersize=10, fillstyle='none', mew=2,
        label='max. significance for cut at %.2f' % (threshold_proba))
plt.title('Significance distribution')
plt.xlabel('MVA output')
plt.ylabel('$S / \sqrt{(S+B)}$')
plt.legend()
plt.savefig('temp_output/bdt/MVAoutput_significance_test.png')


# ### Calculation of Signal-to-Background Ratio

if not n_B.all()==0:
    SoverB = n_S/(n_B*1.0)
#pos_maxSoverB = np.argmax(SoverB)
plt.figure()
plt.plot(SoverB, label='signal over background')
#plt.plot(pos_maxSoverB, SoverB[pos_maxSoverB], 'o',
#        markersize=10, fillstyle='none', mew=2,
#        label='max. signal-over-backgr. for cut at %.2f' % (pos_maxSoverB/nbins))
plt.title('Signal-over-background distribution')
plt.xlabel('MVA output')
plt.ylabel('$S / B$')
#plt.legend()
plt.savefig('temp_output/bdt/MVAoutput_SoverB_test.png')


# ### ROC Curve

print('Generating ROC curve...')

fpr_test, tpr_test, thresholds_test = roc_curve(Ytest, Ytest_score[:,1], pos_label=1)
#roc_auc_test = roc_auc_score(fpr_test, tpr_test)
roc_auc_test = roc_auc_score(Ytest, Ytest_score[:,1])

plt.figure()
plt.plot(fpr_test, tpr_test, label='ROC curve (area = %0.3f)' % roc_auc_test)
plt.plot([0, 1], [0, 1], 'k--')
plt.xlim([0.0, 1.05])
plt.ylim([0.0, 1.05])
plt.xlabel('False Positive Rate')
plt.ylabel('True Positive Rate')
plt.title('Receiver operating characteristic curve')

# find point on ROC curve closest to the optimum operating point
# as determined before
close_optimum_test = np.argmin(np.abs(threshold_proba-thresholds_test))
plt.plot(fpr_test[close_optimum_test], tpr_test[close_optimum_test], 'o', markersize=10,
         label="threshold at %.2f" % threshold_proba, fillstyle="none",
        mew=2)

plt.legend(loc=4)
#plt.show()
plt.savefig('temp_output/bdt/roc_curve_test.png')
print('AUC: %f' % roc_auc_test)


# ### Precision-Recall Curve

print('Generating precision-recall curve...')

# Compute Precision-Recall
precision_test = dict()
recall_test = dict()
average_precision_test = dict()
precision_test, recall_test, thresholds_PRC_test = precision_recall_curve(Ytest,
                                                                          Ytest_score[:,1])
average_precision_test = average_precision_score(Ytest, Ytest_score[:,1])

plt.clf()

plt.plot(recall_test, precision_test, lw=lw,
         label='Precision-recall curve of signal class (area = {1:0.2f})'
         ''.format(1, average_precision_test))

# find threshold closest to threshold_proba
close_optimum = np.argmin(np.abs(thresholds_PRC_test-threshold_proba))
plt.plot(recall_test[close_optimum], precision_test[close_optimum],
         'o',
         markersize=10,
         label="threshold at %.2f" % threshold_proba,
         fillstyle="none",
         mew=2)

# find threshold closest to zero
close_zero = np.argmin(np.abs(thresholds_PRC_test))
plt.plot(recall_test[close_zero], precision_test[close_zero],
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
plt.savefig('temp_output/bdt/precision_recall_test.png')


# Compute confusion matrix
Ytest_score_labels = (Ytest_score[:,1]>threshold_proba) #thresholds[close_optimum]).astype(int)
cnf_matrix = confusion_matrix(Ytest, Ytest_score_labels)
np.set_printoptions(precision=2)


# Plot normalized confusion matrix
plt.figure()
plot_confusion_matrix(cnf_matrix, classes=['background','signal'],
                      normalize=True, title='Normalized confusion matrix', label='test')

# Plot non-normalized confusion matrix
plt.figure()
plot_confusion_matrix(cnf_matrix, classes=['background','signal'],
                      normalize=False, title='Confusion matrix (non-normalized)', label='test')


print('Classification report (test sample):')
print(classification_report(Ytest, Ytest_score_labels,
                            target_names=['background','signal']))
