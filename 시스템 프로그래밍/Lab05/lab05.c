#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#define MAXARG 7

int main(){

    char buf[256];
	  char buf1[256];
    char *args[MAXARG];
    char *args1[MAXARG];
    char *s;
    char *save;
    char * tmp;
    int argn,argn1,i,len,pid, status,wait,io=-1,fd;

    static const char delim[] = " \t\n"; /* 공백, 탭, 개행으로 이루어진 구분자 선언 */

    while(1) {  /* 무한 반복 */
	     wait = 1;
       printf("[shell] ");/* 프롬프트 출력 */
       gets(buf);
       argn = 0;
       s = strtok_r(buf, delim, &save);/* 문자열에서 delim을 기준으로 단어를 잘라냄 */

       while(s) {
          args[argn++] = s;
          s = strtok_r(NULL, delim, &save);
       }
       args[argn] = (char *)0;/* 인수가 더 없음을 의미하는 문자 추가 */
       if (argn == 0){continue;} /* 명령어가 없으면 루프 계속 */
       if (!strcmp(args[0], "quit")){break;} /* 프롬프트로 입력받은 단어가‘quit’이면 while 문 벗어남 */
       else {                     /* 자식 프로세스는 execvp를 이용하여 arg[0] 실행 */
         //;로 끝나는 단위로 execvp실행해야됨
         i = 0;
         argn1 = 0;
         while(i<argn){
             //단어 단위로 검사
             tmp = args[i];
             //단어 끝이 ;로 끝나는지 확인
             if(!strcmp(tmp,"<")){io = 0;}
             if(!strcmp(tmp,">")){io = 1;}
             if(!strcmp(tmp,"&")){wait = 0;}
			       len = -1;
             while (*(tmp+(++len)));
             if(tmp[len-1]==';'){
                //;를 뺸 단어를 명령인수 배열에 추가
                strcpy(buf1,args[i]);
                buf1[len-1] = '\0';
                args1[argn1++] = buf1;
				        args1[argn1] = (char *)0;

				       //fork를 하여 진행할 프로세스들 생성
                if ((pid=fork()) == -1) {perror("fork failed");}
                //execvp에 명령어, 명령어 인수배열 전달하여 프로세스 실행
                if(pid == 0){execvp(args1[0],args1);}
                else{
                  //프로세스 끝날 때까지 기다리기
                  pid = waitpid(pid, &status, 0);
                  //args1,argn1 초기화
                  memset(args1,0,sizeof(args1));
                  argn1 = 0;
                }
             }
             //단어 끝이 ;로 안 끝나는 경우
             else{args1[argn1++] = args[i];}
             i++;
         }
         //남은 명령어 실행하기
		     args1[argn1] = (char *)0;
         if ((pid=fork()) == -1) {perror("fork failed");}
         //execvp에 명령어, 명령어 인수배열 전달하여 프로세스 실행
         if(pid == 0){
           if(io!=-1){
			        fd = open(args1[argn1-1],O_CREAT|O_RDWR,0666);
			        args1[argn1-1] = (char *)0;
              args1[argn1-2] = (char *)0;
              dup2(fd,io);
              close(fd);
              execvp(args1[0],args1);
           }
           else{
	           if(!wait){
			  		     args1[argn1-1] = (char *)0;
			        }
			        execvp(args1[0],args1);
		      	}
         }
         else{
			       if(wait){pid = waitpid(pid, &status, 0);}
	       }
		     io = -1;
       }
    }
    exit(0);
}
