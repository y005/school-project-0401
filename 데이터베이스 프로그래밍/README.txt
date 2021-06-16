4-1 데이터베이스 프로그래밍 프로젝트 관련

##JSP서버 환경설정##
Oracle 11g express edition
Tomcat9.0(JDK와 연동시키기)
Java SDK 12.0.2(64bit)
eclipse 사용(file>new>Dynamice web project)
프로젝트 라이브러리 내 ojdbc.jar 넣기
상세한 설정은 JSP 환경설정.pdf에 

##JSP 실행하면서 해결한 문제##

404문제(서버프로그램 위치 재설정)
server properties> switch location
server location>use Tomcat installation

데이터베이스 연결 x
이벤트>~listner 실행

이미 사용 중인 포트인 경우
netstat -a -o
taskkill /f /pid PIDNUM
