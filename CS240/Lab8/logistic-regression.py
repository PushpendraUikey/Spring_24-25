import numpy as np
from sklearn.datasets import load_iris
from sklearn.linear_model import LogisticRegression
from sklearn.model_selection import train_test_split
from sklearn.metrics import accuracy_score

################## using sklearn model ####################

# Load dataset (binary classification: class 0 vs not 0)
iris = load_iris()
X = iris.data
y = (iris.target == 0).astype(int)  # Binary labels

# Split data
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=42)

# Train logistic regression model
model = LogisticRegression()
model.fit(X_train, y_train)

# Predict
y_pred = model.predict(X_test)

# Accuracy
print("Accuracy:", accuracy_score(y_test, y_pred))



################### Using gradient descent & sigmoid: scratch implementation

# Sigmoid function
def sigmoid(z):
    return 1 / (1 + np.exp(-z))

# Loss function
def compute_loss(y, y_pred):
    m = len(y)
    return -np.mean(y * np.log(y_pred + 1e-9) + (1 - y) * np.log(1 - y_pred + 1e-9))

# Gradient Descent
def train(X, y, lr=0.1, epochs=1000):
    m, n = X.shape
    X = np.c_[np.ones((m, 1)), X]  # Add bias term
    weights = np.zeros(n + 1)

    for _ in range(epochs):
        z = np.dot(X, weights)
        y_pred = sigmoid(z)
        gradient = np.dot(X.T, (y_pred - y)) / m
        weights -= lr * gradient

    return weights

# Predict
def predict(X, weights):
    X = np.c_[np.ones((X.shape[0], 1)), X]
    return sigmoid(np.dot(X, weights)) >= 0.5

# Load dataset (binary classification: class 0 vs not 0)
iris = load_iris()
X = iris.data
y = (iris.target == 0).astype(int)

# Split data
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.2, random_state=42)

# Train model
weights = train(X_train, y_train)

# Predict and evaluate
y_pred = predict(X_test, weights)
accuracy = np.mean(y_pred == y_test)
print("Accuracy:", accuracy)
