import pandas as pd
from sklearn.model_selection import train_test_split
from sklearn.preprocessing import StandardScaler
from sklearn.svm import SVC
from sklearn.metrics import classification_report

dataframe = pd.read_csv("./transfusion.data")
print(dataframe.columns)

X = dataframe[['Recency (months)', 'Frequency (times)', 'Monetary (c.c. blood)', 'Time (months)']].to_numpy()
y = dataframe['whether he/she donated blood in March 2007']

print(X.shape)
print(y.shape)

# split the dataset to train and test
X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=248, random_state=10)
"""
The random_state argument in scikit-learn’s train_test_split function controls the randomness 
of the data shuffling before splitting. Setting it to an integer ensures that every time you 
run the code, the same split is produced, which is useful 
for reproducibility. If it's left as None, a different random split will be generated each time.
"""
print(X_train.shape)
print(X_test.shape)
print(y_train.shape)
print(y_test.shape)

# means each feature is transformed so that it has a mean of 0 and a standard deviation of 1: Each data contribute training equally
scaling_model = StandardScaler()
scaling_model.fit(X_train)
print(X_train[0:2])
X_train = scaling_model.transform(X_train)
print(X_train[0:2])
X_test = scaling_model.transform(X_test)

# Train SVM classifier
SVM_classifier = SVC(C=1.0, kernel='linear', class_weight='balanced')
SVM_classifier.fit(X_train, y_train)
print(len(SVM_classifier.support_vectors_))

# Predict outputs on trained classifier
y_predicted = SVM_classifier.predict(X_test)
print(y_predicted)

# generate report: Precision : Recall : F1-Score : Support Vectors
report = classification_report(y_test, y_predicted)
print(report)