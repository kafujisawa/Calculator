/*
 * base.c
 *
 *  Created on: 2017/07/04
 *      Author: fujisawa
 */
#include "base.h"
int main(void)
{
	char str[256] = {0};
	fgets(str, 256, stdin);
	strtok(str,"\r\n");

	//TODO: �v�Z����

	printf("�v�Z���ʂ�%s�ł�", str);
	return 0;
}
