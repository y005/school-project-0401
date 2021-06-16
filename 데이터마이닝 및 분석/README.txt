1. 사용환경 설명
파이썬3의 가상환경을 생성하여 사용해야 합니다.

2-1. titanic 
HW#1에 해당하는 데이터마이닝 과제로 타이타닉 사건의 생존자를 예측하는 모델을
설계하는 과제입니다. (스코어는 0.76555보다 커야합니다.)

2-2. market basket
HW#2에 해당하는 데이터마이닝 과제로 transaction 내 빈발 패턴 조합들을 출력해야 
하는 과제입니다.

3. HW
2의 과제를 모두 수행하는 메뉴 인터페이스가 있는 파이썬 실행물입니다.

##로컬 컴퓨터 내 가상환경에서 하기##
>> python -m venv 가상환경 폴더명
- pyvenv.txt 내 파이썬 연결 정보 수정
  home = 실행할 파이썬 위치
  version = 실행할 파이썬 버전

>> pip install -r requirements.txt
>> (가상환경폴더/Scripts) activate.bat(Window)
>> (가상환경폴더/Scripts) source activate(linux)
- Jupyter Notebook: 주피터 노트북 내 kernel 커널명 선택하여 가상환경과 연결
>>python -m ipykernel install --user --name 가상환경이름 --display-name 커널명
- IDLE: 가상환경 실행 후 IDLE 실행
>>pythonw.exe -m idlelib

##kaggle api를 활용한 Dataset download와 submit 하기
>> pip install kaggle
>> 어카운트 내 kaggle api token 받은 후 
>> 사용자/.kaggle 폴더에 배치
>> kaggle competitions download -c titanic
>> kaggle competitions submit -c titanic -f submission.csv -m "Message"
