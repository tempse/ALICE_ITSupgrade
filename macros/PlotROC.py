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
color_combined = '#edbd00'
color_combined_highlighted = '#a38200'

fig_width = 6.83
fig_height = 5.12
fig_dpi = 150

data_orig = pd.DataFrame(root_numpy.root2array(filename,
                                               branches=branches,
                                               start=0,
                                               stop=75000000))


fpr_CombConvRejMVA, tpr_CombConvRejMVA, thresholds_CombConvRejMVA = roc_curve(data_orig['IsConv'], data_orig['MVAoutput_CombConv'], pos_label=0)
roc_auc_CombConvRejMVA = auc(fpr_CombConvRejMVA, tpr_CombConvRejMVA)

fpr_singleConvTrackRejMVA, tpr_singleConvTrackRejMVA, thresholds_singleConvTrackRejMVA = roc_curve(data_orig['IsConv'], np.minimum.reduce([data_orig['MVAoutput_convTrack1'],data_orig['MVAoutput_convTrack2']]), pos_label=0)
roc_auc_singleConvTrackRejMVA = auc(fpr_singleConvTrackRejMVA, tpr_singleConvTrackRejMVA)


plt.figure(figsize=(fig_width,fig_height), dpi=fig_dpi)
plt.plot(fpr_CombConvRejMVA, tpr_CombConvRejMVA,
         label='Comb. conv. rej. via MVA cuts (area = %.3f)' % roc_auc_CombConvRejMVA,
         color=color_CombConvRej)

plt.plot(.7435, .9865,
         color=color_CombConvRej,
         marker='o',
         markersize=8,
         fillstyle='none',
         mew=2,
         linestyle='none')
plt.plot(.7218, .9733,
         color=color_CombConvRej,
         marker='^',
         markersize=8,
         fillstyle='none',
         mew=2,
         linestyle='none')
plt.plot(.7530, .9935,
         color=color_CombConvRej,
         marker='v',
         markersize=8,
         fillstyle='none',
         mew=2,
         linestyle='none')
plt.plot(.3465, .6178,
         color=color_CombConvRej,
         marker='x',
         markersize=8,
         fillstyle='none',
         mew=2,
         linestyle='none')

plt.plot(fpr_singleConvTrackRejMVA, tpr_singleConvTrackRejMVA,
         label='Single conv. track rej. via MVA cuts (area = %.3f)' % roc_auc_singleConvTrackRejMVA,
         color=color_singleConvTrackRej)

# dummy points
plt.plot(-99,-99,
         label='RP conv. rej. via MVA cuts + prefiltering',
         color=color_RPConvRejMVA,
         marker=r'$\bigotimes$',
         markersize=11,
         linestyle='none')
plt.plot(-99, -99,
         label='TMVA-optimized cuts',
         color='#333333',
         marker='o',
         markersize=8,
         fillstyle='none',
         mew=2,
         linestyle='none')
plt.plot(-99, -99,
         label='cut var.: +10%',
         color='#333333',
         marker='^',
         markersize=8,
         fillstyle='none',
         mew=2,
         linestyle='none')
plt.plot(-99, -99,
         label='cut var.: -10%',
         color='#333333',
         marker='v',
         markersize=8,
         fillstyle='none',
         mew=2,
         linestyle='none')
plt.plot(-99, -99,
         label='manual cut (for signal eff. at ~0.6)',
         color='#333333',
         marker='x',
         markersize=8,
         fillstyle='none',
         mew=2,
         linestyle='none')


plt.plot(.712, .903,
         #label='RP conv. rej. via MVA cuts + prefiltering: TMVA-optimized cut',
         color=color_RPConvRejMVA,
         marker='o',
         markersize=8,
         fillstyle='none',
         mew=2,
         linestyle='none')
plt.plot(.663, .845,
         #label='     cut var.: +10%',
         color=color_RPConvRejMVA,
         marker='^',
         markersize=8,
         fillstyle='none',
         mew=2,
         linestyle='none')
plt.plot(.799, .956,
         #label='     cut var.: -10%',
         color=color_RPConvRejMVA,
         marker='v',
         markersize=8,
         fillstyle='none',
         mew=2,
         linestyle='none')
