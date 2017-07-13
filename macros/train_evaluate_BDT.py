from __future__ import division

import math

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
from sklearn.metrics import roc_auc_score, roc_curve, auc
from sklearn.metrics import precision_recall_curve, average_precision_score
from sklearn.metrics import confusion_matrix, classification_report
from sklearn.ensemble import RandomForestClassifier
from sklearn.externals import joblib
import itertools



# fix random seed for reproducibility
seed = 7
np.random.seed(seed)


# ## Data Import

print('Loading data...')

num_entries = 4000000
start = 0

inputfilename = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/workingData/FT2_AnalysisResults_Upgrade_DCAvec_PIDeffs_pairtree_us_part1_1-9-split_correctedPIDeffs.root"

branches_pairTree = [
    #'px1','py1','pz1',
    #'px2','py2','pz2',
    #'phiv',
    'mass',
    #'sumz',
    #'diffz',
    'opang',
    #'nITS1',
    'nITS2',
    'nITSshared1',
    'nITSshared2',
    #'nTPC1',
    #'nTPC2',
    'DCAx1',
    'DCAy1',
    'DCAz1',
    'DCAx2',
    'DCAy2',
    'DCAz2',
    #'DCAxy1', 'DCAxy2', #'DCAxy1_norm','DCAxy2_norm',
    ##'DCAz1_norm','DCAz2_norm',
    #'ITSchi21',
    'ITSchi22',
    #'TPCchi21',
    #'TPCchi22',
    #'pt1',
    'pt2',
    #'eta1',
    'eta2',
    #'phi1',
    #'phi2',
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
                                                     branches=branches_pairTree,
                                                     start=start,
                                                     stop=num_entries+start
))


print('Setting initial mass cuts...')
dataSample_orig = dataSample_orig.drop(dataSample_orig[dataSample_orig['mass']<.05].index)


print('Engineering features...')


# pairTree features
X = pd.DataFrame()
#X['p'] = np.sqrt((dataSample_orig['px1']+dataSample_orig['px2'])*(dataSample_orig['px1']+dataSample_orig['px2']) +
#                          (dataSample_orig['py1']+dataSample_orig['py2'])*(dataSample_orig['py1']+dataSample_orig['py2']) +
#                          (dataSample_orig['pz1']+dataSample_orig['pz2'])*(dataSample_orig['pz1']+dataSample_orig['pz2']))
#X['phiv'] = dataSample_orig['phiv'] - 1.57
#X['mass'] = dataSample_orig['mass']
#X['pz_diff'] = np.divide(dataSample_orig['pz1'], np.sqrt(dataSample_orig['px1']*dataSample_orig['px1'] + dataSample_orig['py1']*dataSample_orig['py1'])) - np.divide(dataSample_orig['pz2'], np.sqrt(dataSample_orig['px2']*dataSample_orig['px2'] + dataSample_orig['py2']*dataSample_orig['py2']))
#X['sumz'] = dataSample_orig['sumz']
#X['diffz'] = dataSample_orig['diffz'] - 1.57
X['opang'] = dataSample_orig['opang']
#X['nITS1'] = dataSample_orig['nITS1']
X['nITS2'] = dataSample_orig['nITS2']
X['nITSshared1'] = dataSample_orig['nITSshared1']
X['nITSshared2'] = dataSample_orig['nITSshared2']
#X['nTPC1'] = dataSample_orig['nTPC1']
#X['nTPC2'] = dataSample_orig['nTPC2']

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

