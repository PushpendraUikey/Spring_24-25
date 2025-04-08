"""
Following is the Tutorial from "https://www.datatechnotes.com/2020/07/classification-example-with-linearsvm-in-python.html"

"""
from sklearn.svm import LinearSVC
from sklearn.datasets import load_iris
from sklearn.datasets import make_classification  # for synthetic classification datset generation
from sklearn.model_selection import train_test_split # splits the dataset
from sklearn.model_selection import cross_val_score # cross validation check of model performace
from sklearn.metrics import confusion_matrix # matrix of Row1: TN , FP ; Row2: FN, TP
from sklearn.metrics import classification_report # Report generator


######### Data Preparation
x, y = make_classification(n_samples=5000, n_features=10, n_classes=3, n_clusters_per_class=1) # class is generated as a single cluster in feature space

X_train, X_test, y_train, y_test = train_test_split(x, y, test_size=0.15) # 15% test & rest train

######### Training the model
lsvc = LinearSVC(verbose=0)  # no ouput printed during the trainig / if set to true, prints evaluation during training
print(lsvc)

LinearSVC(C=1.0, class_weight=None, dual=True, fit_intercept=True, intercept_scaling=1,
          loss='squared_hinge', max_iter=1000, multi_class='ovr', penalty='l2', 
          random_state=None, tol=0.0001, verbose=0 )

lsvc.fit(X_train, y_train)
score = lsvc.score(X_train, y_train)   # How well the model performs on the training dataset
print(f"Score: ", score)

cv_score = cross_val_score(lsvc, X_train, y_train, cv=10) # slit the data into 10 folds, train model 10 times, 10 accuracy score print
print("CV Score: ", cv_score)

######### Predicting and Accuracy Check
ypred = lsvc.predict(X_test)

cm = confusion_matrix(y_test, ypred)
print(cm)

cr = classification_report(y_test, ypred)
print(cr)




############ Iris Dataset classification
print("Iris dataset classification with SVC\n")
iris = load_iris()

x, y = iris.data, iris.target

X_train, X_test, y_train, y_test = train_test_split(x, y, test_size=0.15)

lsvc = LinearSVC(verbose=0)

lsvc.fit(X_train, y_train)
score = lsvc.score(X_train, y_train)
print("Score: ", score)

cv_score = cross_val_score(lsvc, X_train, y_train, cv=10)
print("cross validation scores \n", cv_score)

ypred = lsvc.predict(X_test)

cm = confusion_matrix(ypred, y_test)
print("Confusion matrix \n", cm) 

cr = classification_report(ypred, y_test)
print("Classification report \n", cr)
