import numpy as np
from sklearn.svm import SVC
from sklearn.preprocessing import StandardScaler
from sklearn.preprocessing import OneHotEncoder

class ClassificationPipeline:
    """
    Two-stage classification pipeline: SVM followed by FFNN.
    
    The SVM classifier determines the likelihood of one class from the rest.
    The FFNN determines probabilities for the remaining two classes.
    """
    
    def __init__(self, svm_target=0):
        """
        Initialize the classification pipeline.
        
        Parameters:
        -----------
        svm_target : int
            Specifies the target class for the SVM (0, 1, or 2).
        """
        # Store the target class for the SVM
        self.svm_target = svm_target
        self.y_binary = None
        
        # TODO: Initialize the SVM classifier
        self.svm = SVC(kernel='linear', C=1.0)
        
        # TODO: Initialize attributes for FFNN
        self.c1 = None
        self.c2 = None
        self.wc1 = np.random.randn(4,1)*0.01
        self.wc2 = np.random.randn(4,1)*0.01
        self.b1 = np.random.randn(2,1)*0.01
        for i in range(3):
            if i is not self.svm_target:
                if self.c1 is None:
                    self.c1 = i
                else:
                    self.c2 = i
        
        # You may add additional attributes as needed for your implementation
        
        
    def fitSVM(self, X, y):
        """
        Convert the original labels into binary labels and fit the SVM.
        
        Parameters:
        -----------
        X : numpy.ndarray
            Training feature data
        y : numpy.ndarray
            Training labels
        """
        # TODO: Convert labels to binary (1 for target class, 0 for others)
        self.y_binary = np.where(y == self.svm_target, 1, 0)
        # and fit the SVM model
        self.svm.fit(X, self.y_binary)

        
    def probsSVM(self, X):
        """
        Return the probability estimates from SVM.
        
        Parameters:
        -----------
        X : numpy.ndarray
            Input feature data
            
        Returns:
        --------
        numpy.ndarray
            Probability that each input belongs to the target class
        """
        # TODO: Return probabilities from SVM
        y_pred = self.svm.predict(X)
        y_pred = np.where(y_pred == 1, self.svm_target, -1)
        # print("y_pred: ", y_pred)
        return y_pred
        
    def fitFFNN(self, X, y):
        """
        Train the FFNN component of the pipeline.
        
        Parameters:
        -----------
        X : numpy.ndarray
            Training feature data
        y : numpy.ndarray
            Training labels
        """
        # TODO: Implement FFNN training with backpropagation
        for _ in range(1000):
            # print(self.wc1)
            oc1 = self.wc1.T @ X.T
            oc1 = oc1.reshape(-1,1)
            # print(oc1.shape)
            e1 = (oc1-y.T) @ X
            e1_avg = np.mean(e1, axis=0).reshape(-1,1)
            self.wc1 -= e1_avg

            oc2 = self.wc2.T @ X.T
            oc2 = oc2.reshape(-1,1)
            e2 = (oc2-y.T) @ X 
            e2_avg = np.mean(e2, axis=0).reshape(-1,1)
            self.wc2 -= e2_avg

        
    def probsFFNN(self, X):
        """
        Return the probability estimates from FFNN.
        
        Parameters:
        -----------
        X : numpy.ndarray
            Input feature data
            
        Returns:
        --------
        numpy.ndarray
            Probability estimates for the two classes other than the target class
        """
        # TODO: Return probabilities from FFNN for the other two classes
        return self.wc1.T @ X.T, self.wc2.T @ X.T

    def fit(self, X, y):
        """
        Fit both SVM and FFNN components of the pipeline.
        
        Parameters:
        -----------
        X : numpy.ndarray
            Training feature data
        y : numpy.ndarray
            Training labels
        """
        # TODO: Implement pipeline fitting
        # using the fitSVM and fitFFNN functions
        self.fitSVM(X, y)
        self.fitFFNN(X, y)
        
    def predict(self, X):
        """
        Predict class labels for samples in X.
        
        Parameters:
        -----------
        X : numpy.ndarray(one-hot encoded)
            Input feature data
            
        Returns:
        --------
        numpy.ndarray
            Predicted class labels
        """
        # TODO: Implement prediction using both SVM and FFNN
        # Combine the probabilities and determine the final class prediction
        svm_prediction = self.probsSVM(X)
        c1 , c2 = self.probsFFNN(X)
        c1 = c1.reshape(-1,1)
        c2 = c2.reshape(-1,1)
        final_output = []
        # print(final_output.shape)
        for i in range(X.shape[0]):
            if(svm_prediction[i] != -1) :
                final_output.append( self.svm_target )
            else:
                if c1[i] > c2[i]:
                    final_output.append(self.c1)
                else:
                    final_output.append(self.c2)

        return final_output
        
class StandardNNClassifier:
    """
    Standard Feed-Forward Neural Network classifier for comparison.
    
    This implements a three-class classifier using a neural network.
    """
    
    def __init__(self, input_size=4, output_size=3):
        """
        Initialize the neural network classifier.
        
        Parameters:
        -----------
        input_size : int
            Number of input features
        output_size : int
            Number of output classes
        """
        # TODO: Initialize network architecture and parameters
        self.input_size = input_size
        self.output_size = output_size
        # Add other necessary attributes for your implementation
        self.w1 = np.random.randn(input_size, input_size)*0.01 # 4 X 4
        self.b1 = np.random.randn(input_size, 1)*0.01           # 4 X 1
        self.w2 = np.random.randn(output_size, input_size)*0.01 # 3 X 4
        self.b2 = np.random.randn(output_size, 1) * 0.01           # 3 X 1
        
    def forward(self, X):
        """
        Forward propagation through the network.
        
        Parameters:
        -----------
        X : numpy.ndarray
            Input feature data
            
        Returns:
        --------
        numpy.ndarray
            Output probabilities for each class
        """
        # TODO: Implement forward propagation
        self.a1 = self.w1 @ X  + self.b1 # it should have the dimension of (4 X input stances) as output
        # z1 = sigmoid(a1)
        self.a2 = self.w2 @ a1.T + self.b2 # dimension: 3 X inputsize

        return self.a2.T
        
    def backward(self, X, y, output):
        """
        Backward propagation to update weights.
        
        Parameters:
        -----------
        X : numpy.ndarray
            Input feature data
        y : numpy.ndarray
            True labels 
        output : numpy.ndarray
            Output from forward propagation
        """
        # TODO: Implement backpropagation
        self.e2 = (y - output) * X   # y = n * 3, output n * 3
        self.w2 -= self.e2
        self.e1 = self.w2 @ (y - output) 

        
    def fit(self, X, y):
        """
        Train the neural network using backpropagation.
        
        Parameters:
        -----------
        X : numpy.ndarray
            Training feature data
        y : numpy.ndarray
            Training labels
        """
        # TODO: Implement network training
        
    def predict(self, X):
        """
        Predict class labels for samples in X.
        
        Parameters:
        -----------
        X : numpy.ndarray
            Input feature data
            
        Returns:
        --------
        numpy.ndarray
            Predicted class labels
        """
        # TODO: Implement prediction