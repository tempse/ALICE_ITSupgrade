import matplotlib.pyplot as plt
import seaborn as sns; sns.set()

import numpy as np
from numpy import trapz
import pandas as pd
import root_numpy

from sklearn.metrics import roc_curve, auc


filename_MCdata_CombConvRej = "~/analysis/data/FT2_AnalysisResults_Upgrade/workingData/FT2_AnalysisResults_Upgrade_DCAvec_PIDeffs_pairtree_us_part2_1-9-split_correctedPIDeffs.root"

branches_MCdata_CombConvRej = ['IsConv', 'mass', 'MVAoutput_CombConvRej_wPIDeffs_noMassCuts']



filename_MCdata_singleTrackConvRej = "~/analysis/data/FT2_AnalysisResults_Upgrade/workingData/FT2_AnalysisResults_Upgrade_addFeat_pairtree_us_part2_1-9-split_sklearnTrainingSingleTrackConvRej-wPIDeffs.root"

branches_MCdata_singleTrackConvRej = ['IsConv', 'mass', 'MVAoutput_convTrack1', 'MVAoutput_convTrack2', 'PIDeff1', 'PIDeff2']



filename_ROCdata_CombConvRej_RPConvRej = "~/analysis/data/FT2_AnalysisResults_Upgrade/workingData/ROCdata.root"

branches_ROCdata_CombConvRej_RPConvRej = ['tpr_prefilter', 'fpr_prefilter', 'tpr_noPrefilter', 'fpr_noPrefilter', 'MVAcut']


filename_ROCdata_combined = "~/analysis/data/FT2_AnalysisResults_Upgrade/workingData/ROCdata_combined.root"

branches_ROCdata_combined = ['tpr_combi', 'fpr_combi']


MVAcut_opt_CombConvRejMVA = .33
MVAcut_opt_singleConvTrackRejMVA = .31
MVAcut_opt_RPConvRejMVA = .47


color_RPConvRejClass = 'black'
color_RPConvRejMVA = '#dd0000'
color_singleConvTrackRej = '#4444ff'
color_CombConvRej = 'green'
color_combined = '#edbd00'
color_combined_highlighted = '#a38200'

fig_width = 6.83
fig_height = 5.12
fig_dpi = 150


# range delimiters for file inputs
start = 0
stop = 1640000


data_MCdata_singleTrackConvRej = pd.DataFrame(root_numpy.root2array(filename_MCdata_singleTrackConvRej,
                                                                    branches=branches_MCdata_singleTrackConvRej,
                                                                    start=start,
                                                                    stop=stop
))

data_MCdata_CombConvRej = pd.DataFrame(root_numpy.root2array(filename_MCdata_CombConvRej,
                                                             branches=branches_MCdata_CombConvRej))


data_ROCdata_CombConvRej_RPConvRej = pd.DataFrame(root_numpy.root2array(filename_ROCdata_CombConvRej_RPConvRej,
                                                               branches=branches_ROCdata_CombConvRej_RPConvRej))

data_ROCdata_combined = pd.DataFrame(root_numpy.root2array(filename_ROCdata_combined,
                                                           branches=branches_ROCdata_combined))



sample_weight_singleConvTrackRejMVA = np.ones(data_MCdata_singleTrackConvRej.shape[0])
sample_weight_singleConvTrackRejMVA[np.where(data_MCdata_singleTrackConvRej['MVAoutput_convTrack1'] <
                                             data_MCdata_singleTrackConvRej['MVAoutput_convTrack2'])] = data_MCdata_singleTrackConvRej['PIDeff1']
sample_weight_singleConvTrackRejMVA[np.where(data_MCdata_singleTrackConvRej['MVAoutput_convTrack1'] >=
                                             data_MCdata_singleTrackConvRej['MVAoutput_convTrack2'])] = data_MCdata_singleTrackConvRej['PIDeff2']

fpr_singleConvTrackRejMVA, tpr_singleConvTrackRejMVA, thresholds_singleConvTrackRejMVA = roc_curve(data_MCdata_singleTrackConvRej['IsConv'],
                                                                                                   np.minimum.reduce([data_MCdata_singleTrackConvRej['MVAoutput_convTrack1'],
                                                                                                                      data_MCdata_singleTrackConvRej['MVAoutput_convTrack2']]),
                                                                                                   sample_weight=sample_weight_singleConvTrackRejMVA,
                                                                                                   pos_label=0)

pos_workingpoint_opt_singleConvTrackRejMVA = np.argmin(np.abs(thresholds_singleConvTrackRejMVA -
                                                              MVAcut_opt_singleConvTrackRejMVA))

pos_workingpoint_opt_CombConvRejMVA = np.argmin(np.abs(data_ROCdata_CombConvRej_RPConvRej['MVAcut'] -
                                                       MVAcut_opt_CombConvRejMVA))

pos_workingpoint_opt_RPConvRejMVA = np.argmin(np.abs(data_ROCdata_CombConvRej_RPConvRej['MVAcut'] -
                                                     MVAcut_opt_RPConvRejMVA))


plt.figure(figsize=(fig_width,fig_height), dpi=fig_dpi)

ax = plt.gca()

ax.plot([0,1], [0,1], 'k--', color='#ababab')


p = (data_ROCdata_combined['fpr_combi']).argsort()
fpr_combi = data_ROCdata_combined['fpr_combi'][p]
tpr_combi = data_ROCdata_combined['tpr_combi'][p]


h_combi = ax.plot(fpr_combi, tpr_combi,
                  label='classifier combination (AUC = %.3f)' % (np.trapz(tpr_combi, fpr_combi)),
                  color=color_combined)


