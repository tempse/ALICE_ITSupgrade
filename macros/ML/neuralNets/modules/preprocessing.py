import sys

import numpy as np

from sklearn.externals import joblib
from sklearn.preprocessing import StandardScaler

def preprocess(X, output_prefix, load_fitted_attributes=False):
    """
    Performs preprocessing steps on the data and returns it as a numpy array.
    """

    print('Data preprocessing: Scaling to zero mean and unit variance')

    if(load_fitted_attributes and output_prefix==None):
        print('Error: Cannot load fitted attributes during data preprocessing. Variable "output_prefix" is not defined.')
        sys.exit()
    
    scaler_attributes_filename = str(output_prefix) + 'StandardScaler_attributes.pkl'
    
    if load_fitted_attributes:
        print('Loading previously determined scaler attributes...')
        scaler_attributes = joblib.load(scaler_attributes_filename)
        scaler_mean = scaler_attributes[0,:]
        scaler_scale = scaler_attributes[1,:]
        X -= scaler_mean
        X /= scaler_scale
        return X
    
    else:
        scaler = StandardScaler()
        X = scaler.fit_transform(X)
        Xfeats_mean = scaler.mean_
        Xfeats_scale = scaler.scale_
        Xfeats_var = scaler.var_
        # store the (vstacked) array of shape (3,n_feats)
        joblib.dump(np.array([Xfeats_mean, Xfeats_scale, Xfeats_var], dtype=np.float32),
                    scaler_attributes_filename)
        return X
