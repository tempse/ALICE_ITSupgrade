import matplotlib.pyplot as plt
import seaborn as sns; sns.set()

import numpy as np
import pandas as pd
import root_numpy

from sklearn.metrics import roc_curve, auc

filename = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/analysis_singleConvTrackRejMVAcuts/applicationPhase1/FT2_AnalysisResults_Upgrade_addFeat_pairtree_us_part2_1-9-split.root"

branches = ['MVAoutput_CombConv',
            'MVAoutput_RPConv',
            'MVAoutput_convTrack1',
            'MVAoutput_convTrack2',
            'IsConv',
            'mass']

color_RPConvRejClass = '#666666'
color_RPConvRejMVA = '#dd0000'
color_singleConvTrackRej = '#4444ff'
color_CombConvRej = 'green'

data_orig = pd.DataFrame(root_numpy.root2array(filename,
                                               branches=branches,
                                               start=0,
                                               stop=75000000))


fpr_CombConvRejMVA, tpr_CombConvRejMVA, thresholds_CombConvRejMVA = roc_curve(data_orig['IsConv'], data_orig['MVAoutput_CombConv'], pos_label=0)
roc_auc_CombConvRejMVA = auc(fpr_CombConvRejMVA, tpr_CombConvRejMVA)

fpr_singleConvTrackRejMVA, tpr_singleConvTrackRejMVA, thresholds_singleConvTrackRejMVA = roc_curve(data_orig['IsConv'], np.minimum.reduce([data_orig['MVAoutput_convTrack1'],data_orig['MVAoutput_convTrack2']]), pos_label=0)
roc_auc_singleConvTrackRejMVA = auc(fpr_singleConvTrackRejMVA, tpr_singleConvTrackRejMVA)


plt.figure()
plt.plot(fpr_CombConvRejMVA, tpr_CombConvRejMVA,
        label='Comb. conv. rej. via MVA cuts (area = %.3f)' % roc_auc_CombConvRejMVA,
        color=color_CombConvRej)
plt.plot(fpr_singleConvTrackRejMVA, tpr_singleConvTrackRejMVA,
        label='Single conv. track rej. via MVA cuts (area = %.3f)' % roc_auc_singleConvTrackRejMVA,
        color=color_singleConvTrackRej)
plt.plot(.712, .903,
         label='RP conv. rej. via MVA cuts + prefiltering: TMVA-optimized cut',
         color=color_RPConvRejMVA,
         marker='o',
         markersize=8,
         fillstyle='none',
         mew=2,
         linestyle='none')
plt.plot(.663, .845,
         label='     cut var.: +10%',
         color=color_RPConvRejMVA,
         marker='^',
         markersize=8,
         fillstyle='none',
         mew=2,
         linestyle='none')
plt.plot(.799, .956,
         label='     cut var.: -10%',
         color=color_RPConvRejMVA,
         marker='v',
         markersize=8,
         fillstyle='none',
         mew=2,
         linestyle='none')
plt.plot(.531, .599,
         label='     manual cut',
         color=color_RPConvRejMVA,
         marker='x',
         markersize=8,
         fillstyle='none',
         mew=2,
         linestyle='none')

plt.plot(.447, .532, 'o', color=color_RPConvRejClass,
         label='RP conv. rej. via classical cuts: $\phi_V>\pi/2$, $mass<0.05$', markersize=8)
plt.plot(.613, .759, 'o', color=color_RPConvRejClass,
         label='     $\phi_V>2$, $mass<0.04$', markersize=6)
plt.plot(.737, .916, 'o', color=color_RPConvRejClass,
         label='     $\phi_V>2.4$, $mass<0.01$', markersize=4)
plt.plot(.936, .987, 'o', color=color_RPConvRejClass,
         label='     $\phi_V>2.9$, $mass<0.0035$', markersize=3)

plt.plot([0,1], [0,1], 'k--')
plt.xlim([0.0, 1.05])
plt.ylim([0.0, 1.05])
plt.xlabel('False Positive Rate')
plt.ylabel('True Positive Rate')
plt.title('Receiver operating characteristic curves (entire mass range)')
plt.legend(loc=4, fontsize='xx-small', markerscale=.75)
plt.savefig('temp_output/ROCcurves_entireMassRange.png')
plt.savefig('temp_output/ROCcurves_entireMassRange.pdf')



