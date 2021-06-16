#include<stdio.h>

FILE* fp;
struct account{
    int id;
    char name[100];
    int cash;
};

void makeAccount(){
    struct account tmp;
    struct account find;
    printf("\n계좌 생성 메뉴 - 계좌번호, 이름, 초기 입금액을 띄어쓰기로 구분하여 입력해주시기 바랍니다.\n");
    fscanf(stdin,"%d %s %d",&tmp.id,tmp.name,&tmp.cash);
    fseek(fp,(tmp.id-202100001)*sizeof(struct account),SEEK_SET);
    fread(&find,sizeof(struct account),1,fp);
    if(find.id==tmp.id){
        printf("이미 존재하는 계좌번호입니다.\n");
        return;
    }
    else{
        fseek(fp,-sizeof(struct account),SEEK_CUR);
        fwrite(&tmp,sizeof(struct account),1,fp);
    }
}
void deposit(){
    struct account tmp;
    int id,money;
    printf("\n입금 메뉴\n계좌번호를 입력하세요:");
    scanf("%d",&id);
    fseek(fp,(id-202100001)*sizeof(struct account),SEEK_SET);
    fread(&tmp,sizeof(struct account),1,fp);
    if(tmp.id!=id){
        printf("NO RECORD!\n");
        return;
    }
    printf("\n입금할 금액을 입력하세요:");
    scanf("%d",&money);
    tmp.cash += money;
    fseek(fp,-sizeof(struct account),SEEK_CUR);
    fwrite(&tmp,sizeof(struct account),1,fp);
    printf("\n");
}
void withdraw(){
    struct account tmp;
    int id,money;
    printf("\n출금 메뉴\n계좌번호를 입력하세요:");
    scanf("%d",&id);
    fseek(fp,(id-202100001)*sizeof(struct account),SEEK_SET);
    fread(&tmp,sizeof(struct account),1,fp);
    if(tmp.id!=id){
        printf("NO RECORD!\n");
        return;
    }
    printf("\n출금할 금액을 입력하세요:");
    scanf("%d",&money);
	if(tmp.cash<money){
		printf("\nnot enough money\n");
		return;
	}
    tmp.cash -= money;
    fseek(fp,-sizeof(struct account),SEEK_CUR);
	fwrite(&tmp,sizeof(struct account),1,fp);
    printf("\n");
}
void info(){
	struct account tmp;
    int id;
    printf("\n잔액조회 메뉴\n계좌번호를 입력하세요:");
    scanf("%d",&id);
    fseek(fp,(id-202100001)*sizeof(struct account),SEEK_SET);
    fread(&tmp,sizeof(struct account),1,fp);
    if(tmp.id!=id){
        printf("NO RECORD!\n");
        return;
    }
    printf("\n*계좌번호: %d\n",tmp.id);
    printf("*이름: %s\n",tmp.name);
    printf("*잔액: %d\n",tmp.cash);
}

int main(int argc,char* argv[]){
    int sel;
    if(argc<2){return -1;}
    if ((fp = fopen(argv[1], "ab+")) == NULL ) {
        fprintf(stderr, "파일 열기 오류\n");
        return;
    }
	fclose(fp);
    while(1){
        printf("----Menu----\n");
        printf("1. 계좌생성\n");
        printf("2. 입금\n");
        printf("3. 출금\n");
        printf("4. 잔액조회\n");
        printf("5. 프로그램 종료\n");
        printf("------------\n");
        printf("원하는 메뉴를 입력하시오:");
        scanf("%d",&sel);
        switch(sel){
            case 1:
    			if ((fp = fopen(argv[1], "ab+")) == NULL ) {
        			fprintf(stderr, "파일 열기 오류\n");
        			return;
    			}
				makeAccount();
				fclose(fp);
                break;
            case 2:
                if ((fp = fopen(argv[1], "rb+")) == NULL ) {
        		      fprintf(stderr, "파일 열기 오류\n");
        			  return;
    			}
				deposit();
				fclose(fp);
				break;
            case 3:
				if ((fp = fopen(argv[1], "rb+")) == NULL ) {
        		    fprintf(stderr, "파일 열기 오류\n");
        	        return;
    			}
				withdraw();
		        fclose(fp);
                break;
            case 4:
				 if ((fp = fopen(argv[1], "rb")) == NULL ) {
        		     fprintf(stderr, "파일 열기 오류\n");
        	         return;
    			 }
				 info();
				 fclose(fp);
                 break;
            case 5:
                printf("프로그램이 종료되었습니다.\n");
                return;
            default:
                getchar();
                printf("올바른 숫자를 입력하세요.\n");
        }
    }
}
