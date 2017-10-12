import matplotlib.pyplot as plt
import seaborn as sns; sns.set()

import numpy as np
from numpy import trapz
import pandas as pd
import root_numpy

from sklearn.metrics import roc_curve, auc


filename_MCdata = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/properPrefiltering/workingData/testSample/temp_output/FT2_ITSup_pairTree-us_part2_538163tightCutEvents.root"

branches_MCdata = ['IsConv']



filename_MCdata_singleTrackConvRej = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/properPrefiltering/singleConvTrackRejMVA/pairing/temp_output/FT2_ITSup_pairTree_us_part2_538163tightCutEvents_wSingleConvTrackMVAoutput.root"

branches_MCdata_singleTrackConvRej = ['IsConv', 'MVAoutput_convTrack1', 'MVAoutput_convTrack2', 'PIDeff1', 'PIDeff2', 'TrackCut1', 'TrackCut2']



filename_ROCdata_CombConvRej_RPConvRej = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/properPrefiltering_wPIDeffs/ROCdata/ROCdata_25steps_PIDeffs/temp_output/ROCdata_MVAplusPrefilter.root"

branches_ROCdata_CombConvRej_RPConvRej = ['tpr_prefilter', 'fpr_prefilter', 'tpr_noPrefilter', 'fpr_noPrefilter', 'MVAcut']


filename_ROCdata_RPConvRej_wLooseCuts = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/properPrefiltering_wPIDeffs/ROCdata/ROCdata_25steps_PIDeffs_wLooseTracks/temp_output/ROCdata_MVAplusPrefilter.root"

branches_ROCdata_RPConvRej_wLooseCuts = ['tpr_prefilter', 'fpr_prefilter', 'MVAcut']


filename_ROCdata_RPConvRej_wLooseCuts_wAllTracks = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/properPrefiltering_wPIDeffs/ROCdata/ROCdata_25steps_PIDeffs_wLooseTracks_allTracks/temp_output/ROCdata_MVAplusPrefilter.root"

branches_ROCdata_RPConvRej_wLooseCuts_wAllTracks = ['tpr_prefilter', 'fpr_prefilter', 'MVAcut']


filename_ROCdata_combined = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/properPrefiltering_wPIDeffs/ROCdata/ROCdata_25steps_PIDeffs/temp_output/ROCdata_MVAplusPrefilter_combined.root"

branches_ROCdata_combined = ['tpr_combi', 'fpr_combi']


MVAcut_opt_CombConvRejMVA = .23
MVAcut_opt_singleConvTrackRejMVA = .44
MVAcut_opt_RPConvRejMVA = .38
MVAcut_opt_RPConvRejMVA_wLooseCuts = .32
MVAcut_opt_RPConvRejMVA_wLooseCuts_wAllTracks = .33


color_RPConvRejClass = 'black'
color_RPConvRejMVA = '#4444ff'
color_RPConvRejMVA_wLooseCuts = 'black'
color_RPConvRejMVA_wLooseCuts_wAllTracks = 'black'
color_singleConvTrackRej = '#dd0000'
color_CombConvRej = 'green'
color_combined = '#edbd00'
color_combined_highlighted = '#a38200'

fig_width = 6.83
fig_height = 5.12
fig_dpi = 150


# range delimiters for file inputs
start = 0
stop = 50000000


data_MCdata_singleTrackConvRej = pd.DataFrame(root_numpy.root2array(filename_MCdata_singleTrackConvRej,
                                                                    branches=branches_MCdata_singleTrackConvRej,
                                                                    start=start,
                                                                    stop=stop,
                                                                    selection='TrackCut1==2 && TrackCut2==2 && MVAoutput_convTrack1>=0 && MVAoutput_convTrack1<=1 && MVAoutput_convTrack2>=0 && MVAoutput_convTrack2<=1'
))

data_MCdata = pd.DataFrame(root_numpy.root2array(filename_MCdata,
                                                 branches=branches_MCdata))


data_ROCdata_CombConvRej_RPConvRej = pd.DataFrame(root_numpy.root2array(filename_ROCdata_CombConvRej_RPConvRej,
                                                                        branches=branches_ROCdata_CombConvRej_RPConvRej))

data_ROCdata_RPConvRej_wLooseCuts = pd.DataFrame(root_numpy.root2array(filename_ROCdata_RPConvRej_wLooseCuts,
                                                                       branches=branches_ROCdata_RPConvRej_wLooseCuts))

