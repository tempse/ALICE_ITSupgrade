from keras.models import Sequential
from keras.layers import Dense, Dropout, AlphaDropout GaussianNoise, BatchNormalization


def DNNBinaryClassifier(nr_of_layers = 2,
                        first_layer_size = 100,
                        layers_slope_coeff = 1.0,
                        dropout = .5,
                        use_alpha_dropout = False,
                        normalize_batch = False,
                        noise = 1.,
                        activation = 'relu',
                        kernel_initializer = 'glorot_normal',
                        bias_initializer = 'glorot_normal',
                        input_dim = 2):
    
    model = Sequential()
    
    model.add(Dense(first_layer_size,
                    input_dim = input_dim,
                    activation = activation,
                    kernel_initializer = kernel_initializer,
                    bias_initializer = bias_initializer))

    if layers_slope_coeff != 1:
        current_layer_size = int(first_layer_size * layers_slope_coeff) + 1
    else:
        current_layer_size = int(first_layer_size)
    
    for index_of_layer in range(nr_of_layers - 1):
        if normalize_batch:
            model.add(BatchNormalization())

        if use_alpha_dropout:
            model.add(AlphaDropout(dropout))
        else:
            model.add(Dropout(dropout))
            
        #if index_of_layer%2==0: model.add(GaussianNoise(noise))
        model.add(Dense(current_layer_size,
                        activation = activation,
                        kernel_initializer = kernel_initializer,
                        bias_initializer = bias_initializer))
        if layers_slope_coeff != 1:
            current_layer_size = int(current_layer_size * layers_slope_coeff) + 1

    if normalize_batch:
        model.add(BatchNormalization())
        
    model.add(Dense(1,
                    kernel_initializer = kernel_initializer,
                    activation = 'sigmoid'))
    
    model.compile(loss = 'binary_crossentropy',
                  optimizer = 'adam',
                  metrics=['accuracy'])

    print(model.summary())
    
    return model
