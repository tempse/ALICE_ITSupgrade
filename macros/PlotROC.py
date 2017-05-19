import matplotlib.pyplot as plt
import seaborn as sns; sns.set()

color_RPConvRejClass = '#666666'
color_RPConvRejMVA = '#dd0000'
color_singleConvTrackRej = '#4444ff'
color_CombConvRej = 'green'

marker_opt = 'o'
marker_up = '^'
marker_down = 'v'
marker_man = 'x'

plt.figure(figsize=(6.83,5.12), dpi=150)
plt.xlim(-.05,1.05)
plt.ylim(-.05,1.05)
plt.title('ROC of the different analyses')
plt.xlabel('false positive rate')
plt.ylabel('true positive rate')
plt.plot([0,1], [0,1], 'k--')


# dummy points
plt.plot(-99,-99, marker=marker_opt, color='black',
         label='TMVA-optimized cuts',
         fillstyle='none', linestyle='none', mew=2, markersize=8)
plt.plot(-99,-99, marker=marker_up, color='black',
         label='cut variations: +10%',
         fillstyle='none', linestyle='none', mew=2, markersize=8)
plt.plot(-99,-99, marker=marker_down, color='black',
         label='cut variations: -10%',
         fillstyle='none', linestyle='none', mew=2, markersize=8)
plt.plot(-99,-99, marker=marker_man, color='black',
         label='manual cuts (for signal efficiency at ~0.6)',
         fillstyle='none', linestyle='none', mew=2, markersize=8)
plt.plot(-99,-99, marker='None', color=color_RPConvRejClass,
         label='RP conv. rej. via classical cuts',
         linestyle='-', linewidth=7)
plt.plot(-99,-99, marker='None', color=color_RPConvRejMVA,
         label='RP conv. rej. via MVA cuts',
         linestyle='-', linewidth=7)
plt.plot(-99,-99, marker='None', color=color_singleConvTrackRej,
         label='Single conv. track rej. via MVA cuts',
         linestyle='-', linewidth=7)
plt.plot(-99,-99, marker='None', color=color_CombConvRej,
         label='Comb. conv. rej. via MVA cuts',
         linestyle='-', linewidth=7)
# end of dummy points


plt.plot(.289, .363, 'o', color=color_RPConvRejClass,
         label='$\phi_V>\pi/2$, $mass<0.05$', markersize=2)
plt.plot(.230, .373, 'o', color=color_RPConvRejClass,
         label='$\phi_V>2$, $mass<0.04$', markersize=4)
plt.plot(.132, .374, 'o', color=color_RPConvRejClass,
         label='$\phi_V>2.4$, $mass<0.01$', markersize=6)
plt.plot(.936, .987, 'o', color=color_RPConvRejClass,
         label='$\phi_V>2.9$, $mass<0.0035$', markersize=8)

plt.plot(.223,.77, marker=marker_opt, color=color_RPConvRejMVA,
         #label='TMVA-optimized cut',
         fillstyle='none', mew=2, markersize=8)
plt.plot(.123, .726, marker=marker_up, color=color_RPConvRejMVA,
         #label='cut variation: +10%',
         fillstyle='none', mew=2, markersize=8)
plt.plot(.446, .853, marker=marker_down, color=color_RPConvRejMVA,
         #label='cut variation: -10%',
         fillstyle='none', mew=2, markersize=8)
plt.plot(.005, .556, marker=marker_man, color=color_RPConvRejMVA,
         #label='manual cut (at 0.6)',
         fillstyle='none', mew=2, markersize=8)

plt.plot(.595, .885, marker=marker_opt, color=color_singleConvTrackRej,
         #label='TMVA-optimized cut',
         fillstyle='none', mew=2, markersize=8)
plt.plot(.563, .811, marker=marker_up, color=color_singleConvTrackRej,
         #label='cut variation: +10%',
         fillstyle='none', mew=2, markersize=8)
plt.plot(.646, .874, marker=marker_down, color=color_singleConvTrackRej,
         #label='cut variation: -10%',
         fillstyle='none', mew=2, markersize=8)
plt.plot(.299, .491, marker=marker_man, color=color_singleConvTrackRej,
         #label='manual cut (at 0.2)',
         fillstyle='none', mew=2, markersize=8)

plt.plot(.736, .986, marker=marker_opt, color=color_CombConvRej,
         #label='TMVA-optimized cut',
         fillstyle='none', mew=2, markersize=8)
plt.plot(.714, .972, marker=marker_up, color=color_CombConvRej,
         #label='cut variation: +10%',
         fillstyle='none', mew=2, markersize=8)
plt.plot(.746, .993, marker=marker_down, color=color_CombConvRej,
         #label='cut variation: -10%',
         fillstyle='none', mew=2, markersize=8)
plt.plot(.328, .600, marker=marker_man, color=color_CombConvRej,
         #label='TMVA-optimized cut',
         fillstyle='none', mew=2, markersize=8)



plt.legend(loc=4, ncol=1, fontsize='xx-small', markerscale=.75)

plt.savefig('temp_output/ROC_comparison.png')
plt.savefig('temp_output/ROC_comparison.pdf')
