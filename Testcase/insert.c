
#include <stdio.h>
#include <stdlib.h>

#include "insert.h"

void insert(List x, int d)
{
	List y, t, e;
	y = x;
	while (y->n != NULL) {
		y = y->n;
	}
	t = (List)malloc(sizeof(List));
	t->data = d;
	e = y->n;
	t->n = e;
	y->n = t;
}