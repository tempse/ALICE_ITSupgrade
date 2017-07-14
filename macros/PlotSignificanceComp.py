import matplotlib.pyplot as plt
import seaborn as sns; sns.set()

import numpy as np
from numpy import trapz
import pandas as pd
import root_numpy


filename = "~/analysis/data/FT2_AnalysisResults_Upgrade/fullAnalysis_ROOT_TMVA_BDT/plots/significanceComparisons/significanceData.root"

color_RPConvRejMVA = '#dd0000'
color_singleConvTrackRej = '#4444ff'
color_CombConvRej = 'green'
color_RPConvRejClass = 'black'

fig_width = 6.83
fig_height = 5.12
fig_dpi = 150

# working point efficiencies:
workingpoint_eff_RPConvRejMVA = .9265
workingpoint_eff_singleConvTrackRej = .9703
workingpoint_eff_CombConvRej = .9823


data = pd.DataFrame(root_numpy.root2array(filename))


plt.figure(figsize=(fig_width,fig_height), dpi=fig_dpi)

ax = plt.gca()

ax.plot(data['S_eff_RPConvRejMVA'], data['significance_RPConvRejMVA'],
        label='RP conv. rej. + prefiltering',
        color=color_RPConvRejMVA)
plt.plot(workingpoint_eff_RPConvRejMVA, data.iloc[np.argmin(np.abs(data['S_eff_RPConvRejMVA']-workingpoint_eff_RPConvRejMVA))]['significance_RPConvRejMVA'],
         color=color_RPConvRejMVA,
         marker='o',
         markersize=7,
         fillstyle='none',
         mew=1,
         linestyle='none')

ax.plot(data['S_eff_CombConvRejMVA'], data['significance_CombConvRejMVA'],
        label='comb. conv. rej.',
        color=color_CombConvRej)
plt.plot(workingpoint_eff_CombConvRej, data.iloc[np.argmin(np.abs(data['S_eff_CombConvRejMVA']-workingpoint_eff_CombConvRej))]['significance_CombConvRejMVA'],
         color=color_CombConvRej,
         marker='o',
         markersize=7,
         fillstyle='none',
         mew=1,
         linestyle='none')

ax.plot(data['S_eff_singleTrackConvRejMVA'], data['significance_singleTrackConvRejMVA'],
        label='single-track conv. rej.',
        color=color_singleConvTrackRej)
plt.plot(workingpoint_eff_singleConvTrackRej, data.iloc[np.argmin(np.abs(data['S_eff_singleTrackConvRejMVA']-workingpoint_eff_singleConvTrackRej))]['significance_singleTrackConvRejMVA'],
         color=color_singleConvTrackRej,
         marker='o',
         markersize=7,
         fillstyle='none',
         mew=1,
         linestyle='none')

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
ax.plot(.5963, 214.9077,
        label='RP conv. rej. (class. cuts) + prefiltering $\phi_V>\pi/2$, $mass<0.05$',
        color=color_RPConvRejClass,
        marker='o',
        markersize=8,
        linestyle='none',
        alpha=.6)
ax.plot(.7751, 246.6994,
        label='     $\phi_V>2$, $mass<0.04$',
        color=color_RPConvRejClass,
        marker='o',
        markersize=6,
        linestyle='none',
        alpha=.6)
ax.plot(.9115, 267.7348,
        label='     $\phi_V>2.4$, $mass<0.01$',
        color=color_RPConvRejClass,
        marker='o',
        markersize=4,
        linestyle='none',
        alpha=.6)
ax.plot(.9860, 263.7709,
        label='     $\phi_V>2.9$, $mass<0.0035$',
        color=color_RPConvRejClass,
        marker='o',
        markersize=3,
        linestyle='none',
        alpha=.6)


plt.legend(loc=4, fontsize=7)

plt.xlim([-.05,1.05])
plt.xlabel('Signal efficiency')
plt.ylabel('Significance')
plt.savefig('temp_output/significances_effS.png')
plt.savefig('temp_output/significances_effS.pdf')



plt.figure(figsize=(fig_width,fig_height), dpi=fig_dpi)

ax = plt.gca()

ax.plot(data['MVAcut'], data['significance_RPConvRejMVA'],
        label='RP conv. rej. + prefiltering',
        color=color_RPConvRejMVA)

ax.plot(data['MVAcut'], data['significance_CombConvRejMVA'],
        label='comb. conv. rej.',
        color=color_CombConvRej)

ax.plot(data['MVAcut'], data['significance_singleTrackConvRejMVA'],
        label='single-track conv. rej.',
        color=color_singleConvTrackRej)

plt.legend()

plt.xlabel('MVA cut')
plt.ylabel('Significance')
plt.savefig('temp_output/significances_MVAcut.png')
plt.savefig('temp_output/significances_MVAcut.pdf')


plt.figure(figsize=(fig_width,fig_height), dpi=fig_dpi)

ax = plt.gca()

ax.plot(data['MVAcut'], data['B_eff_RPConvRejMVA'],
        color=color_RPConvRejMVA,
        linestyle='--',
        alpha=.8)

ax.plot(data['MVAcut'], data['B_eff_CombConvRejMVA'],
        color=color_CombConvRej,
        linestyle='--',
        alpha=.8)

ax.plot(data['MVAcut'], data['B_eff_singleTrackConvRejMVA'],
        color=color_singleConvTrackRej,
        linestyle='--',
        alpha=.8)

ax.plot(data['MVAcut'], data['S_eff_RPConvRejMVA'],
        label='RP conv. rej. + prefiltering',
        color=color_RPConvRejMVA)

ax.plot(data['MVAcut'], data['S_eff_CombConvRejMVA'],
        label='comb. conv. rej.',
        color=color_CombConvRej)

ax.plot(data['MVAcut'], data['S_eff_singleTrackConvRejMVA'],
        label='single-track conv. rej.',
        color=color_singleConvTrackRej)


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

plt.legend(handles, labels)

plt.xlabel('MVA cut')
plt.ylabel('Signal (background) efficiency')
plt.xlim([-.05, 1.05])
plt.ylim([-.05, 1.05])
plt.savefig('temp_output/effS_MVAcut.png')
plt.savefig('temp_output/effS_MVAcut.pdf')
