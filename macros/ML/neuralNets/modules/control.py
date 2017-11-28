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

    parser.add_argument('-nrows', '-nentries',
                        help='specify number of rows to read from the datafile',
                        action='store',
                        dest='num_rows',
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