data_ROCdata_RPConvRej_wLooseCuts_wAllTracks = pd.DataFrame(root_numpy.root2array(filename_ROCdata_RPConvRej_wLooseCuts_wAllTracks,
                                                                                  branches=branches_ROCdata_RPConvRej_wLooseCuts_wAllTracks))

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

pos_workingpoint_opt_RPConvRejMVA_wLooseCuts = np.argmin(np.abs(data_ROCdata_RPConvRej_wLooseCuts['MVAcut'] -
                                                                MVAcut_opt_RPConvRejMVA_wLooseCuts))

pos_workingpoint_opt_RPConvRejMVA_wLooseCuts_wAllTracks = np.argmin(np.abs(data_ROCdata_RPConvRej_wLooseCuts_wAllTracks['MVAcut'] -
                                                                           MVAcut_opt_RPConvRejMVA_wLooseCuts_wAllTracks))


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
                           label='MVA 1 (AUC = %.3f)' % (-np.trapz(data_ROCdata_CombConvRej_RPConvRej['tpr_noPrefilter'],
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
                                  label='MVA 2 (AUC = %.3f)' % (np.trapz(tpr_singleConvTrackRejMVA, fpr_singleConvTrackRejMVA)),
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


h_RPConvRejMVA = ax.plot(pd.Series.append(data_ROCdata_CombConvRej_RPConvRej['fpr_prefilter'], pd.Series([0])),
                         pd.Series.append(data_ROCdata_CombConvRej_RPConvRej['tpr_prefilter'], pd.Series([0])),
                         label='MVA 3 (AUC = %.3f)' % (-np.trapz(pd.Series.append(data_ROCdata_CombConvRej_RPConvRej['tpr_prefilter'], pd.Series([0])),
                                                                 pd.Series.append(data_ROCdata_CombConvRej_RPConvRej['fpr_prefilter'], pd.Series([0])))),
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

h_RPConvRejMVA_wLooseCuts = ax.plot(pd.Series.append(data_ROCdata_RPConvRej_wLooseCuts['fpr_prefilter'], pd.Series([0])),
                                    pd.Series.append(data_ROCdata_RPConvRej_wLooseCuts['tpr_prefilter'], pd.Series([0])),
                                    label='MVA 3 with loose-cuts tracks (AUC = %.3f)' % (-np.trapz(pd.Series.append(data_ROCdata_RPConvRej_wLooseCuts['tpr_prefilter'], pd.Series([0])),
                                                                                                   pd.Series.append(data_ROCdata_RPConvRej_wLooseCuts['fpr_prefilter'], pd.Series([0])))),
                                    color=color_RPConvRejMVA_wLooseCuts,
                                    linestyle='-',
                                    alpha=.3)

ax.plot(data_ROCdata_RPConvRej_wLooseCuts.iloc[pos_workingpoint_opt_RPConvRejMVA_wLooseCuts]['fpr_prefilter'],
        data_ROCdata_RPConvRej_wLooseCuts.iloc[pos_workingpoint_opt_RPConvRejMVA_wLooseCuts]['tpr_prefilter'],
        color=color_RPConvRejMVA_wLooseCuts,
        marker='o',
        markersize=7,
        fillstyle='none',
        mew=1,
        linestyle='none',
        alpha=.45)
print('RPConvRejMVA (w/ loose tracks): working point at tpr = %.4f, fpr = %.4f' % (data_ROCdata_RPConvRej_wLooseCuts.iloc[pos_workingpoint_opt_RPConvRejMVA_wLooseCuts]['tpr_prefilter'],
                                                                                   data_ROCdata_RPConvRej_wLooseCuts.iloc[pos_workingpoint_opt_RPConvRejMVA_wLooseCuts]['fpr_prefilter']))

h_RPConvRejMVA_wLooseCuts_wAllTracks = ax.plot(pd.Series.append(data_ROCdata_RPConvRej_wLooseCuts_wAllTracks['fpr_prefilter'], pd.Series([0])),
                                               pd.Series.append(data_ROCdata_RPConvRej_wLooseCuts_wAllTracks['tpr_prefilter'], pd.Series([0])),
                                                label='MVA 3 with loose-cuts and all other tracks (AUC = %.3f)' % (-np.trapz(pd.Series.append(data_ROCdata_RPConvRej_wLooseCuts_wAllTracks['tpr_prefilter'], pd.Series([0])),
                                                                                                                             pd.Series.append(data_ROCdata_RPConvRej_wLooseCuts_wAllTracks['fpr_prefilter'], pd.Series([0])))),
                                                color=color_RPConvRejMVA_wLooseCuts_wAllTracks,
                                               linestyle=':',
                                               alpha=.55)

ax.plot(data_ROCdata_RPConvRej_wLooseCuts_wAllTracks.iloc[pos_workingpoint_opt_RPConvRejMVA_wLooseCuts_wAllTracks]['fpr_prefilter'],
        data_ROCdata_RPConvRej_wLooseCuts_wAllTracks.iloc[pos_workingpoint_opt_RPConvRejMVA_wLooseCuts_wAllTracks]['tpr_prefilter'],
        color=color_RPConvRejMVA_wLooseCuts_wAllTracks,
        marker='o',
        markersize=7,
        fillstyle='none',
        mew=1,
        linestyle='none',
        alpha=.8)
print('RPConvRejMVA (w/ loose tracks, all other tracks): working point at tpr = %.4f, fpr = %.4f' % (data_ROCdata_RPConvRej_wLooseCuts_wAllTracks.iloc[pos_workingpoint_opt_RPConvRejMVA_wLooseCuts_wAllTracks]['tpr_prefilter'],
                                                                                                     data_ROCdata_RPConvRej_wLooseCuts_wAllTracks.iloc[pos_workingpoint_opt_RPConvRejMVA_wLooseCuts_wAllTracks]['fpr_prefilter']))

# dummy point
h_optMarker = ax.plot(-99, -99,
                      label='optimized cuts',
                      color='#333333',
                      marker='o',
                      markersize=8,
                      fillstyle='none',
                      mew=1,
                      linestyle='none')



"""
# case: prefilter w/o PID effs
h_RPConvRejClass_1 = ax.plot(.4527, .5547, 'o', color=color_RPConvRejClass,
                             label=r'RP conv. rej. via classical cuts: $\varphi_V>\pi/2$, $m_{ee}<0.05$', markersize=8, alpha=.8)
h_RPConvRejClass_2 = ax.plot(.5620, .7086, 'o', color=color_RPConvRejClass,
                             label=r'     $\varphi_V>2$, $m_{ee}<0.04$', markersize=6, alpha=.8)
h_RPConvRejClass_3 = ax.plot(.7200, .9110, 'o', color=color_RPConvRejClass,
                             label=r'     $\varphi_V>2.4$, $m_{ee}<0.01$', markersize=4, alpha=.8)
h_RPConvRejClass_4 = ax.plot(.9367, .9860, 'o', color=color_RPConvRejClass,
                             label=r'     $\varphi_V>2.9$, $m_{ee}<0.0035$', markersize=3, alpha=.8)
"""

# case: prefilter w/ PID effs
h_RPConvRejClass_1 = ax.plot(.9424, .9585, 'o', color=color_RPConvRejClass,
                             label=r'RP conv. rej. via classical cuts: $\varphi_V>\pi/2$, $m_{ee}<0.05$', markersize=8, alpha=.8)
h_RPConvRejClass_2 = ax.plot(.9540, .9737, 'o', color=color_RPConvRejClass,
                             label=r'     $\varphi_V>2$, $m_{ee}<0.04$', markersize=6, alpha=.8)
h_RPConvRejClass_3 = ax.plot(.9691, .9909, 'o', color=color_RPConvRejClass,
                             label=r'     $\varphi_V>2.4$, $m_{ee}<0.01$', markersize=4, alpha=.8)
h_RPConvRejClass_4 = ax.plot(.9934, .9986, 'o', color=color_RPConvRejClass,
                             label=r'     $\varphi_V>2.9$, $m_{ee}<0.0035$', markersize=3, alpha=.8)



plt.xlim([-0.05, 1.05])
plt.ylim([-0.05, 1.05])
plt.xlabel('False Positive Rate', fontsize=18)
plt.ylabel('True Positive Rate', fontsize=18)
#plt.title('Receiver operating characteristics')

handles, labels = ax.get_legend_handles_labels()
handles_temp, labels_temp = handles[0], labels[0]
handles, labels = np.delete(handles, 0), np.delete(labels, 0)
handles, labels = np.insert(handles, 5, handles_temp), np.insert(labels, 5, labels_temp)

plt.legend(handles, labels, loc=4, fontsize=7.5)

plt.tight_layout()

plt.savefig('temp_output/ROCcurves.png')
plt.savefig('temp_output/ROCcurves.pdf')
