import time

import keras

from sklearn.metrics import roc_auc_score


class callback_ROC(keras.callbacks.Callback):
    """
    Custom callback class for Keras which calculates and plots ROC AUCs after each training epoch.
    """
    
    def __init__(self):
        self.best=0
        self.wait=0

    def on_train_begin(self, logs={}):
        self.aucs_val = []
        self.aucs_train = []
        self.losses = []
 
    def on_train_end(self, logs={}):
        return
 
    def on_epoch_begin(self, epoch, logs={}):
        global start_time 
        start_time = time.time()
        return
 
    def on_epoch_end(self, epoch, logs={}):
        loss = logs.get('val_loss')
        self.losses.append(loss)
        global roc_auc_val
        if(epoch%10 != 0):
        
            y_pred_val = self.model.predict(X_val)
            roc_auc_val = roc_auc_score(self.validation_data[1], y_pred_val)
            self.aucs_val.append(roc_auc_val)
            self.aucs_train.append(0)
            
            print("Epoch {} took {:.1f}s".format(epoch, time.time() - start_time)),
            print("   LogLoss: {:.4f}".format(loss)),
            print("   VAL AUC: {:.3f} %".format( roc_auc_val * 100))    
            
        if(epoch%10 == 0):
            y_pred_val = self.model.predict(X_val)
            
            roc_auc_val = roc_auc_score(self.validation_data[1], y_pred_val)
            self.aucs_val.append(roc_auc_val)

            y_pred_train = self.model.predict(X_train)
            roc_auc_train = roc_auc_score(y_train, y_pred_train) 
            self.aucs_train.append(roc_auc_train)
        
            print("Epoch {} took {:.1f}s".format(epoch, time.time() - start_time)),
            print("   LogLoss: {:.4f}".format(loss)),        
            print("   VAL AUC: {:.3f} %".format( roc_auc_val  * 100)),
            print("   TRAIN AUC: {:.3f} %".format( roc_auc_train * 100))

            plt.clf()
            plt.plot(self.aucs_val, label='validation sample', color='C1')
            plt.plot(self.aucs_train, label='training sample', \
                     marker='o', fillstyle='none', markersize=4, mew=2, linestyle='none', color='C0')
            plt.xlabel("Epochs")
            plt.ylabel("ROC AUC")
            plt.legend(loc='best', fontsize=16)
            plt.grid(True)
            plt.ylim(0.5,1.05)
            plt.tight_layout()
            plt.savefig('temp_output/ann/learningcurve_rocauc_epochs.png')
            plt.savefig('temp_output/ann/learningcurve_rocauc_epochs.pdf')

        current = roc_auc_val
        if current > self.best:
            self.best = current
            self.wait = 0
            self.model.save("temp_output/ann/keras_weights/model_{}_ROC_AUC{:.5f}.hdf5".format(epoch,current),overwrite=True)
            self.model.save("temp_output/ann/keras_weights/weights_final.hdf5",overwrite=True)

        else:
            if self.wait >= 10:             #patience
                # self.model.stop_training = True
                print('Epoch %05d: early stopping' % (epoch))
                
                
            self.wait += 1 #increment the number of times without improvement
        return

    def on_batch_begin(self, batch, logs={}):
        return
 
    def on_batch_end(self, batch, logs={}):
        return
