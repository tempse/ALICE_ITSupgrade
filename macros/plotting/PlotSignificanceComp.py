import matplotlib.pyplot as plt
import seaborn as sns; sns.set()

import numpy as np
from numpy import trapz
import pandas as pd
import root_numpy


filename = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/properPrefiltering/significanceData/significanceData_100steps_tightCuts/temp_output/significance_data.root"
filename_wLooseCuts = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/properPrefiltering/significanceData/significanceData_100steps_tightCuts_looseCuts/temp_output/significance_data.root"
filename_wLooseCuts_wAllTracks = "/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_DNN/properPrefiltering/significanceData/significanceData_100steps_tightCuts_looseCuts_allTracks/temp_output/significance_data.root"

color_RPConvRejMVA = '#4444ff'
color_RPConvRejMVA_wLooseCuts = 'black'
color_RPConvRejMVA_wLooseCuts_wAllTracks = 'black'
color_singleConvTrackRej = '#dd0000'
color_CombConvRej = 'green'
color_RPConvRejClass = 'black'

fig_width = 6.83
fig_height = 5.12
fig_dpi = 150

# working point efficiencies:
workingpoint_eff_RPConvRejMVA = .8662
workingpoint_eff_RPConvRejMVA_wLooseTracks = .8462
workingpoint_eff_RPConvRejMVA_wLooseTracks_wAllTracks = .8343
workingpoint_eff_singleConvTrackRej = .9859
workingpoint_eff_CombConvRej = .979


data = pd.DataFrame(root_numpy.root2array(filename))
data_wLooseCuts = pd.DataFrame(root_numpy.root2array(filename_wLooseCuts))
data_wLooseCuts_wAllTracks = pd.DataFrame(root_numpy.root2array(filename_wLooseCuts_wAllTracks))


plt.figure(figsize=(fig_width,fig_height), dpi=fig_dpi)

ax = plt.gca()

ax.plot(data['S_eff_CombConvRejMVA'], data['significance_CombConvRejMVA'],
        label='MVA 1', #'comb. conv. rej.',
        color=color_CombConvRej)
plt.plot(workingpoint_eff_CombConvRej, data.iloc[np.argmin(np.abs(data['S_eff_CombConvRejMVA']-workingpoint_eff_CombConvRej))]['significance_CombConvRejMVA'],
         color=color_CombConvRej,
         marker='o',
         markersize=7,
         fillstyle='none',
         mew=1,
         linestyle='none')

ax.plot(data['S_eff_singleConvTrackRejMVA'], data['significance_singleConvTrackRejMVA'],
        label='MVA 2', #'single-track conv. rej.',
        color=color_singleConvTrackRej)
plt.plot(workingpoint_eff_singleConvTrackRej, data.iloc[np.argmin(np.abs(data['S_eff_singleConvTrackRejMVA']-workingpoint_eff_singleConvTrackRej))]['significance_singleConvTrackRejMVA'],
         color=color_singleConvTrackRej,
         marker='o',
         markersize=7,
         fillstyle='none',
         mew=1,
         linestyle='none')

ax.plot(data['S_eff_RPConvRejMVA'], data['significance_RPConvRejMVA'],
        label='MVA 3', #'RP conv. rej. + prefiltering',
        color=color_RPConvRejMVA)
plt.plot(workingpoint_eff_RPConvRejMVA, data.iloc[np.argmin(np.abs(data['S_eff_RPConvRejMVA']-workingpoint_eff_RPConvRejMVA))]['significance_RPConvRejMVA'],
         color=color_RPConvRejMVA,
         marker='o',
         markersize=7,
         fillstyle='none',
         mew=1,
         linestyle='none')

ax.plot(data_wLooseCuts['S_eff_RPConvRejMVA'], data_wLooseCuts['significance_RPConvRejMVA'],
        label='MVA 3 with loose-cuts tracks',
        color=color_RPConvRejMVA_wLooseCuts,
        alpha=.3)
plt.plot(workingpoint_eff_RPConvRejMVA_wLooseTracks, data_wLooseCuts.iloc[np.argmin(np.abs(data_wLooseCuts['S_eff_RPConvRejMVA']-workingpoint_eff_RPConvRejMVA_wLooseTracks))]['significance_RPConvRejMVA'],
         color=color_RPConvRejMVA_wLooseCuts,
         marker='o',
         markersize=7,
         fillstyle='none',
         mew=1,
         linestyle='none',
         alpha=.45)

ax.plot(data_wLooseCuts_wAllTracks['S_eff_RPConvRejMVA'], data_wLooseCuts_wAllTracks['significance_RPConvRejMVA'],
        label='MVA 3 with loose-cuts tracks and all other tracks',
        color=color_RPConvRejMVA_wLooseCuts_wAllTracks,
        alpha=.55,
        linestyle=':')
