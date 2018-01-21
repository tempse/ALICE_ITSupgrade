import numpy as np
import matplotlib.patches as mpatches
import matplotlib.pyplot as plt
import seaborn as sns;

rc = {'figure.figsize':(11.7,8.27),
      'hatch.color':'#333333'}
sns.set(rc=rc,
        font_scale=1.3)

#color_list = ['coffee', 'steel grey', 'raspberry', 'goldenrod', \
#              'leaf green', 'pumpkin', 'cerulean']
#sns.set_palette(sns.xkcd_palette(color_list))
color_palette = sns.color_palette('YlOrBr_r', 7)
sns.set_palette(color_palette)

alpha = 0.8

pattern_wKinFeats = ''
pattern_woKinFeats = '.'
pattern_DCAvecComp = '|'
pattern_engHFfeats = '/'
pattern_allHFfeats = '|/'

patch_wKinFeats_DCAvecComp = mpatches.Patch(color=color_palette[0], label='default feats, incl. kinematic feats & DCA vector components', alpha=alpha, hatch=pattern_wKinFeats*2+pattern_DCAvecComp*2)
patch_wKinFeats_DCAvecComp.set_edgecolor(rc['hatch.color'])

patch_wKinFeats_DCAvecComp_noMassFeat = mpatches.Patch(color=color_palette[1], label='default feats, excl. mass', alpha=alpha, hatch=pattern_wKinFeats*2+pattern_DCAvecComp*2)
patch_wKinFeats_DCAvecComp_noMassFeat.set_edgecolor(rc['hatch.color'])

patch_wKinFeats = mpatches.Patch(fill=False, hatch=pattern_wKinFeats*3, label='(undotted) with kinematic feats')

patch_woKinFeats = mpatches.Patch(fill=False, hatch=pattern_woKinFeats*3, label='w/o kinematic feats')

patch_DCAvecComp = mpatches.Patch(fill=False, hatch=pattern_DCAvecComp*3, label='DCA vector components only')

patch_engHFfeats = mpatches.Patch(fill=False, hatch=pattern_engHFfeats*3, label='engineered HF feats only')

patch_allHFfeats = mpatches.Patch(fill=False, hatch=pattern_allHFfeats*3, label='DCA vec. comp. & engineered HF feats')



N = 3

# HF_1, HF_3, HF_2
defaultFeats = (98.5, 76.9, 81.7)
noMassFeat = (98.4, 0., 0.)
noKinematicFeats = (90.8, 71.1, 71.6)
wHFfeats = (98.4, 77.1, 0.)
noKinematicFeats_wHFfeats = (80.2, 62.8, 0.)
wAllHFfeats = (98.3, 78.7, 0.)
wAllHFfeats_noKinematicFeats = (92.7, 72.0, 0.)

ind = np.arange(N) # the x locations for the groups
width = 0.11 # the width of the bars

fig, ax = plt.subplots()
bars_defaultFeats = ax.bar(ind, defaultFeats, width, alpha=alpha, hatch=pattern_wKinFeats+pattern_DCAvecComp)
bars_noMassFeat = ax.bar(ind+width, noMassFeat, width, alpha=alpha, hatch=pattern_wKinFeats+pattern_DCAvecComp)
bars_noKinematicFeats = ax.bar(ind+width*2, noKinematicFeats, width, alpha=alpha, hatch=pattern_woKinFeats+pattern_DCAvecComp)
bars_wHFfeats = ax.bar(ind+width*3, wHFfeats, width, alpha=alpha, hatch=pattern_wKinFeats+pattern_engHFfeats)
bars_noKinematicFeats_wHFfeats = ax.bar(ind+width*4, noKinematicFeats_wHFfeats, width, alpha=alpha, hatch=pattern_woKinFeats+pattern_engHFfeats)
bars_wAllHFfeats = ax.bar(ind+width*5, wAllHFfeats, width, alpha=alpha, hatch=pattern_wKinFeats+pattern_allHFfeats)
bars_wAllHFfeats_noKinematicFeats = ax.bar(ind+width*6, wAllHFfeats_noKinematicFeats, width, alpha=alpha, hatch=pattern_woKinFeats+pattern_allHFfeats)

ax.set_ylabel('ROC AUC [%]')
ax.set_xticks(ind+width*2.5)
ax.set_xticklabels(('RP::HF (pair level)','CombBG::HF (pair level)','NonConvs::HF (single-track level)'))
ax.legend(handles=[patch_wKinFeats_DCAvecComp,patch_wKinFeats_DCAvecComp_noMassFeat, patch_DCAvecComp, patch_engHFfeats, patch_wKinFeats, patch_woKinFeats, patch_allHFfeats])
#ax.legend((bars_defaultFeats[0],
#           bars_noMassFeat[0],
#           bars_noKinematicFeats[0],
#           bars_wHFfeats[0],
#           bars_noKinematicFeats_wHFfeats[0],
#           bars_wAllHFfeats[0],
#           bars_wAllHFfeats_noKinematicFeats),
#          ('with kin. feats, DCA vec. comp.','w/o mass feat','w/o kin. feats, DCA vec. comp.','with kin. feats, engineered HF feats','w/o kin. feats, engineered HF feats','with kin. feats, with all HF feats','w/o kin. feats, with all HF feats'),
#          ncol=2)
ax.set_ylim([50,120])

def autolabel(rects):
    """
    Attach a text label above each bar displaying its height
    """
    for rect in rects:
        height = rect.get_height()
        if height > 0:
            ax.text(rect.get_x() + rect.get_width()/2., 1.02*height,
                    '%.1f' % (height),
                    ha='center', va='bottom', rotation=90)

autolabel(bars_defaultFeats)
autolabel(bars_noMassFeat)
autolabel(bars_noKinematicFeats)
autolabel(bars_wHFfeats)
autolabel(bars_noKinematicFeats_wHFfeats)
autolabel(bars_wAllHFfeats)
autolabel(bars_wAllHFfeats_noKinematicFeats)

plt.savefig('temp_output/HFstudy_ROCAUC-comparison.png')
plt.savefig('temp_output/HFstudy_ROCAUC-comparison.pdf')
