import pandas as pd
import numpy as np
from sklearn.ensemble import RandomForestClassifier
from sklearn.model_selection import KFold
from sklearn.model_selection import cross_val_score
from sklearn.metrics import mean_squared_error
from mlxtend.preprocessing import TransactionEncoder
from mlxtend.frequent_patterns import fpgrowth

while True:

    print('[ Student ID: 1815060 ]')
    print('[ Name: 문정현 ]')
    print('1. Titanic Survivor Predictor')
    print('2. Market Basket Analyzer')
    print('3. Quit')
    number = int(input('원하는 메뉴의 숫자를 입력하세요: '))
    
    if number == 1:
        train1 = pd.read_csv('train.csv')
        test1 = pd.read_csv('test.csv')
        submission1 = pd.read_csv('gender_submission.csv')

        #매개변수 데이터 내 상관없는 속성과 결과값 분리시키기
        y_train = train1['Survived']
        data = pd.concat([train1,test1])
        data.drop(['Survived','Ticket','PassengerId','Cabin'], axis=1,inplace=True)

        #결측값: Nan인'Age'의 경우 같은 이름범주에 속하는 사람들의 나이 중앙값으로 대체
        data['Age'].fillna(0,inplace=True)
        data['Name'] = data['Name'].str.split(', ').str[1].str.split('. ').str[0]

        mapping = {'Mlle': 'Miss', 'Major': 'Mr', 'Col': 'Mr', 'Sir': 'Mr', 'Don': 'Mr', 'Mme': 'Miss',
                  'Jonkheer': 'Mr', 'Lady': 'Mrs', 'Capt': 'Mr', 'Countess': 'Mrs', 'Ms': 'Miss', 'Dona': 'Mrs'}
        data.replace({'Name': mapping}, inplace=True)

        titles = ['Dr', 'Master', 'Miss', 'Mr', 'Mrs', 'Rev']
        medians = []

        #같은 이름 범주를 가진 사람들 중 0이 아닌 값을 가진 사람들의 나이 중앙값 구하기
        for i in range(6):
            medians.append(data.loc[ (data['Name']== titles[i]) & (data['Age']!=0) , 'Age'].median())
            
        for i in range(6):
            data.loc[ (data['Name'] == titles[i]) & (data['Age']== 0) , 'Age'] =  medians[i]
            
        data.drop(['Name'], axis=1,inplace=True)

        #결측값: Nan인 값들 대체(Age, Fare, Embarked를 중앙값,최빈값으로 대체)
        data['Fare'].fillna(data['Fare'].median(),inplace=True)
        data['Embarked'].fillna('S',inplace=True)

        #문자인 속성들 숫자화(Sex의 경우 값이 2개이므로 replace 사용)
        data['Sex'].replace(['male','female'],[0,1],inplace=True)

        #문자인 속성들 숫자화(Embarked의 경우 값이 3개 이므로 one-hot encoding 사용)

        data['Embarked_S'] = 0
        data['Embarked_C'] = 0
        data['Embarked_Q'] = 0
        Embarked_list = ['S','C','Q']
        for embarked in Embarked_list:
            data['Embarked_'+f'{embarked}'] = 1*(data['Embarked'] == embarked)
        data.drop(['Embarked'], axis=1,inplace=True)

        #카테고리인 범주화 인코딩(Age, Fare)
        data.loc[ data['Age'] <=16, 'Age']= 0
        data.loc[(data['Age'] > 16) & (data['Age'] <=26), 'Age'] = 1
        data.loc[(data['Age'] > 26) & (data['Age'] <=36), 'Age'] = 2
        data.loc[(data['Age'] > 36) & (data['Age'] <=62), 'Age'] = 3
        data.loc[(data['Age'] > 62), 'Age'] = 4

        data.loc[ data['Fare'] <=17, 'Fare'] = 0
        data.loc[(data['Fare'] > 17) & (data['Fare'] <=30), 'Fare'] = 1
        data.loc[(data['Fare'] > 30) & (data['Fare'] <=100), 'Fare'] = 2
        data.loc[ data['Fare'] > 100, 'Fare'] = 3

        data[['Age', 'Fare']] = data[['Age', 'Fare']].astype(int)

        #Family의 크기에 따른 카테고리 특성 생성
        data['FamilySize'] = data['SibSp']+data['Parch']+1
        data.drop(['SibSp','Parch'], axis=1,inplace=True)


        data.loc[ data['FamilySize'] == 1, 'FamilySize'] = 0
        data.loc[ (data['FamilySize'] <= 4) & (data['FamilySize'] > 1), 'FamilySize'] = 1
        data.loc[ data['FamilySize'] >= 5, 'FamilySize'] = 2

        X_train = data[:len(train1)]
        X_test = data[len(train1):]

        #랜덤포레스트에 선택한 하이퍼파라미터를 적용한 후 test에 대한 결과 예측하기
        best_num=150
        best_depth=5
        clf = RandomForestClassifier(n_estimators=best_num,max_depth=best_depth,random_state=0)
        clf.fit(X_train,y_train)
        y_pred = clf.predict(X_test)

        #결과값을 csv로 저장한 후 출력
        submission1["Survived"] = list(map(int,y_pred))
        submission1.to_csv('submission.csv',index = False)

    elif number == 2:
        value = float(input('Enter the minimum support:'))
        file = pd.read_csv('Market_Basket_Optimisation.csv',header = None)
                
        #list comprehension을 사용하여 transaction 리스트에서 nan은 제외한 리스트들의 집합으로 만들기
        train = []
        for i in range(7501):
            train.append([ x for x in file.loc[i].tolist() if pd.isna(x) == False ])

        te = TransactionEncoder()
        te_ary = te.fit(train).transform(train)
        df = pd.DataFrame(te_ary, columns=te.columns_)

        result = fpgrowth(df, min_support=value,use_colnames=True)
        
        for i in range(result.shape[0]):
            print(list(result.loc[i,'itemsets']),' (support:',str(result.loc[i,'support']),')')
        
    elif number == 3:
        break
    
    else:
        print('화면에 있는 메뉴의 숫자로 입력하세요')
