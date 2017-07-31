import os


def get_output_paths():
    """
    Ensures that all output paths exist before returning them as strings.
    """

    output_prefix = 'output/'
    keras_models_prefix = 'keras_model_saves/'
    
    if not os.path.exists(output_prefix):
        os.makedirs(output_prefix)

    if not os.path.exists(output_prefix + keras_models_prefix):
        os.makedirs(output_prefix + keras_models_prefix)
        
    return output_prefix, keras_models_prefix