# do the same as above for mass>0.05

IsConv_masscut = data_orig['IsConv'][np.array(data_orig['mass']>.05)]
MVAoutput_CombConv_masscut = data_orig['MVAoutput_CombConv'][np.array(data_orig['mass']>.05)]
MVAoutput_RPConv_masscut = data_orig['MVAoutput_RPConv'][np.array(data_orig['mass']>.05)]
MVAoutput_convTrack1_masscut = data_orig['MVAoutput_convTrack1'][np.array(data_orig['mass']>.05)]
MVAoutput_convTrack2_masscut = data_orig['MVAoutput_convTrack2'][np.array(data_orig['mass']>.05)]


fpr_CombConvRejMVA_masscut, tpr_CombConvRejMVA_masscut, thresholds_CombConvRejMVA_masscut = roc_curve(IsConv_masscut, MVAoutput_CombConv_masscut, pos_label=0)
roc_auc_CombConvRejMVA_masscut = auc(fpr_CombConvRejMVA_masscut, tpr_CombConvRejMVA_masscut)

fpr_singleConvTrackRejMVA_masscut, tpr_singleConvTrackRejMVA_masscut, thresholds_singleConvTrackRejMVA_masscut = roc_curve(IsConv_masscut, np.minimum.reduce([MVAoutput_convTrack1_masscut,MVAoutput_convTrack2_masscut]), pos_label=0)
roc_auc_singleConvTrackRejMVA_masscut = auc(fpr_singleConvTrackRejMVA_masscut, tpr_singleConvTrackRejMVA_masscut)

plt.figure()
plt.plot(fpr_CombConvRejMVA_masscut, tpr_CombConvRejMVA_masscut,
        label='Comb. conv. rej. via MVA cuts (area = %.3f)' % roc_auc_CombConvRejMVA_masscut,
        color=color_CombConvRej)
plt.plot(fpr_singleConvTrackRejMVA_masscut, tpr_singleConvTrackRejMVA_masscut,
        label='Single conv. track rej. via MVA cuts (area = %.3f)' % roc_auc_singleConvTrackRejMVA_masscut,
        color=color_singleConvTrackRej)
plt.plot(.717, .905,
         label='RP conv. rej. via MVA cuts + prefiltering: TMVA-optimized cut',
         color=color_RPConvRejMVA,
         marker='o',
         markersize=8,
         fillstyle='none',
         mew=2,
         linestyle='none')
plt.plot(.669, .849,
         label='     cut var.: +10%',
         color=color_RPConvRejMVA,
         marker='^',
         markersize=8,
         fillstyle='none',
         mew=2,
         linestyle='none')
plt.plot(.803, .957,
         label='     cut var.: -10%',
         color=color_RPConvRejMVA,
         marker='v',
         markersize=8,
         fillstyle='none',
         mew=2,
         linestyle='none')
plt.plot(.537, .609,
         label='     manual cut',
         color=color_RPConvRejMVA,
         marker='x',
         markersize=8,
         fillstyle='none',
         mew=2,
         linestyle='none')

plt.plot(.455, .542, 'o', color=color_RPConvRejClass,
         label='RP conv. rej. via classical cuts: $\phi_V>\pi/2$, $mass<0.05$', markersize=8)
plt.plot(.619, .765, 'o', color=color_RPConvRejClass,
         label='     $\phi_V>2$, $mass<0.04$', markersize=6)
plt.plot(.741, .918, 'o', color=color_RPConvRejClass,
         label='     $\phi_V>2.4$, $mass<0.01$', markersize=4)
plt.plot(.938, .987, 'o', color=color_RPConvRejClass,
         label='     $\phi_V>2.9$, $mass<0.0035$', markersize=3)

plt.plot([0,1], [0,1], 'k--')
plt.xlim([0.0, 1.05])
plt.ylim([0.0, 1.05])
plt.xlabel('False Positive Rate')
plt.ylabel('True Positive Rate')
plt.title('Receiver operating characteristic curves (mass > 50 MeV/c$^2$)')
plt.legend(loc=4, fontsize='xx-small', markerscale=.75)
plt.savefig('temp_output/ROCcurves_masscut.png')
plt.savefig('temp_output/ROCcurves_masscut.pdf')
