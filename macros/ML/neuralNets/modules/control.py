import os
import sys
import argparse
import configparser


def get_input_args(argv=None):

    parser = argparse.ArgumentParser(description='Basic framework for neural net training with Keras')

    
    parser.add_argument('-p', '-pretrained',
                        help='load pretrained model instead of performing the training',
                        action="store_true",
                        dest='load_pretrained_model',
                        default=False)

    parser.add_argument('-nentries', '-nrows',
                        help='specify number of rows to read from the datafile',
                        action='store',
                        dest='num_entries',
                        default=None,
                        type=int)

    parser.add_argument('-num_val_sample',
                        help='specify number of entries in the validation sample',
                        action='store',
                        dest='num_val_sample',
                        default=-1,
                        type=int)

    parser.add_argument('-num_test_sample',
                        help='specify number of entries in the test sample',
                        action='store',
                        dest='num_test_sample',
                        default=-1,
                        type=int)

    parser.add_argument('-frac_val_sample',
                        help='fraction of entries in the validation sample',
                        action='store',
                        dest='frac_val_sample',
                        default=0.2,
                        type=float)

    parser.add_argument('-frac_test_sample',
                        help='fraction of entries in the test sample',
                        action='store',
                        dest='frac_test_sample',
                        default=1/3.,
                        type=float)

    parser.add_argument('-nepochs',
                        help='specify number of epochs for the training',
                        action='store',
                        dest='num_epochs',
                        default=101,
                        type=int)

    parser.add_argument('-batchsize',
                        help='specify number of entries per training batch',
                        action='store',
                        dest='batchsize',
                        default=512,
                        type=int)

    # parser.add_argument('-njobs',
    #                     help='specify number of parallel processes (default: -1)',
    #                     action='store',
    #                     dest='num_jobs',
    #                     default=-1,
    #                     type=int)

    parser.add_argument('-verbose',
                        help='make the program more chatty',
                        action='store_true',
                        dest='verbose_setting',
                        default=False)

    parser.add_argument('-data_params', '-data_config',
                        help='keyword to identify a data configuration from the data config file',
                        action='store',
                        dest='data_params_id',
                        default=None,
                        type=str)

    parser.add_argument('-classifier_params', '-classifier_config', '-model_params', '-model_config',
                        help='keyword to identify a classifier configuration from the classifier config file',
                        action='store',
                        dest='classifier_params_id',
                        default=None,
                        type=str)

    return parser.parse_args(argv)


def get_run_params(parser_results):
    """
    Receives parser arguments as input and returns a dictionary of run parameters.
    """

    run_params = {
        'load_pretrained_model': parser_results.load_pretrained_model,
        'num_entries':           parser_results.num_entries,
        'num_val_sample':        parser_results.num_val_sample,
        'num_test_sample':       parser_results.num_test_sample,
        'frac_val_sample':       parser_results.frac_val_sample,
        'frac_test_sample':      parser_results.frac_test_sample,
        'num_epochs':            parser_results.num_epochs,
        'batchsize':             parser_results.batchsize,
        'verbose_setting':       parser_results.verbose_setting
        }

    if parser_results.data_params_id:
        run_params['data_params_id'] = parser_results.data_params_id
    else:
        try:
            run_params['data_params_id'] = os.environ['DATA_PARAMS']
            print('Using data parameters: {}'.format(run_params['data_params_id']))
        except KeyError:
            print('Error: variable "data_params" not specified. ' \
                  'Define it either via the "-data_params" flag or ' \
                  'set the environment variable DATA_PARAMS and run ' \
                  'the program again.')
            sys.exit(1)

    if parser_results.classifier_params_id:
        run_params['classifier_params_id'] = parser_results.classifier_params_id
    else:
        try:
            run_params['classifier_params_id'] = os.environ['MODEL_PARAMS']
            print('Using model parameters: {}'.format(run_params['classifier_params_id']))
        except KeyError:
            print('Error: variable "model_params" not specified. ' \
                  'Define it either via the "-model_params" flag or ' \
                  'set the environment variable MODEL_PARAMS and run ' \
                  'the program again.')
            sys.exit(1)

    if(run_params['num_val_sample'] == -1):
        run_params['val_sample_size'] = run_params['frac_val_sample']
    else:
        run_params['val_sample_size'] = run_params['num_val_sample']

    if(run_params['num_test_sample'] == -1):
        run_params['test_sample_size'] = run_params['frac_test_sample']
    else:
        run_params['test_sample_size'] = run_params['num_test_sample']

    return run_params


def get_data_params(identifier):
    """
    Returns the relevant section parameters (specified by 'identifier') of the data configuration file as a dictionary.
    """

    print('\nImport data configuration: {}'.format(identifier))
    
    config_file_path = 'config/data_params.conf'

    config = configparser.ConfigParser()
    config.read(config_file_path)

    section = identifier
    options = config.options(section)

    data_config = {}

    for option in options:
        try:
            data_config[option] = config.get(section, option)
            if data_config[option] == -1:
                print('Skipping: {}'.format(option))

        except:
            print('Exception on {}.'.format(option))
            data_config[option] = None

    return data_config


def get_classifier_params(identifier):
    """
    Returns the relevant section parameters (specified by 'identifier') of the classifier configuration file as a dictionary.
    """

    print('\nImport classifier configuration: {}'.format(identifier))
    
    config_file_path = 'config/classifier_params.conf'

    config = configparser.ConfigParser()
    config.read(config_file_path)

    section = identifier
    options = config.options(section)

    data_config = {}

    for option in options:
        try:
            data_config[option] = config.get(section, option)
            if data_config[option] == -1:
                print('Skipping: {}'.format(option))

        except:
            print('Exception on {}.'.format(option))
            data_config[option] = None

    return data_config
