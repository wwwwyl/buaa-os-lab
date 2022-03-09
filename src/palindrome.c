#include<stdio.h>
int main()
{
	int n;
	scanf("%d",&n);
	int sum=0,tmp=n;
	while(tmp){
		sum = sum*10+tmp%10;
		tmp /= 10;
	}
	if(sum==n){
		printf("Y");
	}else{
		printf("N");
	}
	return 0;
}
