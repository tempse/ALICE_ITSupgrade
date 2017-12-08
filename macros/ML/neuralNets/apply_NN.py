from __future__ import division

import numpy as np
import pandas as pd
import root_numpy

from sklearn.preprocessing import scale
from sklearn.externals import joblib

from keras.models import load_model

from modules.get_output_paths import get_output_paths
from modules.load_data import load_data, get_branches, engineer_features
from modules.preprocessing import preprocess

def main():

    output_prefix, keras_models_prefix = get_output_paths()
    
    model_filename = output_prefix + keras_models_prefix + 'weights_final.hdf5'
    
    data_filename = '[DATA TO BE CLASSIFIED]'
    track_identifier = '[tree name]'
    
    num_datasplits = 20
    
    predictions_filename = output_prefix + 'predictions_NN.root'

    # determine the number of entries in the tree by importing a single branch
    num_entries = pd.DataFrame(root_numpy.root2array(data_filename, branches=get_branches(track_identifier)[0])).shape[0]

    print('Loading previously trained model...')
    clf = load_model(model_filename)


    Yscore = np.empty((0,1))

    num_steps = num_datasplits if (num_entries%num_datasplits)==0 else num_datasplits+1
    
    for split_step in range(num_steps):
        start = split_step*int(num_entries/(num_datasplits*1.0))
        stop = (split_step+1)*int(num_entries/(num_datasplits*1.0)) if (split_step+1)*int(num_entries/(num_datasplits*1.0))<=num_entries else num_entries

        print('\n------------------------- PART %d of %d -------------------------' % (split_step+1, num_datasplits))
        print('Reading file %s...' % data_filename)
        dataSample_orig = load_data(data_filename,
                                    branches=get_branches(track_identifier),
                                    start=start,
                                    stop=stop)
    

        Xapp = engineer_features(dataSample_orig, track_identifier)
        
        # data preprocessing
        Xapp = preprocess(Xapp, track_identifier, load_fitted_attributes=True)
        
        print('Applying the trained classifier...')
        Yscore_chunk = clf.predict_proba(Xapp.as_matrix()).astype(np.float32)

        
        print('Tagging entries outside the training scope as untrustworthily classified...')
        Yscore_chunk[np.where(dataSample_orig['mass']<0.05)] = 99

        
        Yscore = np.concatenate((Yscore, Yscore_chunk))
        print('\n')


    # export predictions
    print('\nExporting predictions to file %s...' % predictions_filename)
    Yscore = np.array(Yscore, dtype=[('NN', np.float32)])

    root_numpy.array2root(Yscore,
                          predictions_filename,
                          treename=track_identifier + '_MVAoutput',
                          mode='recreate')


if __name__ == '__main__':
    main()
