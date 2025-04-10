import numpy as np
import matplotlib.pyplot as plt
from sklearn.linear_model import LinearRegression
from sklearn.metrics import mean_squared_error

# -----------------------------
# 1. Generate Nonlinear Training Data using a 5th order polynomial with sharp curvature
# -----------------------------
np.random.seed(42)

# Generate 250 evenly spaced points between -3 and 3 for a smooth curve and shuffle them
X_all = np.linspace(-3, 3, 250).reshape(-1, 1)
np.random.shuffle(X_all)

# Split into 200 training points and 50 validation points
X_train = X_all[:200]
y_train = 10 * X_train**5 - 100 * X_train**3 + 40 * X_train + np.random.randn(200, 1) * 30

X_val = X_all[200:]
y_val = 10 * X_val**5 - 100 * X_val**3 + 40 * X_val + np.random.randn(50, 1) * 30

# -----------------------------
# 2. Create polynomial features manually for degree 5 and fit linear regression
# -----------------------------
def create_polynomial_features(X, degree):
    """
    Given an input array X, construct polynomial features up to the specified degree.
    """
    # Concatenate [X^1, X^2, ..., X^(degree)]: np.hstack() concatenates the columns to make one matrix out of it
    return np.hstack([X**i for i in range(1, degree+1)])

def create_polynomial_features_bias(X, degree):
    return np.hstack([np.ones((X.shape[0], 1))] + [X**i for i in range(1, degree+1)])


degree = 5
# X_train_poly = create_polynomial_features(X_train, degree)
# X_val_poly = create_polynomial_features(X_val, degree)
X_train_poly = create_polynomial_features_bias(X_train, degree)
X_val_poly = create_polynomial_features_bias(X_val, degree)

# Initialize and train the linear regression model
model = LinearRegression()
model.fit(X_train_poly, y_train)

# Make predictions on both training and validation sets
y_train_pred = model.predict(X_train_poly)
y_val_pred = model.predict(X_val_poly)

# Calculate mean squared errors
train_mse = mean_squared_error(y_train, y_train_pred)
val_mse = mean_squared_error(y_val, y_val_pred)
print("Training MSE:", train_mse)
print("Validation MSE:", val_mse)

# -----------------------------
# 3. Plot the Data and the Fit
# -----------------------------
# Generate a smooth curve for visualization
X_curve = np.linspace(-3, 3, 300).reshape(-1, 1)
X_curve_poly = create_polynomial_features_bias(X_curve, degree)
y_curve = model.predict(X_curve_poly)

plt.figure(figsize=(10, 6))
plt.scatter(X_train, y_train, color='blue', alpha=0.6, label='Training Data')
plt.scatter(X_val, y_val, color='orange', alpha=0.6, label='Validation Data')
plt.plot(X_curve, y_curve, color='red', linewidth=2, label='Polynomial Fit (Degree 5)')
plt.xlabel('X')
plt.ylabel('y')
plt.title('5th Degree Polynomial Regression Fit')
plt.legend()
plt.show()
