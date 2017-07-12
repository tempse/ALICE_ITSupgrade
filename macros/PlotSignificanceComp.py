import matplotlib.pyplot as plt
import seaborn as sns; sns.set()

import numpy as np
from numpy import trapz
import pandas as pd
import root_numpy


filename = "~/analysis/code/FT2_AnalysisResults_Upgrade/macros/temp_output/significance_data.root"

color_RPConvRejMVA = '#dd0000'
color_singleConvTrackRej = '#4444ff'
color_CombConvRej = 'green'

fig_width = 6.83
fig_height = 5.12
fig_dpi = 150


data = pd.DataFrame(root_numpy.root2array(filename))


plt.figure(figsize=(fig_width,fig_height), dpi=fig_dpi)

ax = plt.gca()

ax.plot(data['S_eff_RPConvRejMVA'], data['significance_RPConvRejMVA'],
        label='RP conv. rej. + prefiltering',
        color=color_RPConvRejMVA)

ax.plot(data['S_eff_CombConvRejMVA'], data['significance_CombConvRejMVA'],
        label='comb. conv. rej.',
        color=color_CombConvRej)

ax.plot(data['S_eff_singleTrackConvRejMVA'], data['significance_singleTrackConvRejMVA'],
        label='single-track conv. rej.',
        color=color_singleConvTrackRej)

plt.legend(loc=4)

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
