# Compiler C-Simple Language

### linux C project

I have implemented a compiler project, which accepts a text file in C simple and checks the integrity of the input in terms of syntax and semantics, and only then converts the code into three adreess code.

To run the project you must first install the yacc and the lex on your Linux computer (I ran my Linux on virtual box) by the following command:

```
$ sudo apt-get install byacc flex
```

Then run the following commands, in the directory in which the files are located:

```
$ lex scanner.l 
$ yacc -d parser.y
$ cc lex.yy.c y.tab.c -o parser.exe 
```

And now we have the project executable file, in order to run the project, we need a file in the c language, for example the following file:

```
int foo()
{
	int a;
	a = 23;
	if (a == 23 && a != 100)
		a = 10;
	else
		a = 19;
	return a;
}


int fee(int a, int b)
{
	int sum1, sum2;
	sum1 = 0;
	sum2 = 0;
	while (a < b)
	{
		if (a / 2 == 0 || a*b < 100 && sum1 == sum2)
			sum1 = sum1 + a;
		else
			sum2 = sum2 + 1;
		a = a + 1;
	}
	return sum1 + sum2;
}


void main()
{
	int a;
	int b;
	int c;
	a = 0;
	
	while(a < 10)
        {
		b = foo();
		a = a + b;
	}
}
```

Command Prompt:
```
$ ./parse.exe < 3AK.txt
```

And we get the next output, as in the picture: 

![](https://profile.fcdn.co.il/images/0__05acc11148773a.jpg)

