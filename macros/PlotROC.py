import matplotlib.pyplot as plt
import seaborn as sns; sns.set()

import numpy as np
import pandas as pd
import root_numpy

from sklearn.metrics import roc_curve, auc


filename_MCdata_CombConvRej = "~/analysis/data/FT2_AnalysisResults_Upgrade/workingData/FT2_AnalysisResults_Upgrade_DCAvec_PIDeffs_pairtree_us_part2_1-9-split_correctedPIDeffs.root"

branches_MCdata_CombConvRej = ['IsConv', 'mass', 'MVAoutput_CombConvRej_wPIDeffs_noMassCuts']



filename_MCdata_singleTrackConvRej = "~/analysis/data/FT2_AnalysisResults_Upgrade/workingData/FT2_AnalysisResults_Upgrade_addFeat_pairtree_us_part2_1-9-split_sklearnTrainingSingleTrackConvRej-wPIDeffs.root"

branches_MCdata_singleTrackConvRej = ['IsConv', 'mass', 'MVAoutput_convTrack1', 'MVAoutput_convTrack2']



filename_ROCdata_RPConvRejMVA = "~/analysis/data/FT2_AnalysisResults_Upgrade/workingData/ROCdata_MVAplusPrefilter.root"

branches_ROCdata_RPConvRejMVA = ['tpr_prefilter', 'fpr_prefilter']


filename_ROCdata_combined = "~/analysis/data/FT2_AnalysisResults_Upgrade/workingData/ROCdata_MVAplusPrefilter_combined.root"

branches_ROCdata_combined = ['tpr_combi', 'fpr_combi']



color_RPConvRejClass = '#666666'
color_RPConvRejMVA = '#dd0000'
color_singleConvTrackRej = '#4444ff'
color_CombConvRej = 'green'
color_combined = '#edbd00'
color_combined_highlighted = '#a38200'

fig_width = 6.83
fig_height = 5.12
fig_dpi = 150


## range delimiters for file inputs
#start = 0
#stop = 100000000


data_MCdata_singleTrackConvRej = pd.DataFrame(root_numpy.root2array(filename_MCdata_singleTrackConvRej,
                                                                    branches=branches_MCdata_singleTrackConvRej,
                                                                    #start=start,
                                                                    #stop=stop
))

data_MCdata_CombConvRej = pd.DataFrame(root_numpy.root2array(filename_MCdata_CombConvRej,
                                                             branches=branches_MCdata_CombConvRej,
                                                             #start=start,
                                                             #stop=stop
))


data_ROCdata_RPConvRejMVA = pd.DataFrame(root_numpy.root2array(filename_ROCdata_RPConvRejMVA,
                                                               branches=branches_ROCdata_RPConvRejMVA,
                                                               #start=start,
                                                               #stop=stop
))

data_ROCdata_combined = pd.DataFrame(root_numpy.root2array(filename_ROCdata_combined,
                                                           branches=branches_ROCdata_combined,
                                                           #start=start,
                                                           #stop=stop
))



fpr_CombConvRejMVA, tpr_CombConvRejMVA, thresholds_CombConvRejMVA = roc_curve(data_MCdata_CombConvRej['IsConv'], data_MCdata_CombConvRej['MVAoutput_CombConvRej_wPIDeffs_noMassCuts'], pos_label=0)
roc_auc_CombConvRejMVA = auc(fpr_CombConvRejMVA, tpr_CombConvRejMVA)

fpr_singleConvTrackRejMVA, tpr_singleConvTrackRejMVA, thresholds_singleConvTrackRejMVA = roc_curve(data_MCdata_singleTrackConvRej['IsConv'],
                                                                                                   np.minimum.reduce([data_MCdata_singleTrackConvRej['MVAoutput_convTrack1'],
                                                                                                                      data_MCdata_singleTrackConvRej['MVAoutput_convTrack2']]),
                                                                                                   pos_label=0)
roc_auc_singleConvTrackRejMVA = auc(fpr_singleConvTrackRejMVA, tpr_singleConvTrackRejMVA)



plt.figure(figsize=(fig_width,fig_height), dpi=fig_dpi)

plt.plot([0,1], [0,1], 'k--', color='#ababab')


plt.plot(fpr_CombConvRejMVA, tpr_CombConvRejMVA,
         label='Comb. conv. rej. via MVA cuts (area = %.3f)' % roc_auc_CombConvRejMVA,
         color=color_CombConvRej)

plt.plot(.6712, .9697,
         color=color_CombConvRej,
         marker='o',
         markersize=8,
         fillstyle='none',
         mew=1,
         linestyle='none')


plt.plot(fpr_singleConvTrackRejMVA, tpr_singleConvTrackRejMVA,
         label='Single-track conv. rej. via MVA cuts (area = %.3f)' % roc_auc_singleConvTrackRejMVA,
         color=color_singleConvTrackRej)

plt.plot(.6763, .9698,
         color=color_singleConvTrackRej,
         marker='o',
         markersize=8,
         fillstyle='none',
         mew=1,
         linestyle='none')


plt.plot(data_ROCdata_RPConvRejMVA['fpr_prefilter'], data_ROCdata_RPConvRejMVA['tpr_prefilter'],
         label='RP conv. rej. via MVA cuts + prefiltering',
         color=color_RPConvRejMVA)



# dummy point
plt.plot(-99, -99,
         label='optimized cuts',
         color='#333333',
         marker='o',
         markersize=8,
         fillstyle='none',
         mew=1,
         linestyle='none')



plt.plot(.7056, .9361,
         #label='RP conv. rej. via MVA cuts + prefiltering: optimized cut',
         color=color_RPConvRejMVA,
         marker='o',
         markersize=8,
         fillstyle='none',
         mew=1,
         linestyle='none')


plt.plot(data_ROCdata_combined['fpr_combi'], data_ROCdata_combined['tpr_combi'],
         label='classifier combinations',
         color=color_combined,
         linestyle='none',
         marker='o',
         markersize=2,
         mew=2)



plt.plot(.447, .532, 'o', color=color_RPConvRejClass,
         label='RP conv. rej. via classical cuts: $\phi_V>\pi/2$, $mass<0.05$', markersize=8)
plt.plot(.613, .759, 'o', color=color_RPConvRejClass,
         label='     $\phi_V>2$, $mass<0.04$', markersize=6)
plt.plot(.737, .916, 'o', color=color_RPConvRejClass,
         label='     $\phi_V>2.4$, $mass<0.01$', markersize=4)
plt.plot(.936, .987, 'o', color=color_RPConvRejClass,
         label='     $\phi_V>2.9$, $mass<0.0035$', markersize=3)



plt.xlim([0.0, 1.05])
plt.ylim([0.0, 1.05])
plt.xlabel('False Positive Rate')
plt.ylabel('True Positive Rate')
plt.title('Receiver operating characteristics')
plt.legend(loc=4, fontsize=7)
plt.savefig('temp_output/ROCcurves_entireMassRange.png')
plt.savefig('temp_output/ROCcurves_entireMassRange.pdf')
