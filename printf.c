#include "printf.h"
#include "serial.h"

static const char HexCharsCap[] = "0123456789ABCDEF";
#define hexcharCap(x)	HexCharsCap[(x)&0x0F]

static const char HexCharsNoCap[] = "0123456789abcdef";
#define hexcharNoCap(x)	HexCharsNoCap[(x)&0x0F]

static int print(int *varg);

static void printc(char C);
static void printi(char base, char isSigned, char cap, long n);
static void prints(char *string);

int printf (const char *format, ...)
{ 
  int *varg = (int *)(&format);

  return print(varg);
}

static void printc(char C)
{
	putchar_serial0(C);
	
	return;
}

static void printi(char base, char isSigned, char cap, long n)
{
	char *p, buf[32];
	unsigned long x;
	unsigned char count = 1;
	
	if (isSigned && (n < 0))
	{
		x = -n;
	}
	else
	{
		x = n;
	}

  	p = buf + 32;
  	*--p = '\0';	
	
	if (cap == 1)
	{
		*--p = hexcharCap(x%base); 
		x /= base;
	}				
	else
	{
		*--p = hexcharNoCap(x%base);
		x /= base;
	}
	

	while(x != 0)
	{
		if (cap == 1)
		{
			*--p = hexcharCap(x%base); 
			x /= base;
		}				
		else
		{
			*--p = hexcharNoCap(x%base);
			x /= base;
		}
		count++;
	}

	
		
	
	if( isSigned )
	{
		if(n < 0)
		{
   			*--p = '-';
   			count++;   			
		}
		
	}
	

	while(count--)
	{
		printc(*p++);
	}	
	
	return;
}

static void prints(char *string)
{
	while(*string != '\0')
	{
		printc(*string++);
	}
	return;
}

static int print(int *varg)
{
	char *format = (char *) (*varg++);
	
	for (; *format != 0; ++format)
	{
		if (*format ==  '%')
		{
			++format;
			
			if (*format == 'c')
			{
				printc(*varg++);
			}
			
			if (*format == 's')
			{
				prints((char *)*varg++);
			}
			
			if ((*format == 'd') || (*format == 'i'))
			{
				printi(10, 1, 0, *varg++);
			}
			
			if (*format == 'u')
			{
				printi(10, 0, 0, *varg++);
			}
			
			if (*format == 'X')
			{
				printi(16, 0, 1, *varg++);
			}
			
			if (*format == 'x')
			{
				printi(16, 0, 0, *varg++);
			}
			
			if (*format == 'o')
			{
				printi(8, 0, 0, *varg++);
			}
			format++;
			
		}
		if (*format != '\0') printc(*format);
	}
	
	putstring_serial0(format);
	
	return 0;
}


