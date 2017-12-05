from __future__ import division

import sys
from select import select
import os
from pprint import pprint

import matplotlib
matplotlib.use('agg')
import seaborn as sns
sns.set()

import numpy as np
import pandas as pd

from sklearn.utils import shuffle
from sklearn.model_selection import train_test_split
from sklearn.externals import joblib

from modules.get_output_paths import get_output_paths
from modules.logger import logger
from modules.load_data import load_data, get_branches, engineer_features
from modules.control import get_run_params, get_input_args, get_data_params, \
    get_classifier_params
from modules.preprocessing import preprocess
from modules.keras_models import DNNBinaryClassifier
from modules.keras_callbacks import callback_ROC
from modules.evaluation_plots import plot_MVAoutput, plot_cut_efficiencies, \
    plot_ROCcurve, plot_metrics_history
from modules.utils import calculate_pair_sample_weight, print_dict, \
    print_class_counts

from keras.models import load_model


def main():

    sys.stdout = logger()

    data_HFenh = load_data(data_params['hf_datapath'],
                           branches=get_branches(data_params['track_identifier']),
                           start=0,
                           stop=run_params['num_entries'],
                           selection=data_params['hf_selection'])

    # add new column with class label
    data_HFenh[data_params['data_identifier']] = 1
    
    data_GP = load_data(data_params['gp_datapath'],
                        branches=get_branches(data_params['track_identifier']),
                        start=0,
                        stop=run_params['num_entries'],
                        selection=data_params['gp_selection'])

    # add new column with class label
    data_GP[data_params['data_identifier']] = 0

    data_orig = shuffle(pd.concat([data_HFenh, data_GP]))
    
    #data_orig = load_data("/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/workingData/DNNAnalysis/FT2_ITSup_pairTree-us_part1_424650tightCutEvents.root",
    #                      branches=branches_pairTree,
    #                      start=0,
    #                      stop=run_params['num_entries'],
    #                      selection='IsRP==1 && TrackCut1==2 && TrackCut2==2')
    
    # target vector setup
    Y = pd.DataFrame()
    Y = (data_orig[data_params['data_identifier']]==1).astype(int)

    # feature matrix setup
    X = pd.DataFrame()

    X = engineer_features(data_orig, data_params['track_identifier'])
    
    # save feature names for later purposes
    X_featureNames = list(X)
    print('Selected features:', X_featureNames)
    joblib.dump(X_featureNames, output_prefix+'featureNames.pkl')

    if data_params['track_identifier'] == 'pairTree':
        sample_weight = calculate_pair_sample_weight(data_orig['PIDeff1'], data_orig['PIDeff2'])

    elif data_params['track_identifier'] == 'singleTree':
        sample_weight = data_orig['PIDeff']


    print_class_counts(Y, 'total', background=0, signal=1)

    del data_orig

    
    # Data Split in Training, Validation and Test Samples
    
    print('\nSplitting the data in training, validation and test samples...')
    
    X_train, X_test, y_train, y_test, sample_weight_train, sample_weight_test = train_test_split(X, Y, sample_weight, test_size=run_params['test_sample_size'], random_state=42)

    X_train, X_val, y_train, y_val, sample_weight_train, sample_weight_val = train_test_split(X_train, y_train, sample_weight_train, test_size=run_params['val_sample_size'], random_state=43)


    print_class_counts(y_train, 'training', background=0, signal=1)
    print_class_counts(y_val, 'validation', background=0, signal=1)
    print_class_counts(y_test, 'test', background=0, signal=1)


    # Data preprocessing
    X_train = preprocess(X_train, data_params['track_identifier'])
    X_val = preprocess(X_val, data_params['track_identifier'], load_fitted_attributes=True)
    X_test = preprocess(X_test, data_params['track_identifier'], load_fitted_attributes=True)

    # Convert pandas to numpy arrays
    X_train = np.array(X_train)
    X_val = np.array(X_val)
    X_test = np.array(X_test)
    y_train = np.array(y_train)
    y_val = np.array(y_val)
    y_test = np.array(y_test)
    

    if run_params['load_pretrained_model']:
        if not os.path.exists(pretrained_model_filename):
            print('Error loading model. File %s does not exist.' % pretrained_model_filename)
            sys.exit()
            
        print('Loading pretrained model from file %s' % pretrained_model_filename)
        model = load_model(pretrained_model_filename)
        print(model.summary())
    else:
        
        timeout = 5
        print('\nInfo: The model will be trained anew. Existing saves will be overwritten.')
        print('      (Program paused for %d seconds. Enter an arbitrary string to abort the training.)' % timeout)
        rlist = select([sys.stdin], [], [], timeout)[0]

        if rlist:
            print('Input received. Exit the program...')
            sys.exit()
        else:
            print('No input received. Continue running the program...')
        
            print('\nCreating the model...')
            model_args = {
                "nr_of_layers":         int(classifier_params['nr_of_layers']),
                "first_layer_size":     int(classifier_params['first_layer_size']),
                "layers_slope_coeff": float(classifier_params['layers_slope_coeff']),
                "dropout":            float(classifier_params['dropout']),
                "normalize_batch":     bool(classifier_params['normalize_batch']=='True'),
                "noise":              float(classifier_params['noise']),
                "activation":           str(classifier_params['activation']),
                "kernel_initializer":   str(classifier_params['kernel_initializer']),
                "bias_initializer":     str(classifier_params['bias_initializer']),
                "input_dim":            X_train.shape[1]
                "use_alpha_dropout":   bool(classifier_params['use_alpha_dropout']=='True')
            }
            print_dict(model_args, 'Model parameters:')
            
            model = DNNBinaryClassifier(**model_args)
            
            print('\nFitting the model...\n')
            hist = model.fit(X_train, y_train, sample_weight=sample_weight_train,
                             batch_size=run_params['batchsize'],
                             epochs=run_params['num_epochs'],
                             callbacks=[callback_ROC(X_train,
                                                     y_train,
                                                     sample_weight_train,
                                                     output_prefix,
                                                     keras_models_prefix)],
                             verbose=run_params['verbose_setting'],
                             validation_data=(X_val, y_val, sample_weight_val))
            
            plot_metrics_history(hist)
            print('\nFinished training.')
            
            print('\nLoading model with the highest VAL AUC...')
            model = load_model(output_prefix + keras_models_prefix + 'weights_final.hdf5')


        
    # model evaluation (training sample)

    num_process = X_train.shape[0] if X_train.shape[0]<=X_val.shape[0] else X_val.shape[0]

    print('\nEvaluating the model on the training sample...')
    y_train_score = model.predict_proba(X_train[0:num_process,:])
    print('\n')

    num_trueSignal, num_trueBackgr = plot_MVAoutput(y_train[0:num_process], y_train_score,
                                                    label='train')

    MVAcut_opt = plot_cut_efficiencies(num_trueSignal, num_trueBackgr)

    plot_ROCcurve(y_train[0:num_process], y_train_score, sample_weight_train[0:num_process],
                  label='train', workingpoint=MVAcut_opt)

    del y_train_score, num_trueSignal, num_trueBackgr
    
    
    # model evaluation (validation sample)

    print('\nEvaluating the model on the validation sample...')
    y_val_score = model.predict_proba(X_val)
    print('\n')

    plot_ROCcurve(y_val, y_val_score, sample_weight_val, label='val')

    del y_val_score

    
    # model evaluation (test sample)

    print('\nEvaluating the model on the test sample...')
    y_test_score = model.predict_proba(X_test)
    print('\n')

    plot_ROCcurve(y_test, y_test_score, sample_weight_test, label='test')

    del y_test_score


    
if __name__ == "__main__":
    
    # fix random seed for reproducibility
    np.random.seed(7)

    output_prefix, keras_models_prefix = get_output_paths()
    pretrained_model_filename = output_prefix + keras_models_prefix + 'weights_final.hdf5'
    
    user_argv = None

    if(len(sys.argv) > 1):
        user_argv = sys.argv[1:]
    
    parser_results = get_input_args(user_argv)

    run_params = get_run_params(parser_results)
    print_dict(run_params, 'Continue with the following run parameters:')
    
    data_params = get_data_params(run_params['data_params_id'])
    print_dict(data_params)
    
    classifier_params = get_classifier_params(run_params['classifier_params_id'])
    print_dict(classifier_params)
    
    main()
    