plt.plot(workingpoint_eff_RPConvRejMVA_wLooseTracks_wAllTracks, data_wLooseCuts_wAllTracks.iloc[np.argmin(np.abs(data_wLooseCuts_wAllTracks['S_eff_RPConvRejMVA']-workingpoint_eff_RPConvRejMVA_wLooseTracks_wAllTracks))]['significance_RPConvRejMVA'],
         color=color_RPConvRejMVA_wLooseCuts_wAllTracks,
         marker='o',
         markersize=7,
         fillstyle='none',
         mew=1,
         linestyle='none',
         alpha=.8)

# plot dummy point
ax.plot(-99,0,
        label='optimized working points',
        color='black',
        marker='o',
        markersize=7,
        fillstyle='none',
        mew=1,
        linestyle='none')

# classical-cut points
ax.plot(.1944, 180.13, #.5963, 214.9077,
        label=r'RP conv. rej. (class. cuts) + prefiltering $\varphi_V>\pi/2$, $m_{ee}<0.05$',
        color=color_RPConvRejClass,
        marker='o',
        markersize=8,
        linestyle='none',
        alpha=.8)
ax.plot(.3438, 233.30, #.7751, 246.6994,
        label=r'     $\varphi_V>2$, $m_{ee}<0.04$',
        color=color_RPConvRejClass,
        marker='o',
        markersize=6,
        linestyle='none',
        alpha=.8)
ax.plot(.7733, 322.44, #.9115, 267.7348,
        label=r'     $\varphi_V>2.4$, $m_{ee}<0.01$',
        color=color_RPConvRejClass,
        marker='o',
        markersize=4,
        linestyle='none',
        alpha=.8)
ax.plot(.9623, 320.59, #.9860, 263.7709,
        label=r'     $\varphi_V>2.9$, $m_{ee}<0.0035$',
        color=color_RPConvRejClass,
        marker='o',
        markersize=3,
        linestyle='none',
        alpha=.8)


plt.legend(loc=4, fontsize=7)

plt.xlim([-.05,1.05])
plt.xlabel('Signal efficiency', fontsize=18)
plt.ylabel('Significance', fontsize=18)
plt.savefig('temp_output/significances_effS.png')
plt.savefig('temp_output/significances_effS.pdf')



plt.figure(figsize=(fig_width,fig_height), dpi=fig_dpi)

ax = plt.gca()


ax.plot(data['MVAcut'], data['significance_CombConvRejMVA'],
        label='MVA 1', #'comb. conv. rej.',
        color=color_CombConvRej)

ax.plot(data['MVAcut'], data['significance_singleConvTrackRejMVA'],
        label='MVA 2', #'single-track conv. rej.',
        color=color_singleConvTrackRej)

ax.plot(data['MVAcut'], data['significance_RPConvRejMVA'],
        label='MVA 3', #'RP conv. rej. + prefiltering',
        color=color_RPConvRejMVA)

ax.plot(data_wLooseCuts['MVAcut'], data_wLooseCuts['significance_RPConvRejMVA'],
        label='MVA 3 with loose-cuts tracks',
        color=color_RPConvRejMVA_wLooseCuts,
        alpha=.3)

ax.plot(data_wLooseCuts_wAllTracks['MVAcut'], data_wLooseCuts_wAllTracks['significance_RPConvRejMVA'],
        label='MVA 3 with loose-cuts tracks and all other tracks',
        color=color_RPConvRejMVA_wLooseCuts_wAllTracks,
        linestyle=':',
        alpha=.55)

plt.legend(fontsize=7, loc=3)

plt.xlabel('MVA cut', fontsize=18)
plt.ylabel('Significance', fontsize=18)
plt.savefig('temp_output/significances_MVAcut.png')
plt.savefig('temp_output/significances_MVAcut.pdf')


plt.figure(figsize=(fig_width,fig_height), dpi=fig_dpi)

ax = plt.gca()

ax.plot(data['MVAcut'], data['B_eff_RPConvRejMVA'],
        color=color_RPConvRejMVA,
        linestyle='--',
        alpha=.8)

ax.plot(data_wLooseCuts['MVAcut'], data_wLooseCuts['B_eff_RPConvRejMVA'],
        color=color_RPConvRejMVA_wLooseCuts,
        linestyle='--',
        alpha=.2)

ax.plot(data_wLooseCuts_wAllTracks['MVAcut'], data_wLooseCuts_wAllTracks['B_eff_RPConvRejMVA'],
        color=color_RPConvRejMVA_wLooseCuts_wAllTracks,
        linestyle='--',
        alpha=.45)

