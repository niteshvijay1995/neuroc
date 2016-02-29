/*
[A-Z] -> 0
[a U e-z] -> 1
[b-d] -> 2
[0-9] -> 3
< -> 4
> -> 5
% -> 7
# -> 8 
_ -> 9
[ -> 10
] -> 11
; -> 12
: -> 13
. -> 14
( -> 15
) -> 16
+ -> 17
- -> 18
* -> 19
/ -> 20
@ -> 21
& -> 22
~ -> 23
= -> 24
! -> 25

*/

int char2index(char ch)
{
	//printf("=%c=",ch);
	if(ch>='b' && ch<='d')
	{
		return 2;
	}
	else if('A'<=ch && ch<='Z')
		return 0;
	else if('0'<=ch && ch<='9')
		return 3;
	else if('a'<=ch && ch<='z')
		return 1;
	else
	{
		switch(ch)
		{
			case '<':
				return 4;
			case '>':
				return 5;
			case '%':
				return 7;
			case '#':
				return 8;
			case '_':
				return 9;
			case '[':
				return 10;
			case ']':
				return 11;
			case ';':
				return 12;
			case ':':
				return 13;
			case '.':
				return 14;
			case '(':
				return 15;
			case ')':
				return 16;
			case '+':
				return 17;
			case '-':
				return 18;
			case '*':
				return 19;
			case '/':
				return 20;
			case '@':
				return 21;
			case '&':
				return 22;
			case '~':
				return 23;
			case '=':
				return 24;
			case '!':
				return 25;

		}
	}

}
