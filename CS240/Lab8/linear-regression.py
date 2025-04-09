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


########### Gradient Descent

def linear_regression_gd(X, y, lr=0.01, epochs = 1000):
    # Adding a bias term
    X_b = np.c_[np.ones((X.shape[0], 1)), X]    # adding the column(n X 1) of bias terms in the matrix

    m, n = X_b.shape

    w = np.zeros((n,1)) # weight vector
    y = y.reshape(-1,1) # making it (m X 1)

    # gd
    for _ in range(epochs):
        gradients = (2/m) * X_b.T @ ( X_b @ w - y )
        w -= lr * gradients
    
    return w

def ridge_regression_gd(X, y, alpha = 1.0, lr=0.01, epochs=1000 ):
    # add the bias term
    X_b = np.c_[np.ones( (X.shape[0], 1) ), X]
    m, n = X_b.shape
    w = np.zeros((n, 1))
    y = y.reshape(-1,1)

    for _ in range(epochs):
        # do not regularize the bias term (w[0])
        reg_term = alpha * np.r_[[[0]], w[1:]]
        gradients = (2/m) * ( X_b.T @ (X_b @ w - y ) + reg_term)
        w -= lr * gradients

    return w


X = np.array([[1], [2], [3], [4]])
y = np.array([3, 5, 7, 9])  # y ≈ 2x + 1

# Linear Regression
w_lin = linear_regression_gd(X, y)
print("Linear GD Weights(intercept, slope):", w_lin.ravel())

# Ridge Regression
w_ridge = ridge_regression_gd(X, y, alpha=1.0)
print("Ridge GD Weights(intercept, slope):", w_ridge.ravel())


"""
Ridge doesn't fully eliminate the effect of outliers — it just reduces the model's sensitivity to them. 
If you want to be more robust, you'd use methods like Huber loss or Lasso with robust statistics.
"""