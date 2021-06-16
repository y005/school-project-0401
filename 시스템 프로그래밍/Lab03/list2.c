#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
//가능한 옵션은 -daisF
//-d: 디렉터리 정보만 출력
//-a: 숨김파일을 포함하여 디렉터리내의 모든 항목 표시
//-i: 파일의 i노드번호 출력
//-s: 파일 크기 출력
//-F: 파일 유형 출력

//d:0/a:1/i:2/s:3/F:4
int option[5];
int idx;
char path[100];

int main(int argc,char** argv){
    char* dir = NULL;
    DIR *dp;
    struct dirent *d;
    struct stat st;
	int i,j; 
    if(argc<2){dir = ".";}
    else{
        for(i=1;i<argc;i++){
            if(argv[i][0]=='-'){
                for(j=1;argv[i][j]!='\0';j++){
                    switch(argv[i][j]){
                        case 'd':
                            option[0] = 1;
                            option[1] = 1;
                            break;
                        case 'a': option[1] = 1; break;
                        case 'i': option[2] = 1; break;
                        case 's': option[3] = 1; break;
                        case 'F': option[4] = 1; break;
                    }
                }
            }
            else{dir = argv[i];}
        }
        if(dir==NULL){dir=".";}
    }
    if((dp = opendir(dir)) == NULL){perror(dir);}
    while((d = readdir(dp)) != NULL){
        sprintf(path, "%s/%s", dir, d->d_name);
        if(stat(path, &st) < 0){perror(path);}
        //.와..의 정보를 출력
        if((d->d_name[0]=='.')&&(!option[1])){continue;}
        //i 노드번호 숫자출력
        if(option[2]){printf("%10llu ",(unsigned long long)d->d_ino);}
        //파일 크기 출력
        if(option[3]){printf("%8d ",st.st_size);}
        //파일 이름 출력
        if(option[0]){printf("%s",dir);}
        if(!option[0]){printf("%s",d->d_name);}
      //파일 유형  출력
        if(option[4]){
            if (S_ISDIR(st.st_mode)){printf("/");}
            else if (S_ISFIFO(st.st_mode)){printf("|");}
            else if (S_ISLNK(st.st_mode)){printf("@");}
            //실행권한 존재시 실행파일임
            else if((st.st_mode&S_IXUSR)||(st.st_mode&S_IXGRP)||(st.st_mode&S_IXOTH)){printf("*");}
            else if(S_ISSOCK(st.st_mode)){printf("=");}
        }
		printf("\n");
		if(option[0]){break;}
    }
    closedir(dp);
    exit(0);
}