ax.plot(data['MVAcut'], data['B_eff_CombConvRejMVA'],
        color=color_CombConvRej,
        linestyle='--',
        alpha=.8)

ax.plot(data['MVAcut'], data['B_eff_singleConvTrackRejMVA'],
        color=color_singleConvTrackRej,
        linestyle='--',
        alpha=.8)

ax.plot(data['MVAcut'], data['S_eff_CombConvRejMVA'],
        label='MVA 1', #'comb. conv. rej.',
        color=color_CombConvRej)

ax.plot(data['MVAcut'], data['S_eff_singleConvTrackRejMVA'],
        label='MVA 2', #'single-track conv. rej.',
        color=color_singleConvTrackRej)

ax.plot(data['MVAcut'], data['S_eff_RPConvRejMVA'],
        label='MVA 3', #'RP conv. rej. + prefiltering',
        color=color_RPConvRejMVA)

ax.plot(data_wLooseCuts['MVAcut'], data_wLooseCuts['S_eff_RPConvRejMVA'],
        label='MVA 3 with loose-cuts tracks',
        color=color_RPConvRejMVA_wLooseCuts,
        alpha=.3)

ax.plot(data_wLooseCuts_wAllTracks['MVAcut'], data_wLooseCuts_wAllTracks['S_eff_RPConvRejMVA'],
        label='MVA 3 with loose-cuts tracks and all other tracks',
        color=color_RPConvRejMVA_wLooseCuts_wAllTracks,
        linestyle=':',
        alpha=.55)


ax.plot(-99,-99, #dummy point
        label='signal efficiency',
        color='black',
        #linestlye='-',
        alpha=.75)
ax.plot(-99,-99, #dummy point
        label='background efficiency',
        color='black',
        linestyle='--',
        alpha=.75)

handles_all, labels_all = ax.get_legend_handles_labels()
o = [6,7,3,4,5]
handles = [handles_all[O] for O in o]
labels = [labels_all[O] for O in o]

plt.legend(handles, labels, fontsize=7)

plt.xlabel('MVA cut', fontsize=18)
plt.ylabel('Signal (background) efficiency', fontsize=18)
plt.xlim([-.05, 1.05])
plt.ylim([-.05, 1.05])
plt.savefig('temp_output/effS_MVAcut.png')
plt.savefig('temp_output/effS_MVAcut.pdf')



plt.figure(figsize=(fig_width,fig_height), dpi=fig_dpi)

ax = plt.gca()


ax.plot(data['MVAcut'], data['significance_gain_CombConvRejMVA'],
        label='MVA 1', #'comb. conv. rej.',
        color=color_CombConvRej)

ax.plot(data['MVAcut'], data['significance_gain_singleConvTrackRejMVA'],
        label='MVA 2', #'single-track conv. rej.',
        color=color_singleConvTrackRej)

ax.plot(data['MVAcut'], data['significance_gain_RPConvRejMVA'],
        label='MVA 3', #'RP conv. rej. + prefiltering',
        color=color_RPConvRejMVA)

ax.plot(data_wLooseCuts['MVAcut'], data_wLooseCuts['significance_gain_RPConvRejMVA'],
        label='MVA 3 with loose-cuts tracks',
        color=color_RPConvRejMVA_wLooseCuts,
        alpha=.3)

ax.plot(data_wLooseCuts_wAllTracks['MVAcut'], data_wLooseCuts_wAllTracks['significance_gain_RPConvRejMVA'],
        label='MVA 3 with loose-cuts tracks and all other tracks',
        color=color_RPConvRejMVA_wLooseCuts_wAllTracks,
        linestyle=':',
        alpha=.55)

plt.legend(fontsize=7, loc=3)
plt.xlabel('MVA cut', fontsize=18)
plt.ylabel('Significance gain', fontsize=18)
plt.savefig('temp_output/significancegain_MVAcut.png')
plt.savefig('temp_output/significancegain_MVAcut.pdf')



plt.figure(figsize=(fig_width,fig_height), dpi=fig_dpi)

ax = plt.gca()

ax.plot(data['S_eff_CombConvRejMVA'], data['significance_gain_CombConvRejMVA'],
        label='MVA 1', #'comb. conv. rej.',
        color=color_CombConvRej)
plt.plot(workingpoint_eff_CombConvRej, data.iloc[np.argmin(np.abs(data['S_eff_CombConvRejMVA']-workingpoint_eff_CombConvRej))]['significance_gain_CombConvRejMVA'],
         color=color_CombConvRej,
         marker='o',
         markersize=7,
         fillstyle='none',
         mew=1,
         linestyle='none')

