#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

//lineprint.c 원하는 텍스트 파일 읽기
struct node{
    int start;
    int length;
};

struct node table[200];//파일의 정보
char buffer[200];//입출력을 위한 버퍼메모리
int readList[200];//출력을 원하는 줄 수 정보들 저장
char temp[4] = "\n";

//입력방식에서 오류
//출력 문자 갯수에서 오류

int main(int argc, char const *argv[]) {
    	//파일 읽기할 준비
    int fd = open(argv[1],O_RDONLY);
    int line = 1, cnt = 0;
    char tmp;
    	//파일의 줄 정보를 미리 저장
    while(read(fd,&tmp,1)>0){
        ++cnt;
        if(tmp== '\n'){
            table[line].start = cnt;
            table[line-1].length = cnt - (1+table[line-1].start);
            ++line;
        }
    }
   	table[line-1].length = cnt - table[line-1].start;
    //입력버퍼로 바이트 단위로 읽음
    char m;
    while(1){
        read(0, buffer, 200);
        m = buffer[0];
        if(m=='q'){break;}
        //buffer인덱스,숫자가 시작되는 인덱스,readList인덱스
        //각 문장을 파싱하기 위한 추가변수 두개
        int idx = 0, intIdx, iter = 0, flag = 0, minus = 0;
        if (buffer[idx] == '*') {
            for (int i = 0; i < line-1; i++) {readList[iter++] = i+1;}
        }
        else{
            while ((tmp = buffer[idx++]) != '\n') {
                if ((tmp >= '0') && (tmp <= '9')) {
                    if (!flag) {
                        intIdx = idx-1;
                        flag = 1;
                    }
                }
                if (tmp == ',') {
                    readList[iter++] = atoi(buffer + intIdx);
                    flag = 0;
                }
                if (tmp == '-') {
                    readList[iter++] = atoi(buffer + intIdx);
                    flag = 0;
                    minus = 1;
                }
            }
            if (minus) {
                int b = atoi(buffer + intIdx);
                int a = readList[iter - 1];
                for (int i = a+1; i <= b ;i++) {
                    readList[iter++] = i;
                }
            }
            else {
                readList[iter++] = atoi(buffer + intIdx);
            }
        }
        //출력 원하는 줄 번호 배열에 저장
        //원하는 위치로 이동한 뒤 문자들 출력
        idx=0;
        while(buffer[idx]!='\0'){buffer[idx++] = '\0';}
        for (int i = 0; i < iter; i++) {
            lseek(fd, (table[readList[i] - 1].start), SEEK_SET);
            read(fd, buffer, (table[readList[i] - 1].length));
            sprintf(temp,"[%d] ",readList[i]);
            write(1, temp, sizeof(temp));
            idx = 0;
            while (temp[idx] != '\0') { temp[idx++] = '\0'; }
            write(1, buffer, (table[readList[i] - 1].length));
            sprintf(temp, "\n");
            write(1, temp, sizeof(temp));
            idx=0;
            while(buffer[idx]!='\0'){buffer[idx++] = '\0';}
        }
    }
    close(fd);
    return 0;
}
