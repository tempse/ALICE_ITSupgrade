import matplotlib.pyplot as plt
import seaborn as sns; sns.set()

import numpy as np
import pandas as pd
import root_numpy

from sklearn.metrics import roc_curve, roc_auc_score


################################################################################


filename_MCdata = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/workingData/FT2_AnalysisResults_Upgrade_DCAvec_PIDeffs_pairtree_us_part2_1-9-split.root"

branches_MCdata = [
    #'IsRP',
    'IsConv',
    'mass',
    'PIDeff1',
    'PIDeff2'
]


filename_MVAoutput_woPIDeffs_noMassCuts = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/sklearn_BDT_analysis/randomForest/woSampleWeights_PIDeffs_noMassCuts/clf_predict/predictions_BDT.root"

branches_MVAoutput_woPIDeffs_noMassCuts = ['BDT']


filename_MVAoutput_wPIDeffs_noMassCuts = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/sklearn_BDT_analysis/randomForest/wSampleWeights_PIDeffs_noMassCuts/clf_predict/predictions_BDT.root"

branches_MVAoutput_wPIDeffs_noMassCuts = ['BDT']


filename_MVAoutput_woPIDeffs_massCut = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/sklearn_BDT_analysis/randomForest/woSampleWeights_PIDeffs_0.5mass/clf_predict/predictions_BDT.root"

branches_MVAoutput_woPIDeffs_massCut = ['BDT']

filename_MVAoutput_wPIDeffs_massCut = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/sklearn_BDT_analysis/randomForest/wSampleWeights_PIDeffs_0.5mass/clf_predict/predictions_BDT.root"

branches_MVAoutput_wPIDeffs_massCut = ['BDT']



start = 0
stop = 75000000

massCut = .05

fig_width = 6.83
fig_height = 5.12
fig_dpi = 150

color_woPIDeffs = 'black'
color_wPIDeffs = 'orange'


################################################################################


df_MCdata = pd.DataFrame(root_numpy.root2array(filename_MCdata,
                                               branches=branches_MCdata,
                                               start=start,
                                               stop=stop))

##### entire mass range #####

df_MVAoutput_woPIDeffs_noMassCuts = pd.DataFrame(root_numpy.root2array(filename_MVAoutput_woPIDeffs_noMassCuts,
                                                                       branches=branches_MVAoutput_woPIDeffs_noMassCuts,
                                                                       start=start,
                                                                       stop=stop))

df_MVAoutput_wPIDeffs_noMassCuts = pd.DataFrame(root_numpy.root2array(filename_MVAoutput_wPIDeffs_noMassCuts,
                                                                      branches=branches_MVAoutput_wPIDeffs_noMassCuts,
                                                                      start=start,
                                                                      stop=stop))


sample_weight = (df_MCdata['PIDeff1']*df_MCdata['PIDeff2']).values.astype(np.float32)



fpr_woPIDeffs_noMassCuts, tpr_woPIDeffs_noMassCuts, thresholds_woPIDeffs_noMassCuts = roc_curve(df_MCdata['IsConv'],
                                                                                                df_MVAoutput_woPIDeffs_noMassCuts['BDT'],
                                                                                                pos_label=0)
roc_auc_woPIDeffs_noMassCuts = roc_auc_score(df_MCdata['IsConv'],
                                             df_MVAoutput_woPIDeffs_noMassCuts['BDT'])


fpr_wPIDeffs_noMassCuts, tpr_wPIDeffs_noMassCuts, thresholds_wPIDeffs_noMassCuts = roc_curve(df_MCdata['IsConv'],
                                                                                             df_MVAoutput_wPIDeffs_noMassCuts['BDT'],
                                                                                             pos_label=0,
                                                                                             sample_weight=sample_weight)
roc_auc_wPIDeffs_noMassCuts = roc_auc_score(df_MCdata['IsConv'],
                                            df_MVAoutput_wPIDeffs_noMassCuts['BDT'],
                                            sample_weight=sample_weight)


plt.figure(figsize=(fig_width,fig_height), dpi=fig_dpi)
plt.plot(fpr_woPIDeffs_noMassCuts, tpr_woPIDeffs_noMassCuts,
         label='excl. PID efficiences (area = %.3f)' % (1.-roc_auc_woPIDeffs_noMassCuts),
         alpha=.85,
         color=color_woPIDeffs)