ax.plot(data['S_eff_singleConvTrackRejMVA'], data['significance_gain_singleConvTrackRejMVA'],
        label='MVA 2', #'single-track conv. rej.',
        color=color_singleConvTrackRej)
plt.plot(workingpoint_eff_singleConvTrackRej, data.iloc[np.argmin(np.abs(data['S_eff_singleConvTrackRejMVA']-workingpoint_eff_singleConvTrackRej))]['significance_gain_singleConvTrackRejMVA'],
         color=color_singleConvTrackRej,
         marker='o',
         markersize=7,
         fillstyle='none',
         mew=1,
         linestyle='none')

ax.plot(data['S_eff_RPConvRejMVA'], data['significance_gain_RPConvRejMVA'],
        label='MVA 3', #'RP conv. rej. + prefiltering',
        color=color_RPConvRejMVA)
plt.plot(workingpoint_eff_RPConvRejMVA, data.iloc[np.argmin(np.abs(data['S_eff_RPConvRejMVA']-workingpoint_eff_RPConvRejMVA))]['significance_gain_RPConvRejMVA'],
         color=color_RPConvRejMVA,
         marker='o',
         markersize=7,
         fillstyle='none',
         mew=1,
         linestyle='none')

ax.plot(data_wLooseCuts['S_eff_RPConvRejMVA'], data_wLooseCuts['significance_gain_RPConvRejMVA'],
        label='MVA 3 with loose-cuts tracks',
        color=color_RPConvRejMVA_wLooseCuts,
        alpha=.3)
plt.plot(workingpoint_eff_RPConvRejMVA_wLooseTracks, data_wLooseCuts.iloc[np.argmin(np.abs(data_wLooseCuts['S_eff_RPConvRejMVA']-workingpoint_eff_RPConvRejMVA_wLooseTracks))]['significance_gain_RPConvRejMVA'],
         color=color_RPConvRejMVA_wLooseCuts,
         marker='o',
         markersize=7,
         fillstyle='none',
         mew=1,
         linestyle='none',
         alpha=.45)

ax.plot(data_wLooseCuts_wAllTracks['S_eff_RPConvRejMVA'], data_wLooseCuts_wAllTracks['significance_gain_RPConvRejMVA'],
        label='MVA 3 with loose-cuts tracks and all other tracks',
        color=color_RPConvRejMVA_wLooseCuts_wAllTracks,
        linestyle=':',
        alpha=.55)
plt.plot(workingpoint_eff_RPConvRejMVA_wLooseTracks_wAllTracks, data_wLooseCuts_wAllTracks.iloc[np.argmin(np.abs(data_wLooseCuts_wAllTracks['S_eff_RPConvRejMVA']-workingpoint_eff_RPConvRejMVA_wLooseTracks_wAllTracks))]['significance_gain_RPConvRejMVA'],
         color=color_RPConvRejMVA_wLooseCuts_wAllTracks,
         marker='o',
         markersize=7,
         fillstyle='none',
         mew=1,
         linestyle='none',
         alpha=.8)

# plot dummy point
ax.plot(-99,0,
        label='optimized working point',
        color='black',
        marker='o',
        markersize=7,
        fillstyle='none',
        mew=1,
        linestyle='none')

# classical-cut points (calculated from a sample with exactly 16.4M pairs)
ax.plot(.1943, -.5479, #.5968, -.2321,
        label=r'RP conv. rej. (class. cuts) + prefiltering $\varphi_V>\pi/2$, $m_{ee}<0.05$',
        color=color_RPConvRejClass,
        marker='o',
        markersize=8,
        linestyle='none',
        alpha=.8)
ax.plot(.3438, -.3308, #.7751, -.0659,
        label=r'     $\varphi_V>2$, $m_{ee}<0.04$',
        color=color_RPConvRejClass,
        marker='o',
        markersize=6,
        linestyle='none',
        alpha=.8)
ax.plot(.7733, .0331, #.9115, .0400,
        label=r'     $\varphi_V>2.4$, $m_{ee}<0.01$',
        color=color_RPConvRejClass,
        marker='o',
        markersize=4,
        linestyle='none',
        alpha=.8)
ax.plot(.9623, .0256, #.9860, .0137,
        label=r'     $\varphi_V>2.9$, $m_{ee}<0.0035$',
        color=color_RPConvRejClass,
        marker='o',
        markersize=3,
        linestyle='none',
        alpha=.8)

plt.legend(fontsize=7)
plt.xlim([-0.05,1.05])
plt.xlabel('Signal efficiency', fontsize=18)
plt.ylabel('Significance gain', fontsize=18)
plt.savefig('temp_output/significancegain_effS.png')
plt.savefig('temp_output/significancegain_effS.pdf')
