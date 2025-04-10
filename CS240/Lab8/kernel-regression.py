import numpy as np
from sklearn.metrics.pairwise import rbf_kernel
from sklearn.model_selection import train_test_split

# Sample nonlinear data
X = np.linspace(-3, 3, 100).reshape(-1, 1)
y = np.sin(X).ravel() + 0.1 * np.random.randn(100)

# Split
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=42)

# Kernel and parameters
gamma = 1.0
lambda_reg = 1e-2

# Compute kernel matrix
K = rbf_kernel(X_train, X_train, gamma=gamma)

# Compute alpha
alpha = np.linalg.inv(K + lambda_reg * np.eye(len(K))).dot(y_train)

# Predict on test data
K_test = rbf_kernel(X_test, X_train, gamma=gamma)
y_pred = K_test.dot(alpha)

# Evaluate
mse = np.mean((y_pred - y_test) ** 2)
print("MSE:", mse)
