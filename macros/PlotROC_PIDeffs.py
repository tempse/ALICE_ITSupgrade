import matplotlib.pyplot as plt
import seaborn as sns; sns.set()

import numpy as np
import pandas as pd
import root_numpy

from sklearn.metrics import roc_curve, roc_auc_score


filename_MCdata = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/workingData/FT2_AnalysisResults_Upgrade_DCAvec_PIDeffs_pairtree_us_part2_1-9-split.root"

branches_MCdata = [
    #'IsRP',
    'IsConv',
    'mass',
    'PIDeff1',
    'PIDeff2'
]


filename_MVAoutput_woPIDeffs = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/sklearn_BDT_analysis/randomForest/woSampleWeights_PIDeffs_noMassCuts/clf_predict/predictions_BDT.root"

branches_MVAoutput_woPIDeffs = ['BDT']


filename_MVAoutput_wPIDeffs = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/sklearn_BDT_analysis/randomForest/wSampleWeights_PIDeffs_noMassCuts/clf_predict/predictions_BDT.root"

branches_MVAoutput_wPIDeffs = ['BDT']


start = 0
stop = 75000000


fig_width = 6.83
fig_height = 5.12
fig_dpi = 150

color_woPIDeffs = 'black'
color_wPIDeffs = 'orange'


df_MCdata = pd.DataFrame(root_numpy.root2array(filename_MCdata,
                                               branches=branches_MCdata,
                                               start=start,
                                               stop=stop))

df_MVAoutput_woPIDeffs = pd.DataFrame(root_numpy.root2array(filename_MVAoutput_woPIDeffs,
                                                            branches=branches_MVAoutput_woPIDeffs,
                                                            start=start,
                                                            stop=stop))

df_MVAoutput_wPIDeffs = pd.DataFrame(root_numpy.root2array(filename_MVAoutput_wPIDeffs,
                                                           branches=branches_MVAoutput_wPIDeffs,
                                                           start=start,
                                                           stop=stop))


sample_weight = (df_MCdata['PIDeff1']*df_MCdata['PIDeff2']).values.astype(np.float32)



fpr_woPIDeffs, tpr_woPIDeffs, thresholds_woPIDeffs = roc_curve(df_MCdata['IsConv'],
                                                               df_MVAoutput_woPIDeffs['BDT'],
                                                               pos_label=0)
roc_auc_woPIDeffs = roc_auc_score(df_MCdata['IsConv'],
                                  df_MVAoutput_woPIDeffs['BDT'])


fpr_wPIDeffs, tpr_wPIDeffs, thresholds_wPIDeffs = roc_curve(df_MCdata['IsConv'],
                                                            df_MVAoutput_wPIDeffs['BDT'],
                                                            pos_label=0,
                                                            sample_weight=sample_weight)
roc_auc_wPIDeffs = roc_auc_score(df_MCdata['IsConv'],
                                 df_MVAoutput_wPIDeffs['BDT'],
                                 sample_weight=sample_weight)


plt.figure(figsize=(fig_width,fig_height), dpi=fig_dpi)
plt.plot(fpr_woPIDeffs, tpr_woPIDeffs,
         label='excl. PID efficiences (area = %.3f)' % (1.-roc_auc_woPIDeffs),
         alpha=.85,
         color=color_woPIDeffs)
plt.plot(fpr_wPIDeffs, tpr_wPIDeffs,
         label='incl. PID efficiencies (area = %.3f)' % (1.-roc_auc_wPIDeffs),
         alpha=.85,
         color=color_wPIDeffs)
plt.plot([0,1], [0,1], 'k--')
plt.xlim([0.0, 1.05])
plt.ylim([0.0, 1.05])
plt.xlabel('False Positive Rate')
plt.ylabel('True Positive Rate')
plt.title('Receiver operating characteristic curve (no mass cuts)')
plt.legend(loc=4)
plt.savefig('temp_output/ROCcurves_PIDeffs_CombConvRej_masscut.png')
plt.savefig('temp_output/ROCcurves_PIDeffs_CombConvRej_masscut.pdf')