plt.plot(.531, .599,
         #label='     manual cut (for signal eff. at ~0.6)',
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

plt.plot(.4527, .8331,
         label='Classifier combinations',
         color=color_combined,
         marker='o',
         markersize=2,
         fillstyle='full',
         mew=2,
         linestyle='none')
plt.plot(.5678, .9431,
         #label='Classifier combinations',
         color=color_combined,
         marker='o',
         markersize=2,
         fillstyle='full',
         mew=2,
         linestyle='none')
plt.plot(.3544, .5896,
         #label='Classifier combinations',
         color=color_combined,
         marker='o',
         markersize=2,
         fillstyle='full',
         mew=2,
         linestyle='none')
plt.plot(.4779, .8779,
         #label='Classifier combinations',
         color=color_combined,
         marker='o',
         markersize=2,
         fillstyle='full',
         mew=2,
         linestyle='none')
plt.plot(.4389, .8217,
         #label='Classifier combinations',
         color=color_combined,
         marker='o',
         markersize=2,
         fillstyle='full',
         mew=2,
         linestyle='none')
plt.plot(.5508, .9304,
         #label='Classifier combinations',
         color=color_combined,
         marker='o',
         markersize=2,
         fillstyle='full',
         mew=2,
         linestyle='none')
plt.plot(.3430, .5812,
         #label='Classifier combinations',
         color=color_combined,
         marker='o',
         markersize=2,
         fillstyle='full',
         mew=2,
         linestyle='none')
plt.plot(.4999, .8965,
         #label='Classifier combinations',
         color=color_combined,
         marker='o',
         markersize=2,
         fillstyle='full',
         mew=2,
         linestyle='none')
plt.plot(.4595, .8394,
         #label='Classifier combinations',
         color=color_combined,
         marker='o',
         markersize=2,
         fillstyle='full',
         mew=2,
         linestyle='none')
plt.plot(.5754, .9499,
         #label='Classifier combinations',
         color=color_combined,
         marker='o',
         markersize=2,
         fillstyle='full',
         mew=2,
         linestyle='none')
plt.plot(.3598, .5946,
         #label='Classifier combinations',
         color=color_combined,
         marker='o',
         markersize=2,
         fillstyle='full',
         mew=2,
         linestyle='none')
plt.plot(.2235, .5564,
         #label='Classifier combinations',
         color=color_combined,
         marker='o',
         markersize=2,
         fillstyle='full',
         mew=2,
         linestyle='none')
plt.plot(.2031, .5190,
         #label='Classifier combinations',
         color=color_combined,
         marker='o',
         markersize=2,
         fillstyle='full',
         mew=2,
         linestyle='none')
plt.plot(.2626, .5906,
         #label='Classifier combinations',
         color=color_combined,
         marker='o',
         markersize=2,
         fillstyle='full',
         mew=2,
         linestyle='none')
plt.plot(.1596, .3635,
         #label='Classifier combinations',
         color=color_combined,
         marker='o',
         markersize=2,
         fillstyle='full',
         mew=2,
         linestyle='none')
plt.plot(.4930, .8900,
         label='     individually optimized cuts',
         color=color_combined_highlighted,
         alpha=.75,
         marker='o',
         markersize=5,
         fillstyle='none',
         mew=1,
         linestyle='none')

plt.plot([0,1], [0,1], 'k--')
plt.xlim([0.0, 1.05])
plt.ylim([0.0, 1.05])
plt.xlabel('False Positive Rate')
plt.ylabel('True Positive Rate')
plt.title('Receiver operating characteristics (entire mass range)')
plt.legend(loc=4, fontsize=6, markerscale=.75)
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

plt.figure(figsize=(fig_width,fig_height), dpi=fig_dpi)
plt.plot(fpr_CombConvRejMVA_masscut, tpr_CombConvRejMVA_masscut,
        label='Comb. conv. rej. via MVA cuts (area = %.3f)' % roc_auc_CombConvRejMVA_masscut,
         color=color_CombConvRej)


plt.plot(.7362, .9859,
         color=color_CombConvRej,
         marker='o',
         markersize=8,
         fillstyle='none',
         mew=2,
         linestyle='none')
plt.plot(.7138, .9721,
         color=color_CombConvRej,
         marker='^',
         markersize=8,
         fillstyle='none',
         mew=2,
         linestyle='none')
plt.plot(.7459, .9932,
         color=color_CombConvRej,
         marker='v',
         markersize=8,
         fillstyle='none',
         mew=2,
         linestyle='none')
plt.plot(.3277, .6002,
         color=color_CombConvRej,
         marker='x',
         markersize=8,
         fillstyle='none',
         mew=2,
         linestyle='none')

plt.plot(fpr_singleConvTrackRejMVA_masscut, tpr_singleConvTrackRejMVA_masscut,
        label='Single conv. track rej. via MVA cuts (area = %.3f)' % roc_auc_singleConvTrackRejMVA_masscut,
         color=color_singleConvTrackRej)

# dummy points
plt.plot(-99,-99,
         label='RP conv. rej. via MVA cuts + prefiltering',
         color=color_RPConvRejMVA,
         marker=r'$\bigotimes$',
         markersize=11,
         linestyle='none')
plt.plot(-99, -99,
         label='TMVA-optimized cuts',
         color='#333333',
         marker='o',
         markersize=8,
         fillstyle='none',
         mew=2,
         linestyle='none')
plt.plot(-99, -99,
         label='cut var.: +10%',
         color='#333333',
         marker='^',
         markersize=8,
         fillstyle='none',
         mew=2,
         linestyle='none')
plt.plot(-99, -99,
         label='cut var.: -10%',
         color='#333333',
         marker='v',
         markersize=8,
         fillstyle='none',
         mew=2,
         linestyle='none')
plt.plot(-99, -99,
         label='manual cut (for signal eff. at ~0.6)',
         color='#333333',
         marker='x',
         markersize=8,
         fillstyle='none',
         mew=2,
         linestyle='none')


plt.plot(.717, .905,
         #label='RP conv. rej. via MVA cuts + prefiltering: TMVA-optimized cut',
         color=color_RPConvRejMVA,
         marker='o',
         markersize=8,
         fillstyle='none',
         mew=2,
         linestyle='none')
plt.plot(.669, .849,
         #label='     cut var.: +10%',
         color=color_RPConvRejMVA,
         marker='^',
         markersize=8,
         fillstyle='none',
         mew=2,
         linestyle='none')
plt.plot(.803, .957,
         #label='     cut var.: -10%',
         color=color_RPConvRejMVA,
         marker='v',
         markersize=8,
         fillstyle='none',
         mew=2,
         linestyle='none')
plt.plot(.537, .609,
         #label='     manual cut (for signal eff. at ~0.6)',
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

plt.plot(.4522, .8362,
         label='Classifier combinations',
         color=color_combined,
         marker='o',
         markersize=2,
         fillstyle='full',
         mew=2,
         linestyle='none')
plt.plot(.5646, .9435,
         #label='Classifier combinations',
         color=color_combined,
         marker='o',
         markersize=2,
         fillstyle='full',
         mew=2,
         linestyle='none')
plt.plot(.3555, .5993,
         #label='Classifier combinations',
         color=color_combined,
         marker='o',
         markersize=2,
         fillstyle='full',
         mew=2,
         linestyle='none')
plt.plot(.4759, .8790,
         #label='Classifier combinations',
         color=color_combined,
         marker='o',
         markersize=2,
         fillstyle='full',
         mew=2,
         linestyle='none')
plt.plot(.4377, .8243,
         #label='Classifier combinations',
         color=color_combined,
         marker='o',
         markersize=2,
         fillstyle='full',
         mew=2,
         linestyle='none')
plt.plot(.5471, .9302,
         #label='Classifier combinations',
         color=color_combined,
         marker='o',
         markersize=2,
         fillstyle='full',
         mew=2,
         linestyle='none')
plt.plot(.3437, .5905,
         #label='Classifier combinations',
         color=color_combined,
         marker='o',
         markersize=2,
         fillstyle='full',
         mew=2,
         linestyle='none')
plt.plot(.4986, .8985,
         #label='Classifier combinations',
         color=color_combined,
         marker='o',
         markersize=2,
         fillstyle='full',
         mew=2,
         linestyle='none')
plt.plot(.4589, .8428,
         #label='Classifier combinations',
         color=color_combined,
         marker='o',
         markersize=2,
         fillstyle='full',
         mew=2,
         linestyle='none')
plt.plot(.5725, .9506,
         #label='Classifier combinations',
         color=color_combined,
         marker='o',
         markersize=2,
         fillstyle='full',
         mew=2,
         linestyle='none')
plt.plot(.3610, .6045,
         #label='Classifier combinations',
         color=color_combined,
         marker='o',
         markersize=2,
         fillstyle='full',
         mew=2,
         linestyle='none')
plt.plot(.2142, .5427,
         #label='Classifier combinations',
         color=color_combined,
         marker='o',
         markersize=2,
         fillstyle='full',
         mew=2,
         linestyle='none')
plt.plot(.1952, .5076,
         #label='Classifier combinations',
         color=color_combined,
         marker='o',
         markersize=2,
         fillstyle='full',
         mew=2,
         linestyle='none')
plt.plot(.2507, .5747,
         #label='Classifier combinations',
         color=color_combined,
         marker='o',
         markersize=2,
         fillstyle='full',
         mew=2,
         linestyle='none')
plt.plot(.1551, .3627,
         #label='Classifier combinations',
         color=color_combined,
         marker='o',
         markersize=2,
         fillstyle='full',
         mew=2,
         linestyle='none')
plt.plot(.4915, .8917,
         label='     individually optimized cuts',
         color=color_combined_highlighted,
         alpha=.75,
         marker='o',
         markersize=5,
         fillstyle='none',
         mew=1,
         linestyle='none')

plt.plot([0,1], [0,1], 'k--')
plt.xlim([0.0, 1.05])
plt.ylim([0.0, 1.05])
plt.xlabel('False Positive Rate')
plt.ylabel('True Positive Rate')
plt.title('Receiver operating characteristics (mass > 50 MeV/c$^2$)')
plt.legend(loc=4, fontsize=6, markerscale=.75)
plt.savefig('temp_output/ROCcurves_masscut.png')
plt.savefig('temp_output/ROCcurves_masscut.pdf')
