import numpy as np
import pandas as pd
from sklearn.pipeline import make_pipeline
from sklearn.preprocessing import StandardScaler
from sklearn.svm import SVC
from sklearn.svm import LinearSVC
from sklearn import metrics
from sklearn.metrics import accuracy_score


train_data=pd.read_csv(r"/content/mnist_train.csv")
test_data=pd.read_csv(r"/content/mnist_test.csv")

train_data=train_data.to_numpy()    # train_data shape: (60000, 785)
m, n = train_data.shape
test_data=test_data.to_numpy()      # test_data shape: (10000, 785)

X_train=train_data[:,1:]            # X_train shape: (60000, 784)
y_train=train_data[:,0]             # y_train shape: (60000,)
X_test=test_data[:,1:]              # X_test shape: (10000, 784)
y_test=test_data[:,0]               # y_test shape: (10000,)

X_train = X_train / 255.0           # Normalizing the data
X_test = X_test / 255.0


### scaled data can also be used instead.
scaler = StandardScaler()
X_train_scaled = scaler.fit_transform(X_train)
X_test_scaled = scaler.fit_transform(X_test)


classifiers = {}
for digit in range(10):
    # Creating a new dataset where the target is 1 if it's the current digit and 0 otherwise
    print("Training the classifier for digit : ", digit)
    y_binary_train = np.where(y_train == digit, 1, 0)

    # Training the SVM classifier
    # classifier = SVC(kernel='linear', C=1.0, random_state=42)
    classifier = LinearSVC(C=1.0, class_weight=None, dual=True, fit_intercept=True, intercept_scaling=1,
                            loss='squared_hinge', max_iter=10000, multi_class='ovr', penalty='l2', 
                            random_state=None, tol=0.0001, verbose=0 )
    classifier.fit(X_train, y_binary_train)
    classifiers[digit] = classifier
    print(f"Training for the digit {digit} completed!\n")



accuracy_scores = {}
for digit in range(10):
    print("Prediction & Report of the digit : ", digit)
    # Predict on the test set
    y_pred = classifiers[digit].predict(X_test)
    y_binary_test = np.where(y_test == digit, 1, 0)

    cr = metrics.classification_report(y_pred, y_binary_test)
    print(cr)
    # Calculate accuracy for the current digit
    # accuracy = accuracy_score(np.where(y_test == digit, 1, 0), y_pred)
    # accuracy_scores[digit] = accuracy
    # f1_score = metrics.f1_score(y_test, y_pred, average='weighted', labels=np.unique(y_pred))
    # precision = metrics.precision_score(y_test, y_pred, average='weighted', labels=np.unique(y_pred))
    # recall = metrics.recall_score(y_test, y_pred, average='weighted', labels=np.unique(y_pred))
    # print(f"Accuracy for digit {digit}: {accuracy}")
    # print(f"F1 score for digit {digit}: {f1_score}")
    # print(f"Precision for digit {digit}: {precision}")
    # print(f"Recall for digit {digit}: {recall}")
    print()


# 4. Prediction (Example)
def predict_digit(image_data, classifiers):
    # Use all classifiers to predict
    predictions = []
    for digit in range(10):
        predictions.append(classifiers[digit].predict(image_data.reshape(1, -1))[0])

    # Return the digit with the highest confidence
    return np.argmax(predictions)

# Example prediction
image_data = train_data[20000][1:]/255.0
predicted_digit = predict_digit(image_data, classifiers)
print(f"Predicted digit: {predicted_digit}")