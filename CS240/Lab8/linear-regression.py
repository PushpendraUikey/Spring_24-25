import numpy as np
import pandas as pd

##### Linear Regression using closed form solution
def linear_regression(X, y):
    # Adding the bias term
    X_b = np.c_[np.ones( (X.shape[0], 1) ), X]

    #  closed form solution: w = (x^T.x)^-1.x^T.y
    w = np.linalg.inv( (X_b.T @ X_b) ) @ X_b.T @ y

    return w

def ridge_regression(X, y, alpha):
    # Adding the bias term
    X_b = np.c_[ np.ones( (X.shape[0], 1)), X]

    n_features = X_b.shape[1]
    I = np.eye(n_features)
    I[0,0] = 0 # Don't regularize the bias term

    # closed form solution: w = (x^T.x + alpha*Identity)^-1.x^T.y
    w = np.linalg.inv( (X_b.T @ X_b) + alpha * I ) @ X_b.T @ y 

    return w

# Example data
X = np.array([[1], [2], [3], [4]])
y = np.array([3, 5, 7, 9])  # y ≈ 2x + 1

# Linear regression
w_lin = linear_regression(X, y)
print("Linear weights(intercept, slope):", w_lin)

# Ridge regression
w_ridge = ridge_regression(X, y, alpha=1.0)
print("Ridge weights(intercept, slope):", w_ridge)