h_CombConvRejMVA = ax.plot(data_ROCdata_CombConvRej_RPConvRej['fpr_noPrefilter'],
                           data_ROCdata_CombConvRej_RPConvRej['tpr_noPrefilter'],
                           label='Comb. conv. rej. via MVA cuts (AUC = %.3f)' % (-np.trapz(data_ROCdata_CombConvRej_RPConvRej['tpr_noPrefilter'],
                                                                                          data_ROCdata_CombConvRej_RPConvRej['fpr_noPrefilter'])),
                           color=color_CombConvRej)

ax.plot(data_ROCdata_CombConvRej_RPConvRej.iloc[pos_workingpoint_opt_CombConvRejMVA]['fpr_noPrefilter'],
        data_ROCdata_CombConvRej_RPConvRej.iloc[pos_workingpoint_opt_CombConvRejMVA]['tpr_noPrefilter'],
        color=color_CombConvRej,
        marker='o',
        markersize=7,
        fillstyle='none',
        mew=1,
        linestyle='none')
print('CombConvRejMVA: working point at tpr = %.4f, fpr = %.4f' % (data_ROCdata_CombConvRej_RPConvRej.iloc[pos_workingpoint_opt_CombConvRejMVA]['tpr_noPrefilter'],
                                                                   data_ROCdata_CombConvRej_RPConvRej.iloc[pos_workingpoint_opt_CombConvRejMVA]['fpr_noPrefilter']))


h_singleConvTrackRejMVA = ax.plot(fpr_singleConvTrackRejMVA, tpr_singleConvTrackRejMVA,
                                  label='Single-track conv. rej. via MVA cuts (AUC = %.3f)' % (np.trapz(tpr_singleConvTrackRejMVA, fpr_singleConvTrackRejMVA)),
                                  color=color_singleConvTrackRej)

ax.plot(fpr_singleConvTrackRejMVA[pos_workingpoint_opt_singleConvTrackRejMVA],
        tpr_singleConvTrackRejMVA[pos_workingpoint_opt_singleConvTrackRejMVA],
        color=color_singleConvTrackRej,
        marker='o',
        markersize=7,
        fillstyle='none',
        mew=1,
        linestyle='none')
print('singleTrackConvRejMVA: working point at tpr = %.4f, fpr = %.4f' % (tpr_singleConvTrackRejMVA[pos_workingpoint_opt_singleConvTrackRejMVA],
                                                                          fpr_singleConvTrackRejMVA[pos_workingpoint_opt_singleConvTrackRejMVA]))


h_RPConvRejMVA = ax.plot(data_ROCdata_CombConvRej_RPConvRej['fpr_prefilter'], data_ROCdata_CombConvRej_RPConvRej['tpr_prefilter'],
                         label='RP conv. rej. via MVA cuts + prefiltering (AUC = %.3f)' % (-np.trapz(data_ROCdata_CombConvRej_RPConvRej['tpr_prefilter'],
                                                                                                    data_ROCdata_CombConvRej_RPConvRej['fpr_prefilter'])),
                         color=color_RPConvRejMVA)

ax.plot(data_ROCdata_CombConvRej_RPConvRej.iloc[pos_workingpoint_opt_RPConvRejMVA]['fpr_prefilter'],
        data_ROCdata_CombConvRej_RPConvRej.iloc[pos_workingpoint_opt_RPConvRejMVA]['tpr_prefilter'],
        color=color_RPConvRejMVA,
        marker='o',
        markersize=7,
        fillstyle='none',
        mew=1,
        linestyle='none')
print('RPConvRejMVA: working point at tpr = %.4f, fpr = %.4f' % (data_ROCdata_CombConvRej_RPConvRej.iloc[pos_workingpoint_opt_RPConvRejMVA]['tpr_prefilter'],
                                                                 data_ROCdata_CombConvRej_RPConvRej.iloc[pos_workingpoint_opt_RPConvRejMVA]['fpr_prefilter']))



# dummy point
h_optMarker = ax.plot(-99, -99,
                      label='optimized cuts',
                      color='#333333',
                      marker='o',
                      markersize=8,
                      fillstyle='none',
                      mew=1,
                      linestyle='none')




h_RPConvRejClass_1 = ax.plot(.490, .596, 'o', color=color_RPConvRejClass,
        label='RP conv. rej. via classical cuts: $\phi_V>\pi/2$, $mass<0.05$', markersize=8, alpha=.6)
h_RPConvRejClass_2 = ax.plot(.614, .776, 'o', color=color_RPConvRejClass,
        label='     $\phi_V>2$, $mass<0.04$', markersize=6, alpha=.6)
h_RPConvRejClass_3 = ax.plot(.721, .912, 'o', color=color_RPConvRejClass,
        label='     $\phi_V>2.4$, $mass<0.01$', markersize=4, alpha=.6)
h_RPConvRejClass_4 = ax.plot(.936, .986, 'o', color=color_RPConvRejClass,
        label='     $\phi_V>2.9$, $mass<0.0035$', markersize=3, alpha=.6)



plt.xlim([-0.05, 1.05])
plt.ylim([-0.05, 1.05])
plt.xlabel('False Positive Rate')
plt.ylabel('True Positive Rate')
plt.title('Receiver operating characteristics')

handles, labels = ax.get_legend_handles_labels()
handles_temp, labels_temp = handles[0], labels[0]
handles, labels = np.delete(handles, 0), np.delete(labels, 0)
handles, labels = np.insert(handles, 4, handles_temp), np.insert(labels, 4, labels_temp)

plt.legend(handles, labels, loc=4, fontsize=7)


plt.savefig('temp_output/ROCcurves_entireMassRange.png')
plt.savefig('temp_output/ROCcurves_entireMassRange.pdf')