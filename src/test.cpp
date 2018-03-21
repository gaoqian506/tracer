

#include <stdio.h>
#include "tracer/tracer.h"



int main(int argc, char** argv) {
    
    Tracer tracer;
    tracer.load("data/obj/triangle.obj");
    tracer.run();
    //tracer.trace("data/obj/triangle.obj");
    return 0;


}


/*
 	printf("hello world!");
	int i = 6;
	int j = 7;
	return i+j;
 */