#X['DCAxy1'] = np.log(np.abs(dataSample_orig['DCAxy1'])) #dataSample_orig['DCAxy1_norm']))
#X['DCAxy2'] = np.log(np.abs(dataSample_orig['DCAxy2'])) #dataSample_orig['DCAxy2_norm']))
X['DCAz1'] = np.abs(dataSample_orig['DCAz1'])
X['DCAz2'] = np.abs(dataSample_orig['DCAz2'])
#X['ITSchi21'] = dataSample_orig['ITSchi21']
X['ITSchi22'] = dataSample_orig['ITSchi22']
#X['TPCchi21'] = dataSample_orig['TPCchi21']
#X['TPCchi22'] = dataSample_orig['TPCchi22']
#X['pt1'] = dataSample_orig['pt1']
X['pt2'] = dataSample_orig['pt2']
#X['eta1'] = dataSample_orig['eta1']
X['eta2'] = dataSample_orig['eta2']
#X['phi1'] = dataSample_orig['phi1']
#X['phi2'] = dataSample_orig['phi2']

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
"""


X_featureNames = list(X)
print('Selected features:', X_featureNames)
joblib.dump(X_featureNames, 'temp_output/bdt/featureNames.pkl')


print('Calculating sample weights...')
sample_weight = (dataSample_orig['PIDeff1']*dataSample_orig['PIDeff2']).values.astype(np.float32)



# Preparation of the target vector (assign `1` and `0` to the entries, representing the signal and background classes, respectively):
Y = pd.DataFrame()
#Y = (dataSample_orig['pdgMother']!=22).astype(int)
Y = (~((dataSample_orig['IsRP']==0) & (dataSample_orig['IsConv']==1))).astype(int)

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

print('Scaling features to zero mean and unit variance...')
scaler = StandardScaler()
X = scaler.fit_transform(X)
Xfeats_mean = scaler.mean_
Xfeats_scale = scaler.scale_
Xfeats_var = scaler.var_

# store the (vstacked) array of shape (3,n_feats)
joblib.dump(np.array([Xfeats_mean, Xfeats_scale, Xfeats_var], dtype=np.float32),
            'temp_output/bdt/StandardScaler_attributes.pkl')


    
# ## Data Split in Training, Validation and Test Samples

print('Splitting the data in training, validation and test samples...')

X_train, X_test, y_train, y_test, sample_weight_train, sample_weight_test = train_test_split(X, Y, sample_weight, test_size=3000000, random_state=42)

X_train, X_val, y_train, y_val, sample_weight_train, sample_weight_val = train_test_split(X_train, y_train, sample_weight_train, test_size=.5, random_state=43)

print('Number of signal events in training sample: %d (%.2f percent)' % (y_train[y_train==1].shape[0], y_train[y_train==1].shape[0]*100/y_train.shape[0]))
print('Number of backgr events in training sample: %d (%.2f percent)' % (y_train[y_train==0].shape[0], y_train[y_train==0].shape[0]*100/y_train.shape[0]))

print('Number of signal events in validation sample: %d (%.2f percent)' % (y_val[y_val==1].shape[0], y_val[y_val==1].shape[0]*100/y_val.shape[0]))
print('Number of backgr events in validation sample: %d (%.2f percent)' % (y_val[y_val==0].shape[0], y_val[y_val==0].shape[0]*100/y_val.shape[0]))




# ## Model Definition and Training

# ### Boosted Decision Trees (Random Forest)

print('Creating random forest classifier...')

clf = RandomForestClassifier(n_jobs=-1,
                             n_estimators=300,
                             max_depth=None,
                             min_samples_split=50,
                             class_weight='balanced',
                             criterion='gini',
                             max_features=None
)


print('Training the classifier...')

plt.figure()
plt.xlabel('Number of training events (in millions)')
plt.ylabel('ROC AUC')
plt.ylim(.65,1.05)
for i in [1]:# [.1,.25,.4,.55,.7,.85,1]:
    #scores = cross_val_score(clf,
    #                         X_train[0:int(X_train.shape[0]*i),:],
    #                         y_train[0:int(y_train.shape[0]*i)],
    #                         cv=2,
    #                         scoring='f1_macro',
    #                         n_jobs=n_jobs,
    #                         pre_dispatch=pre_dispatch
    #                         )
    clf = clf.fit(X_train[0:int(X_train.shape[0]*i),:],
                  y_train[0:int(y_train.shape[0]*i)],
                  sample_weight=sample_weight_train[0:int(sample_weight_train.shape[0]*i)]
    )
    joblib.dump(clf, 'temp_output/bdt/clf_weights.pkl')
    current_auc = roc_auc_score(y_val[0:int(y_val.shape[0]*i)],
                                clf.predict_proba(X_val[0:int(X_val.shape[0]*i),:])[:,1],
                                sample_weight=sample_weight_val[0:int(sample_weight_val.shape[0]*i)]
    )
    print('Relative size of training sample: %.2f,\tROC AUC = %.3f' % (i, current_auc))
    plt.plot(int(X_train.shape[0]*i)/(1000000.0), current_auc,
             marker='o',
             markersize=10,
             color='#1f77b4',
             linestyle='none')
    plt.tight_layout()
    plt.savefig('temp_output/bdt/ROCAUC_sampleSize.png')

    print("Accuracy on training set:   {:.3f}".format(clf.score(X_train, y_train)))
    print("Accuracy on validation set: {:.3f}".format(clf.score(X_val, y_val)))

    
# ### Variable Importances

print('Calculating feature importances...')
importances = clf.feature_importances_
std = np.std([tree.feature_importances_ for tree in clf.estimators_],
             axis=0)
indices = np.argsort(importances)[::-1]
    
# Print the feature ranking
print("Feature ranking:")

indices_featureNames = np.empty([X_train.shape[1]], dtype=object)

for f in range(X_train.shape[1]):
    indices_featureNames[f] = X_featureNames[indices[f]]
    print("\t%d. %s \t(%f)" % (f + 1,
                               indices_featureNames[f],
                               importances[indices[f]]))
    
# Plot the feature importances of the forest
plt.figure()
plt.title("Feature importances")
plt.bar(range(X_train.shape[1]), importances[indices],
        color="r", yerr=std[indices], align="center")
plt.xticks(range(X_train.shape[1]), indices_featureNames, rotation=90)
plt.xlim([-1, X_train.shape[1]])
plt.tight_layout()
#plt.show()
plt.savefig('temp_output/bdt/var_importance.png')


del X_train, y_train, sample_weight_train


################################################################################


# ## Evaluation of Trained Model

print('Evaluating the trained model on the validation set...')

# general plotting parameters
nbins = 100


y_val_score = clf.predict_proba(X_val)


# ### MVA Output Distributions

def plot_MVAoutput(y_truth, y_score, label='', nbins=100):
    """
    Plots the MVA output as histogram and returns the underlying
    distributions of the positive and the negative class.
    """
    
    # split the array and use only the column containing the
    # probabilities of an event to belong the the "signal" class
    # (can be done in case of 'binary' classification)
    discard, y_score_posClass = np.split(y_score,
                                         2,
                                         axis=1)
    
    y_score_posClass_truePos = y_score_posClass[np.array(y_truth==1)]
    y_score_posClass_trueNeg = y_score_posClass[np.array(y_truth==0)]
    
    plt.figure()

    n_total, bins_total, patches_total = \
        plt.hist(y_score[:,1],
                 bins=nbins,
                 alpha=.25,
                 color='black',
                 label='MVA output')
    
    n_trueNeg, bins_trueNeg, patches_trueNeg = \
        plt.hist(y_score_posClass_trueNeg,
                 bins=nbins,
                 alpha=0.5,
                 color='#dd0000',
                 label='true negative')
    
    n_truePos, bins_truePos, patches_truePos = \
        plt.hist(y_score_posClass_truePos,
                 bins=nbins,
                 alpha=0.5,
                 color='green',
                 label='true positive')
    
    plt.title('MVA output distribution (positive class)')
    plt.xlim(-0.05, 1.05)
    plt.xlabel('MVA output')
    plt.ylabel('Entries')
    plt.legend()
    plt.savefig('temp_output/bdt/MVAoutput_distr_'+label+'.png')
    
    return n_truePos, n_trueNeg


print('Creating MVA output distributions...')
n_S, n_B = plot_MVAoutput(y_val, y_val_score, 'val', nbins)




# ### Cut efficiencies plot
plt.figure()
MVAcut = np.empty((0))

fig, ax1 = plt.subplots()
signal_efficiency = np.empty((0))
backgr_efficiency = np.empty((0))
for i in range(nbins):
    signal_efficiency = np.append(signal_efficiency, \
                                  np.sum(n_S[i:n_S.shape[0]]) / np.sum(n_S))
    backgr_efficiency = np.append(backgr_efficiency, \
                                  np.sum(n_B[i:n_B.shape[0]]) / np.sum(n_B))
    MVAcut = np.append(MVAcut, i/(nbins*1.0))
l1 = ax1.plot(MVAcut, signal_efficiency, label='signal efficiency', color='blue')
l2 = ax1.plot(MVAcut, backgr_efficiency, label='background efficiency', color='red')
ax1.set_xlabel('MVA cut')
ax1.set_ylabel('Efficiency')

ax2 = ax1.twinx()
significance_per_MVAcut = np.empty((0))
for i in range(nbins):
    significance_per_MVAcut = np.append(significance_per_MVAcut, \
                                        np.sum(n_S[i:n_S.shape[0]]) / \
                                        math.sqrt(np.sum(n_S[i:n_S.shape[0]]+n_B[i:n_B.shape[0]])))
    
l3 = ax2.plot(MVAcut, significance_per_MVAcut,
              label='significance',
              color='green')
pos_max = np.argmax(significance_per_MVAcut)
MVAcut_opt = pos_max/(nbins*1.0)
l4 = ax2.plot(pos_max/(nbins*1.0), significance_per_MVAcut[pos_max],
              label='max. significance for cut at %.2f' % MVAcut_opt,
              marker='o', markersize=10, fillstyle='none', mew=2, linestyle='none',
              color='#005500')
ax2.set_ylabel('Significance', color='green')
ax2.tick_params('y', colors='green')

plt.title('MVA cut efficiencies')
lall = l1+l2+l3+l4
labels = [l.get_label() for l in lall]
ax2.legend(lall, labels, loc='lower left')
plt.tight_layout()
plt.savefig('temp_output/bdt/significance_vs_MVAcut_val.png')


# ### ROC Curve

def plot_ROCcurve(y_truth, y_score, sample_weight=None, label='', workingpoint=-1, pos_label=1):
    """
    Plots the ROC curve and (if specified) the chosen working point.
    """

    # if not defined, do not use sample weights
    if(sample_weight==None):
        sample_weight = np.ones(y_truth.shape[0])
    
    fpr, tpr, thresholds = roc_curve(y_truth, y_score[:,1],
                                     sample_weight=sample_weight,
                                     pos_label=pos_label)
    roc_auc = roc_auc_score(y_truth, y_score[:,1], sample_weight=sample_weight)
    print('ROC AUC: %.3f' % roc_auc)
    
    plt.figure()
    plt.plot(fpr, tpr, label='ROC curve (AUC = %0.3f)' % roc_auc)
    plt.plot([0, 1], [0, 1], 'k--')
    plt.xlim([-0.05, 1.05])
    plt.ylim([-0.05, 1.05])
    plt.xlabel('False Positive Rate')
    plt.ylabel('True Positive Rate')
    plt.title('Receiver operating characteristic curve')
    
    if workingpoint != -1:
        # find and plot threshold closest to the chosen working point
        close_MVAcut_opt = np.argmin(np.abs(thresholds-workingpoint))
    
        plt.plot(fpr[close_MVAcut_opt], tpr[close_MVAcut_opt], 'o', markersize=10,
                 label="threshold at %.2f" % workingpoint, fillstyle="none",
                 mew=2)
    
    plt.legend(loc=4)
    plt.savefig('temp_output/bdt/roc_curve_'+label+'.png')


print('Generating ROC curve...')
plot_ROCcurve(y_val, y_val_score, sample_weight_val, 'val', MVAcut_opt)



# ### Precision-Recall Curve

def plot_precision_recall_curve(y_truth, y_score, sample_weight=None, label='', workingpoint=-1):
    """
    Plots the precision-recall curve.
    """

    # if not defined, do not use sample weights
    if(sample_weight==None):
        sample_weight = np.ones(y_truth.shape[0])
    
    precision = dict()
    recall = dict()
    average_precision = dict()
    
    precision, recall, thresholds_PRC = \
        precision_recall_curve(y_truth,
                               y_score[:,1],
                               sample_weight=sample_weight)
    
    average_precision = average_precision_score(y_truth, y_score[:,1], sample_weight=sample_weight)
    
    plt.figure()
    plt.plot(recall, precision, lw=2,
             label='Precision-recall curve of signal class (area = {1:0.2f})'
                    ''.format(1, average_precision))
    
    if workingpoint != -1:
        # find threshold closest to the chosen working point
        close_optimum = np.argmin(np.abs(thresholds_PRC-workingpoint))
        
        plt.plot(recall[close_optimum], precision[close_optimum],
                 'o',
                 markersize=10,
                 label="threshold at %.2f" % workingpoint,
                 fillstyle="none",
                 mew=2)
    
    plt.xlim([-0.05, 1.05])
    plt.ylim([-0.05, 1.05])
    plt.xlabel(r'Recall $R=T_p / (T_p+F_n)$')
    plt.ylabel(r'Precision $P=T_p / (T_p+F_p)$')
    plt.title('Precision-Recall Curve')
    plt.legend(loc="lower right")
    plt.savefig('temp_output/bdt/precision_recall_'+label+'.png')


print('Generating precision-recall curve...')
plot_precision_recall_curve(y_val, y_val_score, sample_weight_val, 'val', MVAcut_opt)



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

    plt.figure()
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
y_val_score_labels = (y_val_score[:,1]>MVAcut_opt)
cnf_matrix = confusion_matrix(y_val, y_val_score_labels, sample_weight=sample_weight_val)
np.set_printoptions(precision=2)



# Plot non-normalized confusion matrix
plot_confusion_matrix(cnf_matrix, classes=['background','signal'],
                      title='Confusion matrix (non-normalized)', label='val')
#plt.show()


# Plot normalized confusion matrix
plot_confusion_matrix(cnf_matrix, classes=['background','signal'],
                      normalize=True, title='Normalized confusion matrix', label='val')


# ### Classification Report


print('Classification report (validation sample_weight):')
print(classification_report(y_val, y_val_score_labels,
                            target_names=['background','signal']))


del y_val_score


################################################################################



# ## Test Model on New Data and Evaluate It

print('Evaluating the trained model on the test sample...')

y_test_score = clf.predict_proba(X_test)


# ### MVA Output Distribution
plot_MVAoutput(y_test, y_test_score, label='test', nbins=nbins)



# ### ROC Curve

print('Generating ROC curve...')
plot_ROCcurve(y_test, y_test_score, sample_weight_test, 'test', MVAcut_opt)


# ### Precision-Recall Curve

print('Generating precision-recall curve...')
plot_precision_recall_curve(y_test, y_test_score, sample_weight_test, 'test', MVAcut_opt)

# Compute confusion matrix
y_test_score_labels = (y_test_score[:,1]>MVAcut_opt) #thresholds[close_optimum]).astype(int)
cnf_matrix = confusion_matrix(y_test, y_test_score_labels, sample_weight=sample_weight_test)
np.set_printoptions(precision=2)


# Plot normalized confusion matrix
plot_confusion_matrix(cnf_matrix, classes=['background','signal'],
                      normalize=True, title='Normalized confusion matrix', label='test')

# Plot non-normalized confusion matrix
plot_confusion_matrix(cnf_matrix, classes=['background','signal'],
                      normalize=False, title='Confusion matrix (non-normalized)', label='test')


print('Classification report (test sample):')
print(classification_report(y_test, y_test_score_labels,
                            target_names=['background','signal']))
