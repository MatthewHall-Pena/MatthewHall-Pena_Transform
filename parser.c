#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "ml6.h"
#include "display.h"
#include "draw.h"
#include "matrix.h"
#include "parser.h"


/*======== void parse_file () ==========
Inputs:   char * filename 
          struct matrix * transform, 
          struct matrix * pm,
          screen s
Returns: 

Goes through the file named filename and performs all of the actions listed in that file.
The file follows the following format:
     Every command is a single character that takes up a line
     Any command that requires arguments must have those arguments in the second line.
     The commands are as follows:
         line: add a line to the edge matrix - 
	    takes 6 arguemnts (x0, y0, z0, x1, y1, z1)
	 ident: set the transform matrix to the identity matrix - 
	 scale: create a scale matrix, 
	    then multiply the transform matrix by the scale matrix - 
	    takes 3 arguments (sx, sy, sz)
	 translate: create a translation matrix, 
	    then multiply the transform matrix by the translation matrix - 
	    takes 3 arguments (tx, ty, tz)
	 rotate: create an rotation matrix,
	    then multiply the transform matrix by the rotation matrix -
	    takes 2 arguments (axis, theta) axis should be x y or z
	 apply: apply the current transformation matrix to the 
	    edge matrix
	 display: draw the lines of the edge matrix to the screen
	    display the screen
	 save: draw the lines of the edge matrix to the screen
	    save the screen to a file -
	    takes 1 argument (file name)
	 quit: end parsing

See the file script for an example of the file format


IMPORTANT MATH NOTE:
the trig functions int math.h use radian mesure, but us normal
humans use degrees, so the file will contain degrees for rotations,
be sure to conver those degrees to radians (M_PI is the constant
for PI)
====================*/
void parse_file ( char * filename, 
                  struct matrix * transform, 
                  struct matrix * edges,
                  screen s) {

  FILE *f;
  char line[256];
  clear_screen(s);
color c;
c.red=255;
c.blue=255;
c.green=255;	
  if ( strcmp(filename, "stdin") == 0 ) 
    f = stdin;
  else
    f = fopen(filename, "r");
	
	char name[50];
	int a,b,h,d,e,g;
	char axis;
	
	
  while ( fgets(line, 255, f) != NULL ) {
    line[strlen(line)-1]='\0';
    printf(":%s:\n",line);
	
	if(strcmp(line, "line")==0){
		fgets(line, 255, f);
		line[strlen(line)-1]='\0';
		sscanf( line, "%d %d %d  %d %d %d", &a, &b, &h, &d, &e, &g);
		add_edge(edges, a, b, h, d, e, g);
	}
	if( strcmp(line,"ident") == 0){
      ident(transform);
    }
	if(strcmp(line, "translate")==0 ||strcmp(line, "move")==0  ){
		fgets(line, 255, f);
		line[strlen(line)-1]='\0';
		sscanf( line, "%d %d %d", &a, &b, &h);
	struct matrix * m;
      m = new_matrix(4,4);
      m = make_translate(a, b, h);
      matrix_mult(m, transform);
	}
	if( strcmp(line,"scale") == 0){
		fgets(line, 255, f);
		line[strlen(line)-1]='\0';
		sscanf( line, "%d %d %d ",  &a, &b, &h);
		 struct matrix * m;
      m = new_matrix(4,4);
      m = make_scale(a, b, h);
      matrix_mult(m, transform); 
	}
	if( strcmp(line,"rotate") == 0){
      fgets(line, 255, f);
      line[strlen(line)-1]='\0';
      
	sscanf( line, "%c %d", &axis, &a);
      
	  struct matrix * m;
      m = new_matrix(4,4);
      
      if(axis == 'z'){
		m = make_rotZ(a);
      }
       if(axis == 'y'){
		m = make_rotY(a);
      }
       if(axis == 'x'){
		m = make_rotX(a);
      }
      
      matrix_mult(m, transform); 
    }
	 if( strcmp(line,"apply") == 0){
      matrix_mult(transform, edges); 
    }
	 if( strcmp(line,"display") == 0){
      clear_screen(s);
      draw_lines(edges, s, c);
      display(s);
    }
	if(strcmp(line, "save")==0){
		fgets(line, 255, f);
		line[strlen(line)-1]='\0';
		sscanf( line, "%s",name);
		save_extension(s, name);
  }
  if( strcmp(line,"quit") == 0){
      break;
    }
  }
  return;
}
  
