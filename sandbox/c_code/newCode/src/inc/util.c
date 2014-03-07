/**
 *	Project Name:     FreFrobo c utilities
 *	File name:        util.c
 *	Creation date:    Fri Mar 7 16:29:03 CET 2014
 *	Author:           Daniel Lindekilde Ravn
 *	SDU email:        darav08@student.sdu.dk
 *	gmail:            daniel.ravn43@gmail.com
**/

#include "util.h"
#include "stdio.h"


char* intToCharArray(INT8U aInt)
{
	char str[4];
	sprintf(str, "%d", aInt);
//	str[3] = '\0';
	return str;
}
