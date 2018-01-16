import numpy as np
import matplotlib.pyplot as plt
import seaborn as sns; sns.set(rc={'figure.figsize':(11.7,8.27)},
                               font_scale=1.25)

color_list = ['cerulean', 'slate blue', 'leaf green', 'scarlet', 'pumpkin']
sns.set_palette(sns.xkcd_palette(color_list))

N = 3

# HF_1, HF_3, HF_2
defaultFeats = (98.5, 76.9, 81.7)
noMassFeat = (98.4, 0., 0.)
noKinematicFeats = (90.8, 71.1, 71.6)
wHFfeats = (98.4, 75.9, 0.)
noKinematicFeats_wHFfeats = (89.5, 69.5, 0.)

ind = np.arange(N) # the x locations for the groups
width = 0.15 # the width of the bars

fig, ax = plt.subplots()
bars_defaultFeats = ax.bar(ind, defaultFeats, width, alpha=.75)
bars_noMassFeat = ax.bar(ind+width, noMassFeat, width, alpha=.75)
bars_noKinematicFeats = ax.bar(ind+width*2, noKinematicFeats, width, alpha=.75)
bars_wHFfeats = ax.bar(ind+width*3, wHFfeats, width, alpha=.75)
bars_noKinematicFeats_wHFfeats = ax.bar(ind+width*4, noKinematicFeats_wHFfeats, width, alpha=.75)

ax.set_ylabel('ROC AUC [%]')
ax.set_xticks(ind + width*0.5)
ax.set_xticklabels(('RP::HF','CombBG::HF','NonConvs::HF'))
ax.legend((bars_defaultFeats[0],
           bars_noMassFeat[0],
           bars_noKinematicFeats[0],
           bars_wHFfeats[0],
           bars_noKinematicFeats_wHFfeats[0]),
          ('defaultFeats','noMassFeat','noKinematicFeats','wHFfeats','noKinematicFeats + wHFfeats'),
          ncol=2)

def autolabel(rects):
    """
    Attach a text label above each bar displaying its height
    """
    for rect in rects:
        height = rect.get_height()
        ax.text(rect.get_x() + rect.get_width()/2., 1.02*height,
                '%.1f' % (height),
                ha='center', va='bottom')

autolabel(bars_defaultFeats)
autolabel(bars_noMassFeat)
autolabel(bars_noKinematicFeats)
autolabel(bars_wHFfeats)
autolabel(bars_noKinematicFeats_wHFfeats)

plt.savefig('temp_output/HFstudy_ROCAUC-comparison.png')
plt.savefig('temp_output/HFstudy_ROCAUC-comparison.pdf')