plt.plot(fpr_wPIDeffs_noMassCuts, tpr_wPIDeffs_noMassCuts,
         label='incl. PID efficiencies (area = %.3f)' % (1.-roc_auc_wPIDeffs_noMassCuts),
         alpha=.85,
         color=color_wPIDeffs)
plt.plot([0,1], [0,1], 'k--')
plt.xlim([0.0, 1.05])
plt.ylim([0.0, 1.05])
plt.xlabel('False Positive Rate')
plt.ylabel('True Positive Rate')
plt.title('Receiver operating characteristic curve (no mass cuts)')
plt.legend(loc=4)
plt.savefig('temp_output/ROCcurves_PIDeffs_CombConvRej_noMassCuts.png')
plt.savefig('temp_output/ROCcurves_PIDeffs_CombConvRej_noMassCuts.pdf')




##### mass > 50 MeV/c^2 #####


df_MVAoutput_woPIDeffs_massCut = pd.DataFrame(root_numpy.root2array(filename_MVAoutput_woPIDeffs_massCut,
                                                                    branches=branches_MVAoutput_woPIDeffs_massCut,
                                                                    start=start,
                                                                    stop=stop))

df_MVAoutput_wPIDeffs_massCut = pd.DataFrame(root_numpy.root2array(filename_MVAoutput_wPIDeffs_massCut,
                                                                   branches=branches_MVAoutput_wPIDeffs_massCut,
                                                                   start=start,
                                                                   stop=stop))


sample_weight = (df_MCdata['PIDeff1']*df_MCdata['PIDeff2']).values.astype(np.float32)


# process data according to mass cuts:
df_MVAoutput_woPIDeffs_massCut = df_MVAoutput_woPIDeffs_massCut[np.array(df_MCdata['mass']>massCut)]
df_MVAoutput_wPIDeffs_massCut = df_MVAoutput_wPIDeffs_massCut[np.array(df_MCdata['mass']>massCut)]
df_MCdata = df_MCdata[np.array(df_MCdata['mass']>massCut)]


fpr_woPIDeffs_massCut, tpr_woPIDeffs_massCut, thresholds_woPIDeffs_massCut = roc_curve(df_MCdata['IsConv'], #((df_MCdata['IsRP']==0) & (df_MCdata['IsConv']==1)).astype(int)
                                                                                       df_MVAoutput_woPIDeffs_massCut['BDT'],
                                                                                       pos_label=0)
roc_auc_woPIDeffs_massCut = roc_auc_score(df_MCdata['IsConv'],
                                          df_MVAoutput_woPIDeffs_massCut['BDT'])


fpr_wPIDeffs_massCut, tpr_wPIDeffs_massCut, thresholds_wPIDeffs_massCut = roc_curve(df_MCdata['IsConv'], #((df_MCdata['IsRP']==0) & (df_MCdata['IsConv']==1)).astype(int)
                                                                                    df_MVAoutput_wPIDeffs_massCut['BDT'],
                                                                                    pos_label=0,
                                                                                    sample_weight=sample_weight)
roc_auc_wPIDeffs_massCut = roc_auc_score(df_MCdata['IsConv'],
                                         df_MVAoutput_wPIDeffs_massCut['BDT'],
                                         sample_weight=sample_weight)


plt.figure(figsize=(fig_width,fig_height), dpi=fig_dpi)
plt.plot(fpr_woPIDeffs_massCut, tpr_woPIDeffs_massCut,
         label='excl. PID efficiences (area = %.3f)' % (1.-roc_auc_woPIDeffs_massCut),
         alpha=.85,
         color=color_woPIDeffs)
plt.plot(fpr_wPIDeffs_massCut, tpr_wPIDeffs_massCut,
         label='incl. PID efficiencies (area = %.3f)' % (1.-roc_auc_wPIDeffs_massCut),
         alpha=.85,
         color=color_wPIDeffs)
plt.plot([0,1], [0,1], 'k--')
plt.xlim([0.0, 1.05])
plt.ylim([0.0, 1.05])
plt.xlabel('False Positive Rate')
plt.ylabel('True Positive Rate')
plt.title('Receiver operating characteristic curve (mass > 50 MeV/c$^{2}$)')
plt.legend(loc=4)
plt.savefig('temp_output/ROCcurves_PIDeffs_CombConvRej_massCut.png')
plt.savefig('temp_output/ROCcurves_PIDeffs_CombConvRej_massCut.pdf')
