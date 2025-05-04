import seaborn as sns
import pandas as pd
from sklearn import datasets
import numpy as np
import matplotlib.pyplot as plt
from sklearn.svm import SVC

# Load dataset: iris dataset
iris = datasets.load_iris()
df = pd.DataFrame(iris.data, columns=iris.feature_names)
df['species'] = iris.target     # Here we assigned the target numbers to species column(ex. 0, 1, 2)

# Map target numbers to species names
species_map = {0: 'Setosa', 1: 'Versicolor', 2: 'Virginica'}
df['species'] = df['species'].map(species_map)  # Mapped target numbers to species names: original df gets modified
# Now the df[species] would contain the species names instead of numbers


# Pairplot to visualize feature relationships
sns.pairplot(df, hue='species', diag_kind='kde')


# Load dataset
iris = datasets.load_iris()
X = iris.data[:, :2]  # Take first two features (sepal length, sepal width)
y = iris.target

# Train SVM
svm = SVC(kernel='linear')
svm.fit(X, y)

# Plot decision boundary
x_min, x_max = X[:, 0].min() - 1, X[:, 0].max() + 1
y_min, y_max = X[:, 1].min() - 1, X[:, 1].max() + 1

## I have no idea about this plotting.
xx, yy = np.meshgrid(np.linspace(x_min, x_max, 100), np.linspace(y_min, y_max, 100))
Z = svm.predict(np.c_[xx.ravel(), yy.ravel()]).reshape(xx.shape)
# gotta learn the contourf function
plt.contourf(xx, yy, Z, alpha=0.3)
plt.scatter(X[:, 0], X[:, 1], c=y, edgecolors='k', cmap=plt.cm.Set1)
plt.xlabel("Sepal Length")
plt.ylabel("Sepal Width")
plt.title("SVM Decision Boundary (Linear Kernel)")
plt.show()


# Load dataset
iris = datasets.load_iris()
X = iris.data[:, :2]  # Take first two features (sepal length, sepal width)
y = iris.target

# Train SVM with RBF Kernel
svm_rbf = SVC(kernel='rbf', gamma=0.5, C=1.0)
svm_rbf.fit(X, y)

# Plot decision boundary
x_min, x_max = X[:, 0].min() - 1, X[:, 0].max() + 1
y_min, y_max = X[:, 1].min() - 1, X[:, 1].max() + 1
xx, yy = np.meshgrid(np.linspace(x_min, x_max, 100), np.linspace(y_min, y_max, 100))
Z = svm_rbf.predict(np.c_[xx.ravel(), yy.ravel()]).reshape(xx.shape)

plt.contourf(xx, yy, Z, alpha=0.3)
plt.scatter(X[:, 0], X[:, 1], c=y, edgecolors='k', cmap=plt.cm.Set1)
plt.xlabel("Sepal Length")
plt.ylabel("Sepal Width")
plt.title("SVM Decision Boundary (RBF Kernel)")
plt.show()

