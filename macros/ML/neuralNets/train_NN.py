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
from modules.control import get_input_args, get_data_params, \
    get_classifier_params
from modules.preprocessing import preprocess
from modules.keras_models import DNNBinaryClassifier
from modules.keras_callbacks import callback_ROC
from modules.evaluation_plots import plot_MVAoutput, plot_cut_efficiencies, \
    plot_ROCcurve, plot_metrics_history
from modules.utils import calculate_pair_sample_weight, print_dict

from keras.models import load_model


def main():

    sys.stdout = logger()

    track_identifier = 'pairTree' # can be either 'pairTree' or 'singleTree'

    data_HFenh = load_data("/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/workingData/HFAnalysis/CA_AnalysisResults_Upgrade_HFenh_pairTree-us_part1_0.2nEvents.root",
                           branches=get_branches(track_identifier),
                           start=0,
                           stop=run_params['num_rows'],
                           selection="((firstMothersInfo1==1 && generator==3) || (firstMothersInfo1==2 && generator==4) || (firstMothersInfo1==2 && generator==5)) && electronsWithHFMother <= 2")

    # add new column with class label
    data_HFenh['class_HF'] = 1
    
    data_GP = load_data("/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/workingData/HFAnalysis/CA_AnalysisResults_Upgrade_GP_pairTree-us_part1_0.2nEvents.root",
                        branches=get_branches(track_identifier),
                        start=0,
                        stop=run_params['num_rows'],
                        selection="IsRP==1 && IsConv==0")

    # add new column with class label
    data_GP['class_HF'] = 0

    data_orig = shuffle(pd.concat([data_HFenh, data_GP]))
    
    #data_orig = load_data("/home/sebastian/analysis/data/FT2_AnalysisResults_Upgrade/workingData/DNNAnalysis/FT2_ITSup_pairTree-us_part1_424650tightCutEvents.root",
    #                      branches=branches_pairTree,
    #                      start=0,
    #                      stop=run_params['num_rows'],
    #                      selection='IsRP==1 && TrackCut1==2 && TrackCut2==2')
    
    # target vector setup
    Y = pd.DataFrame()
    Y = (data_orig['class_HF']==1).astype(int)

    # feature matrix setup
    X = pd.DataFrame()

    X = engineer_features(data_orig, track_identifier)
    
    # save feature names for later purposes
    X_featureNames = list(X)
    print('Selected features:', X_featureNames)
    joblib.dump(X_featureNames, output_prefix+'featureNames.pkl')

    if track_identifier == 'pairTree':
        sample_weight = calculate_pair_sample_weight(data_orig['PIDeff1'], data_orig['PIDeff2'])

    elif track_identifier == 'singleTree':
        sample_weight = data_orig['PIDeff']


    print('Total number of events in data sample: %d' % X.shape[0])
    print('Number of signal events in data sample: %d (%.2f percent)' %
          (Y[Y==1].shape[0], Y[Y==1].shape[0]*100/Y.shape[0]))
    print('Number of backgr events in data sample: %d (%.2f percent)' %
          (Y[Y==0].shape[0], Y[Y==0].shape[0]*100/Y.shape[0]))

    del data_orig

    
    # Data Split in Training, Validation and Test Samples
    
    print('Splitting the data in training, validation and test samples...')
    
    X_train, X_test, y_train, y_test, sample_weight_train, sample_weight_test = train_test_split(X, Y, sample_weight, test_size=run_params['test_sample_size'], random_state=42)

    X_train, X_val, y_train, y_val, sample_weight_train, sample_weight_val = train_test_split(X_train, y_train, sample_weight_train, test_size=run_params['val_sample_size'], random_state=43)

    print('Number of signal events in training sample: %d (%.2f percent)' %
          (y_train[y_train==1].shape[0], y_train[y_train==1].shape[0]*100/y_train.shape[0]))
    print('Number of backgr events in training sample: %d (%.2f percent)' %
          (y_train[y_train==0].shape[0], y_train[y_train==0].shape[0]*100/y_train.shape[0]))
    print('Number of signal events in validation sample: %d (%.2f percent)' %
          (y_val[y_val==1].shape[0], y_val[y_val==1].shape[0]*100/y_val.shape[0]))
    print('Number of backgr events in validation sample: %d (%.2f percent)' %
          (y_val[y_val==0].shape[0], y_val[y_val==0].shape[0]*100/y_val.shape[0]))
    print('Number of signal events in test sample: %d (%.2f percent)' %
          (y_test[y_test==1].shape[0], y_test[y_test==1].shape[0]*100/y_test.shape[0]))
    print('Number of backgr events in test sample: %d (%.2f percent)' %
          (y_test[y_test==0].shape[0], y_test[y_test==0].shape[0]*100/y_test.shape[0]))


    # Data preprocessing
    X_train = preprocess(X_train, output_prefix)
    X_val = preprocess(X_val, output_prefix, load_fitted_attributes=True)
    X_test = preprocess(X_test, output_prefix, load_fitted_attributes=True)

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
        
            print('Creating the model...')
            model_args = {
                "nr_of_layers": int(classifier_params['nr_of_layers']),
                "first_layer_size": int(classifier_params['first_layer_size']),
                "layers_slope_coeff": float(classifier_params['layers_slope_coeff']),
                "dropout": float(classifier_params['dropout']),
                "normalize_batch": bool(classifier_params['normalize_batch']),
                "noise": float(classifier_params['noise']),
                "activation": str(classifier_params['activation']),
                "kernel_initializer": str(classifier_params['kernel_initializer']),
                "bias_initializer": str(classifier_params['bias_initializer']),
                "input_dim": X_train.shape[1]
            }
            print("Model parameters: ", model_args)
            
            model = DNNBinaryClassifier(**model_args)
            
            print('Fitting the model...')
            hist = model.fit(X_train, y_train,
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
            print('Finished training.')
            
            print('Loading model with the highest VAL AUC...')
            model = load_model(output_prefix + keras_models_prefix + 'weights_final.hdf5')


        
    # model evaluation (training sample)

    num_process = X_train.shape[0] if X_train.shape[0]<=X_val.shape[0] else X_val.shape[0]

    print('Evaluating the model on the training sample...')
    y_train_score = model.predict_proba(X_train[0:num_process,:])
    print('\n')

    num_trueSignal, num_trueBackgr = plot_MVAoutput(y_train[0:num_process], y_train_score,
                                                    label='train')

    MVAcut_opt = plot_cut_efficiencies(num_trueSignal, num_trueBackgr)

    plot_ROCcurve(y_train[0:num_process], y_train_score, sample_weight_train[0:num_process],
                  label='train', workingpoint=MVAcut_opt)

    del y_train_score, num_trueSignal, num_trueBackgr
    
    
    # model evaluation (validation sample)

    print('Evaluating the model on the validation sample...')
    y_val_score = model.predict_proba(X_val)
    print('\n')

    plot_ROCcurve(y_val, y_val_score, sample_weight_val, label='val')

    del y_val_score

    
    # model evaluation (test sample)

    print('Evaluating the model on the test sample...')
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

    print('Received input arguments: ')
    pprint(parser_results)
    print('\n')

    run_params = {
        'load_pretrained_model': parser_results.load_pretrained_model,
        'num_rows':              parser_results.num_rows,
        'num_val_sample':        parser_results.num_val_sample,
        'num_test_sample':       parser_results.num_test_sample,
        'frac_val_sample':       parser_results.frac_val_sample,
        'frac_test_sample':      parser_results.frac_test_sample,
        'num_epochs':            parser_results.num_epochs,
        'batchsize':             parser_results.batchsize,
        'verbose_setting':       parser_results.verbose_setting,
        'data_params_id':        parser_results.data_params_id,
        'classifier_params_id':  parser_results.classifier_params_id
        }

    if(run_params['num_val_sample'] == -1):
        run_params['val_sample_size'] = run_params['frac_val_sample']
    else:
        run_params['val_sample_size'] = run_params['num_val_sample']

    if(run_params['num_test_sample'] == -1):
        run_params['test_sample_size'] = run_params['frac_test_sample']
    else:
        run_params['test_sample_size'] = run_params['num_test_sample']
    

    data_params = get_data_params(run_params['data_params_id'])
    print_dict(data_params)
    
    classifier_params = get_classifier_params(run_params['classifier_params_id'])
    print_dict(classifier_params)
    
    main()
    
