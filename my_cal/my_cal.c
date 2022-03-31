extern void _my_putchar(char ch);
extern void _my_exit();
extern char _my_getchar();

void my_cal(){
	unsigned int num=0;
	while(1){
		char c = _my_getchar();
		if(c == '\n') break;
		if(c<'0'||c>'9') continue;
		num = num*10+c-'0';
	}
	char ls[100];
	int i=0;
	while(num){
		if(num%2) ls[i++]='1';
		else ls[i++]='0';
		num/=2;
	}
	i--;
	while(ls[i]=='0')i--;
	for(;i>=0;i--) _my_putchar(ls[i]);
//	_my_putchar('\n');

	/*
	char c = _my_getchar();
	_my_putchar(c);
	_my_exit();
	*/
}
