/*
January 2012
Charitos Georgios
Nikou Dimitrios
*/

#include <iostream>
#include <cstdio>
#include <math.h>
#include <list>
#include <time.h>

#ifdef _WIN32 || WIN32
  #include <windows.h>
  #pragma comment(lib, "winmm.lib")
#endif

#pragma warning(push, 0)
  #include <GL/gl.h>
  #include <GL/glu.h>
  #include <GL/glut.h>
#pragma warning(pop)

using namespace std;

#define N 15 // grid size

#define LEFT_COLUMN 1
#define MIDDLE_COLUMN 2
#define RIGHT_COLUMN 3 // volume limits inside grid
#define BACK_COLUMN 4
#define KENTRO_COLUMN 5
#define FRONT_COLUMN 6
#define DOWN_LEVEL 7
#define MIDDLE_LEVEL 8
#define UP_LEVEL 9

#define YELLOW 1
#define BLUE 2
#define RED 3
#define GREEN 4	// color used by easter egg only!!
#define NOCOLOR -1

float K = N/2.0f;

float zoomx=1.0f,zoomz=1.0f,zoomy=1.0f; // useful for zoom in - zoom out

float x=-K-5.0f,y=K+8.0f,z=K+10.0f; // default position of the camera
float camera2_y = K+20.0f;
float camera3_z = N+12.0f;

float rotation = 0;
float rotation2 = 0;  // useful for camera rotation
float rotation3 = 0;
float rotation_speed = 2.0f;

int camera_count=0; // useful for switching between cameras

/*variables for shapes' creation*/

// array that stores the peak coordinates of volumes
float syn[27][3];
float tempsyn[27][3];

// array that stores neighbor cubes of a volume
int geitones[27][6] = {{2,6,10,-1,-1,-1}, //1
			 {1,3,5,11,-1,-1}, //2
			 {2,4,12,-1,-1,-1}, //3
			 {3,5,9,13,-1,-1}, //4
			 {2,4,6,8,14,-1}, //5
			 {1,5,7,15,-1,-1}, //6
			 {6,8,16,-1,-1,-1}, //7
			 {5,7,9,17,-1,-1}, //8
			 {4,8,18,-1,-1,-1}, //9
			 {11,15,1,19,-1,-1}, //10
			 {10,12,14,2,20,-1}, //11
			 {11,13,3,21,-1,-1}, //12
			 {12,14,18,4,22,-1}, //13
			 {11,13,15,17,23,5}, //14
			 {10,16,14,24,6,-1}, //15
			 {15,17,25,7,-1,-1}, //16
			 {14,16,18,26,8,-1}, //17
			 {13,17,27,9,-1,-1}, //18
			 {20,24,10,-1,-1,-1}, //19
			 {19,21,23,11,-1,-1}, //20
			 {20,22,12,-1,-1,-1}, //21
			 {27,21,23,13,-1,-1}, //22
			 {20,22,24,26,14,-1}, //23
			 {19,23,25,15,-1,-1}, //24
			 {24,26,16,-1,-1,-1}, //25
			 {25,27,23,17,-1,-1}, //26
			 {22,26,18,-1,-1,-1} //27
			};

// number of cubes per volume
int cube_num;
int chosen_cube;

// volume initial drop coordinates
// y always on top of the grid, x & z picked randomly
float syn_x, syn_z, syn_y=-K+1;

int label_counter=-1;

int black[1000][27];

void init_black(int row){
	int i;
	for(i=0;i<27;i++){
		black[row][i] = -1;
	}
}

void pushtoblack(int row,int ch_cube){
	int k=0;
	int tr=0;
	while(k<27 && tr==0){
		if(black[row][k]==-1){
			black[row][k] = ch_cube;
			tr=1;
		}
		k++;
	}
}

class Cube{
	private: float x;
		 float y;
		 float z;
		 int color;

	public: float retX();
		float retY();
		float retZ();
		void puttocube(float xp,float yp, float zp,int color);
		int retcolor();
};

float Cube :: retX(){
	return x;
}

float Cube :: retY(){
	return y;
}

float Cube :: retZ(){
	return z;
}

int Cube :: retcolor(){
	return color;
}

void Cube:: puttocube(float xp, float yp, float zp, int clr){
		x=xp;
		y=yp;
		z=zp;
		color=clr;
}

class Shape{
	private: float x;
		 float y;
		 float z;
		 int label;
		 int color;
	public: void change(float a,float b,float c);
		float retX();
		float retY();
		float retZ();
		int retlabel();
		void putlabel();
		void putcolor(int clr);
		int retcolor();
};

void Shape :: putcolor(int clr){
	color = clr;
}

int Shape :: retcolor(){
	return color;
}

int Shape :: retlabel(){
	return label;
}

void Shape :: putlabel(){
	label = label_counter;
}

void Shape :: change(float a,float b,float c){
	if(a!=-1)
		x = a;
	if(b!=-1)
		y = b;
	if(c!=-1)
		z = c;
}

float Shape :: retX(){
	return x;
}

float Shape :: retY(){
	return y;
}

float Shape :: retZ(){
	return z;
}

/*global item color*/
int temp_color=-1;

list <Shape> shapelist;
list <Shape>::iterator iter;
list <Cube> cubelist;
list <Cube>::iterator iter2;

// insert cubes to the world
void puttocosmos(){
	int i,c=label_counter;
	float temp1,temp2,temp3;
	int q=0;

	Cube A[27];

	for(i=0;i<27;i++){
		if(black[c][i]!= -1){
			temp1 = syn[black[c][i]-1][0];
			temp2 = syn[black[c][i]-1][1];
			temp3 = syn[black[c][i]-1][2];

			A[q].puttocube(temp1,temp2,temp3,temp_color);
			
			cubelist.push_back(A[q]);
		
			q++;
		}
	}

}

bool reached=false;

// coordinates of the cubes of a volume
void fix_syn_table(float syn_xx,float syn_yy,float syn_zz){
	int i,j,counter=0,c=0;

		while(counter!=3){
			for(i=(0+c);i<(3+c);i++){
				syn[i][0] = syn_xx;
				syn[i][1] = syn_yy;
				syn[i][2] = syn_zz;
			
				syn_xx++;
			}
		
			syn_xx--;
			syn_yy++;
			for(i=(3+c);i<(6+c);i++){
				syn[i][0] = syn_xx;
				syn[i][1] = syn_yy;
				syn[i][2] = syn_zz;
			
				syn_xx--;
			}

			syn_xx++;
			syn_yy++;
			for(i=(6+c);i<(9+c);i++){
				syn[i][0] = syn_xx;
				syn[i][1] = syn_yy;
				syn[i][2] = syn_zz;
			
				syn_xx++;
			}

			syn_xx--;

			syn_xx = syn_xx-2;
			syn_yy = syn_yy-2;
			syn_zz--;
		
			c = c+9;
			counter++;
		}
}

int score=0;
char s1[32];

void breakcube(float xxx,float yyy,float zzz){
	list <Cube>::iterator tempiter;

	for(tempiter=cubelist.begin();tempiter!=cubelist.end();tempiter++){
		if( (tempiter->retX()==xxx) && (tempiter->retY()==yyy) && (tempiter->retZ()==zzz) ){
			tempiter = cubelist.erase(tempiter);
		}
	}
}

bool game_over=false;

void ifgameover(){
	list <Cube>::iterator tempiter;

	for(tempiter=cubelist.begin();tempiter!=cubelist.end();tempiter++){
		if(tempiter->retY() == K)
			game_over=true;
	}
}

void init( void )
{
	//Set background color (ousiastika svhnei ta panta me afto to xrwma)
	glClearColor(0.60f, 0.60f, 0.60f, 1.0f);
	
	// select projection matrix
	glMatrixMode(GL_PROJECTION);
	
	// set matrix mode
	glMatrixMode(GL_PROJECTION);

	// reset projection matrix
	glLoadIdentity();
	//GLfloat aspect = (GLfloat) win.width / win.height;

	// specify which matrix is the current matrix
	glMatrixMode(GL_MODELVIEW);
	glShadeModel( GL_SMOOTH );
	
	// specify the clear value for the depth buffer (initial value is 1.0)
	glClearDepth( 1.0f );

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glEnable( GL_DEPTH_TEST );

	// Lighting set up
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
}

void check_aroundX(int choice){
	int c=label_counter;
	float x1,y1,z1;
	int areyou=0;

	if(choice==0){
		list <Cube>::iterator tempiter;
		int i;
			for(i=0; i<27; i++){
				tempsyn[i][0]=syn[i][0]-1;
				tempsyn[i][1]=syn[i][1];
				tempsyn[i][2]=syn[i][2];
			}

			for(i=0;i<27;i++){
				if(black[c][i]!= -1){
					x1 = tempsyn[black[c][i]-1][0];
					y1 = tempsyn[black[c][i]-1][1];
					z1 = tempsyn[black[c][i]-1][2];

					for(tempiter=cubelist.begin();tempiter!=cubelist.end();tempiter++){
						if( (x1==(tempiter->retX())) && (y1==(tempiter->retY())) && (z1==(tempiter->retZ())) ){
							areyou=1;
						}
					}  
				}
			}

			if(areyou==0){
				syn_x--;
			}
	}
	else if(choice==1){
		list <Cube>::iterator tempiter;
		int i;
			for(i=0; i<27; i++){
				tempsyn[i][0]=syn[i][0]+1;
				tempsyn[i][1]=syn[i][1];
				tempsyn[i][2]=syn[i][2];
			}

			for(i=0;i<27;i++){
				if(black[c][i]!= -1){
					x1 = tempsyn[black[c][i]-1][0];
					y1 = tempsyn[black[c][i]-1][1];
					z1 = tempsyn[black[c][i]-1][2];

					for(tempiter=cubelist.begin();tempiter!=cubelist.end();tempiter++){
						if( (x1==(tempiter->retX())) && (y1==(tempiter->retY())) && (z1==(tempiter->retZ())) ){
							areyou=1;
						}
					}  
				}
			}

			if(areyou==0){
				syn_x++;
			}
	}
}

void check_aroundY(){
	float x1,y1,z1;
	int i,t1,t2,t3;
	int c = label_counter;
	int areyou=0;
	float tempyy =syn_y-1;
	
	if(temp_color==YELLOW){		
			for(i=0; i<27; i++){
				tempsyn[i][0]=syn[i][0];
				tempsyn[i][1]=syn[i][1]-1;
				tempsyn[i][2]=syn[i][2];
			}
		
			for(i=0;i<27;i++){
				if(black[c][i]!= -1){
					x1 = tempsyn[black[c][i]-1][0];
					y1 = tempsyn[black[c][i]-1][1];
					z1 = tempsyn[black[c][i]-1][2];

					for(iter2=cubelist.begin();iter2!=cubelist.end();iter2++){
						if( (x1==(iter2->retX())) && (y1==(iter2->retY())) && (z1==(iter2->retZ())) ){
							areyou=1;
						}
					}  
				}
			}
			if(areyou==0){
				syn_y--;
			}
			else {
				reached=true;
			}
	}//YELLOW
	else if(temp_color==BLUE){
			int blue_c=0;
			list <Cube> templist;
			list <Cube>::iterator tempiter;

			for(i=0; i<27; i++){
				tempsyn[i][0]=syn[i][0];
				tempsyn[i][1]=syn[i][1]-1;
				tempsyn[i][2]=syn[i][2];
			}
		
			tempiter=templist.begin();
			for(i=0;i<27;i++){
				if(black[c][i]!= -1){
					x1 = tempsyn[black[c][i]-1][0];
					y1 = tempsyn[black[c][i]-1][1];
					z1 = tempsyn[black[c][i]-1][2];

					for(iter2=cubelist.begin();iter2!=cubelist.end();iter2++){
						if( (x1==(iter2->retX())) && (y1==(iter2->retY())) && (z1==(iter2->retZ())) ){
							blue_c++;
							templist.push_back(*iter2);
						}
					}  
				}
			}
	
			if(blue_c>=3){
				areyou=1;
			}
			else{
				areyou=0;
			}


			if(areyou==0){
				syn_y--;
				tempiter=templist.begin();
				while(blue_c>0){
					breakcube(tempiter->retX(),tempiter->retY(),tempiter->retZ());
					blue_c--;
					score++;
					tempiter++;
				}
			}
			else {
				reached=true;
			}
			cout<<"\n";
	}//BLUE
	else if(temp_color==RED){
			int red_c=0;
			list <Cube> templist2;
			list <Cube>::iterator tempiter2;

			for(i=0; i<27; i++){
		
				tempsyn[i][0]=syn[i][0];
				tempsyn[i][1]=syn[i][1]-1;
				tempsyn[i][2]=syn[i][2];
			}
		
			tempiter2=templist2.begin();
			for(i=0;i<27;i++){
				if(black[c][i]!= -1){
					x1 = tempsyn[black[c][i]-1][0];
					y1 = tempsyn[black[c][i]-1][1];
					z1 = tempsyn[black[c][i]-1][2];

					for(iter2=cubelist.begin();iter2!=cubelist.end();iter2++){
						if( (x1==(iter2->retX())) && (y1==(iter2->retY())) && (z1==(iter2->retZ())) ){
							red_c++;
							templist2.push_back(*iter2);
						}
					}  
				}
			}

			tempiter2=templist2.begin();
			while(red_c>0){
				breakcube(tempiter2->retX(),tempiter2->retY(),tempiter2->retZ());
				red_c--;
				tempiter2++;
				score++;
			}

			syn_y--;
	}//RED
}

void check_aroundZ(int choice){
	int c=label_counter;
	float x1,y1,z1;
	int areyou=0;

	if(choice==0){
		list <Cube>::iterator tempiter;
		int i;
			for(i=0; i<27; i++){
				tempsyn[i][0]=syn[i][0];
				tempsyn[i][1]=syn[i][1];
				tempsyn[i][2]=syn[i][2]-1;
			}

			for(i=0;i<27;i++){
				if(black[c][i]!= -1){
					x1 = tempsyn[black[c][i]-1][0];
					y1 = tempsyn[black[c][i]-1][1];
					z1 = tempsyn[black[c][i]-1][2];

					for(tempiter=cubelist.begin();tempiter!=cubelist.end();tempiter++){
						if( (x1==(tempiter->retX())) && (y1==(tempiter->retY())) && (z1==(tempiter->retZ())) ){
							areyou=1;
						}
					}  
				}
			}

			if(areyou==0){
				syn_z--;
			}
	}
	else if(choice==1){
		list <Cube>::iterator tempiter;
		int i;
			for(i=0; i<27; i++){
				tempsyn[i][0]=syn[i][0];
				tempsyn[i][1]=syn[i][1];
				tempsyn[i][2]=syn[i][2]+1;
			}

			for(i=0;i<27;i++){
				if(black[c][i]!= -1){
					x1 = tempsyn[black[c][i]-1][0];
					y1 = tempsyn[black[c][i]-1][1];
					z1 = tempsyn[black[c][i]-1][2];

					for(tempiter=cubelist.begin();tempiter!=cubelist.end();tempiter++){
						if( (x1==(tempiter->retX())) && (y1==(tempiter->retY())) && (z1==(tempiter->retZ())) ){
							areyou=1;
						}
					}  
				}
			}

			if(areyou==0){
				syn_z++;
			}
	}
}

void open_lights(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	// Set lighting intensity and color
	GLfloat qaAmbientLight0[]	= {0.2, 0.2, 0.2, 1.0};
	glLightfv(GL_LIGHT0, GL_AMBIENT, qaAmbientLight0);
	glLightfv(GL_LIGHT1, GL_AMBIENT, qaAmbientLight0);

	/*	LIGHT0	*/
	GLfloat qaDiffuseLight0[]	= {0.8, 0.8, 0.8, 1.0};
	GLfloat lightPosition0[] = { -K-2,K+10,K+N-5, 1.0f };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, qaDiffuseLight0);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition0);

	/*	LIGHT1	*/
	GLfloat qaDiffuseLight1[]	= {0.8, 0.8, 0.8, 1.0};
	GLfloat lightPosition1[] = {K+5,K+5,K+N-5, 1.0f };
	glLightfv(GL_LIGHT1, GL_DIFFUSE, qaDiffuseLight1);
	glLightfv(GL_LIGHT1, GL_POSITION, lightPosition1);

	glClear(GL_COLOR_BUFFER_BIT);

	/*	LIGHT3	*/
	GLfloat qaDiffuseLight3[] = {0.8, 0.8, 0.8, 1.0};
	GLfloat lightPosition3[] = { -K,-K,K+4, 1.0f };
	glLightfv(GL_LIGHT3, GL_DIFFUSE, qaDiffuseLight3);
	glLightfv(GL_LIGHT3, GL_POSITION, lightPosition3);

	// Set material properties
	GLfloat qaBlack[] = {0.5, 0.5, 0.5, 1.0};
	GLfloat qaGreen[] = {0.0, 1.0, 0.0, 1.0};
	GLfloat qaWhite[] = {1.0, 1.0, 1.0, 1.0};
	glMaterialfv(GL_FRONT, GL_AMBIENT, qaGreen);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, qaGreen);
	glMaterialfv(GL_FRONT, GL_SPECULAR, qaWhite);
	glMaterialf(GL_FRONT, GL_SHININESS, 96.0);

 	glEnable(GL_LIGHT0);
  	glEnable(GL_LIGHT1);
	glDisable(GL_LIGHT3);
}

void
close_lights(void)
{
  glDisable(GL_LIGHT0);
  glDisable(GL_LIGHT1);
  glPopAttrib();
}

void camera(){
	if(camera_count==0){
		gluLookAt( x, y, z,    // Set the camera
		x+(2.0f*K+5.0f), y-(2.0f*K+8.0f), z-(2.0f*K+10.0f),  //arxiki thesi
		0.0f, 1.0f, 0.0f);
	}
	else if(camera_count==1){
		gluLookAt( 0.0f,camera2_y ,1.0f ,    // thesi pano apo to tetris
		0.0f, -K, 0.0f,
		0.0f, 1.0f, 0.0f);
	}
	else if(camera_count==2){
		gluLookAt( 0.0f, 0.0f, camera3_z,    // thesi sta plagia
		0.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 0.0f);
	}
}

void element_cube(float x1,float y1,float z1,int clr){
	
	if(clr == YELLOW){
		glColor3f(1.0,1.0,0);
	}
	else if(clr == BLUE){
		glColor3f(0,0,1);
	}
	else if(clr == GREEN){
		glColor3f(0,1,0);
	}
	else {
		glColor3f(1,0,0);
	}

	glBegin(GL_QUADS);
			glVertex3f(x1,y1,z1); // front surface
			glVertex3f(x1+1,y1,z1);
			glVertex3f(x1+1,y1,z1);
			glVertex3f(x1+1,y1-1,z1);
			glVertex3f(x1+1,y1-1,z1);
			glVertex3f(x1,y1-1,z1);
			glVertex3f(x1,y1-1,z1);
			glVertex3f(x1,y1,z1);

			glVertex3f(x1,y1,z1); // up surface
			glVertex3f(x1,y1,z1-1);
			glVertex3f(x1,y1,z1-1);
			glVertex3f(x1+1,y1,z1-1);
			glVertex3f(x1+1,y1,z1-1);
			glVertex3f(x1+1,y1,z1);
			glVertex3f(x1+1,y1,z1);
			glVertex3f(x1,y1,z1);

			glVertex3f(x1,y1,z1); // left surface
			glVertex3f(x1,y1,z1-1);
			glVertex3f(x1,y1,z1-1);
			glVertex3f(x1,y1-1,z1-1);
			glVertex3f(x1,y1-1,z1-1);
			glVertex3f(x1,y1-1,z1);
			glVertex3f(x1,y1-1,z1);
			glVertex3f(x1,y1,z1);

			glVertex3f(x1+1,y1,z1-1); // right surface
			glVertex3f(x1+1,y1,z1);
			glVertex3f(x1+1,y1,z1);
			glVertex3f(x1+1,y1-1,z1);
			glVertex3f(x1+1,y1-1,z1);
			glVertex3f(x1+1,y1-1,z1-1);
			glVertex3f(x1+1,y1-1,z1-1);
			glVertex3f(x1+1,y1,z1-1);

			glVertex3f(x1,y1,z1-1); // back surface
			glVertex3f(x1,y1-1,z1-1);
			glVertex3f(x1,y1-1,z1-1);
			glVertex3f(x1+1,y1-1,z1-1);
			glVertex3f(x1+1,y1-1,z1-1);
			glVertex3f(x1+1,y1,z1-1);
			glVertex3f(x1+1,y1,z1-1);
			glVertex3f(x1,y1,z1-1);

			glVertex3f(x1,y1-1,z1); // down surface
			glVertex3f(x1,y1-1,z1-1);
			glVertex3f(x1,y1-1,z1-1);
			glVertex3f(x1+1,y1-1,z1-1);
			glVertex3f(x1+1,y1-1,z1-1);
			glVertex3f(x1+1,y1-1,z1);
			glVertex3f(x1+1,y1-1,z1);
			glVertex3f(x1,y1-1,z1);
		

	glEnd();
}

void redraw(float syn_xx,float syn_yy,float syn_zz,int blackrow,int clr){
	int temp , i;	

	fix_syn_table(syn_xx,syn_yy,syn_zz);

	// visualise volume

	i=0;
	while((black[blackrow][i]!= -1)&&(i<27)){
		temp = black[blackrow][i] - 1;	
		element_cube(syn[temp][0],syn[temp][1],syn[temp][2],clr);
		i++;
	}
}

int fpstime,timebase=0; // FPS variables

void Create_Shape(){

	Shape A;

	int i,j,first_cube,k ,size,tr;

	label_counter++;

	A.putlabel();
	init_black(A.retlabel());

	list <int> l; // list that contains the already existing neighbor cubes
	list <int>::iterator it; // list that contains the new candidates neighbor cubes

	srand((int)time(NULL));
	while((cube_num = (rand() % 28))==0){
	}

	if(cube_num <= 10){
		A.putcolor(YELLOW);
		temp_color = YELLOW;
	}
	else if(cube_num <=20){
		A.putcolor(BLUE);
		temp_color = BLUE;
	}
	else{
		A.putcolor(RED);
		temp_color = RED;
	}

	while((chosen_cube = (rand() % 28))==0){
	}
	
	while(cube_num!=0){
		
		pushtoblack((A.retlabel()),chosen_cube);
									
		for(j=0;j<6;j++){
			l.push_back(geitones[chosen_cube-1][j]);
		}

		l.remove(-1); 
		for(i=0;i<27;i++){
			l.remove(black[label_counter][i]); // remove duplicates
		}
		
		l.sort();
		l.unique();

		size = (int)l.size(); // candidate cubes for selection

		if(size!=0){
			while((chosen_cube = (rand() % (size+1)))==0){
			}
		}

		it = l.begin();
		for(i=1; i<chosen_cube; i++){
			it++;
		}
		chosen_cube = *it;

		
		cube_num--;
	}

	do{
		syn_x =(float) (rand() % N) - K; // between -K+1 and K-3
	}while((syn_x < -K) || (syn_x > K-3));

	do{
		syn_z = (float) (rand() % N) - K;
	}while((syn_z < -K+3) || (syn_z > K));

	shapelist.push_back(A);

	fpstime=glutGet(GLUT_ELAPSED_TIME);
	timebase=fpstime;
} 

// grid
void myPlex(void){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	float i;

	glColor3f(0,0,0);

	for(i=-K;i<=K;i+=1.0){
		glBegin(GL_LINES);
			glVertex3f(-K,i,-K);
			glVertex3f(K,i,-K);
		glEnd();

		glBegin(GL_LINES);
			glVertex3f(i,-K,-K);
			glVertex3f(i,K,-K);
		glEnd();

	}

	for(i=-K;i<=K;i+=1.0){
		glBegin(GL_LINES);
			glVertex3f(K,i,-K);
			glVertex3f(K,i,K);
		glEnd();
	}

	for(i=-K;i<=(K+0.1);i+=1.0){
		glBegin(GL_LINES);
			glVertex3f(K,-K,i);
			glVertex3f(K,K,i);
		glEnd();
	}

	for(i=-K;i<=K;i+=1.0){
		glBegin(GL_LINES);
			glVertex3f(i,-K,-K);
			glVertex3f(i,-K,K);
		glEnd();
	}

	for(i=-K;i<=(K+0.1);i+=1.0){
		glBegin(GL_LINES);
			glVertex3f(-K,-K,i);
			glVertex3f(K,-K,i);
		glEnd();
	}
}

void draw_text(
		float x,
		float y,
		float z,
		char *str) {
  char *c;
  glRasterPos3f(x, y,z);
  for (c=str; *c != '\0'; c++) {
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
  }
}

// grid limits
int limits_l;
int limits_r;
int limits_b;
int limits_f;
int limits_d = DOWN_LEVEL;

void check_limits(){
	int c = label_counter;
	int leftflag=0;
	int rightflag=0;
	int backflag=0;
	int frontflag=0;
	int downflag=0;	
	int i;

	// priorities
	for(i=0;i<27;i++){
		if((black[c][i]==1)  ||
		   (black[c][i]==6)  ||
		   (black[c][i]==7)  ||
		   (black[c][i]==10) ||
		   (black[c][i]==15) ||
		   (black[c][i]==16) ||
		   (black[c][i]==19) ||
		   (black[c][i]==24) ||
		   (black[c][i]==25)   ){
				if(leftflag<3){	
					limits_l = LEFT_COLUMN;
					leftflag=3;
				}
				if(rightflag<1){
					limits_r = LEFT_COLUMN;
					rightflag=1;
				}
		}
		if((black[c][i]==3)  ||
		   (black[c][i]==4)  ||
		   (black[c][i]==9)  ||
		   (black[c][i]==12) ||
		   (black[c][i]==13) ||
		   (black[c][i]==18) ||
		   (black[c][i]==21) ||
		   (black[c][i]==22) ||
		   (black[c][i]==27)   ){
				if(leftflag<1){
					limits_l = RIGHT_COLUMN;
					leftflag=1;
				}
				if(rightflag<3){
					limits_r = RIGHT_COLUMN;
					rightflag=3;
				}
		}

		if((black[c][i]==2)  ||
		   (black[c][i]==5)  ||
		   (black[c][i]==8)  ||
		   (black[c][i]==11) ||
		   (black[c][i]==14) ||
		   (black[c][i]==17) ||
		   (black[c][i]==20) ||
		   (black[c][i]==23) ||
		   (black[c][i]==26)   ){
				if(leftflag<2){
					limits_l = MIDDLE_COLUMN;
					leftflag=2;
				}
				if(rightflag<2){
					limits_r = MIDDLE_COLUMN;
					rightflag=2;
				}
		}

		if((black[c][i]==1) ||
		   (black[c][i]==2) ||
		   (black[c][i]==3) ||
		   (black[c][i]==4) ||
		   (black[c][i]==5) ||
		   (black[c][i]==6) ||
		   (black[c][i]==7) ||
		   (black[c][i]==8) ||
		   (black[c][i]==9)   ){
				if(frontflag<3){	
					limits_f = FRONT_COLUMN;
					frontflag=3;
				}
				if(backflag<1){
					limits_b = FRONT_COLUMN;
					backflag=1;
				}
		}
		if((black[c][i]==19) ||
		   (black[c][i]==20) ||
		   (black[c][i]==21) ||
		   (black[c][i]==22) ||
		   (black[c][i]==23) ||
		   (black[c][i]==24) ||
		   (black[c][i]==25) ||
		   (black[c][i]==26) ||
		   (black[c][i]==27)   ){
				if(frontflag<1){
					limits_f = BACK_COLUMN;
					frontflag=1;
				}
				if(backflag<3){
					limits_b = BACK_COLUMN;
					backflag=3;
				}
		}

		if((black[c][i]==10) ||
		   (black[c][i]==11) ||
		   (black[c][i]==12) ||
		   (black[c][i]==13) ||
		   (black[c][i]==14) ||
		   (black[c][i]==15) ||
		   (black[c][i]==16) ||
		   (black[c][i]==17) ||
		   (black[c][i]==18)   ){
				if(frontflag<2){
					limits_f = MIDDLE_COLUMN;
					frontflag=2;
				}
				if(backflag<2){
					limits_b = MIDDLE_COLUMN;
					backflag=2;
				}
		}

		if((black[c][i]==1)  ||
		   (black[c][i]==2)  ||
		   (black[c][i]==3)  ||
		   (black[c][i]==10) ||
		   (black[c][i]==11) ||
		   (black[c][i]==12) ||
		   (black[c][i]==19) ||
		   (black[c][i]==20) ||
		   (black[c][i]==21)   ){
				if(downflag<3){	
					limits_d = DOWN_LEVEL;
					downflag=3;
				}
		}
		if((black[c][i]==7)  ||
		   (black[c][i]==8)  ||
		   (black[c][i]==9)  ||
		   (black[c][i]==16) ||
		   (black[c][i]==17) ||
		   (black[c][i]==18) ||
		   (black[c][i]==25) ||
		   (black[c][i]==26) ||
		   (black[c][i]==27)   ){
				if(downflag<1){
					limits_d = UP_LEVEL;
					downflag=1;
				}
		}

		if((black[c][i]==4)  ||
		   (black[c][i]==5)  ||
		   (black[c][i]==6)  ||
		   (black[c][i]==13) ||
		   (black[c][i]==14) ||
		   (black[c][i]==15) ||
		   (black[c][i]==22) ||
		   (black[c][i]==23) ||
		   (black[c][i]==24)   ){
				if(downflag<2){
					limits_d = MIDDLE_LEVEL;
					downflag=2;
				}
		}
	}//end for loop
}

void updateBlack_X(){
	int j;
	int c = label_counter;

	for(j=0;j<27;j++){
		if(black[c][j] == 1)
			black[c][j] = 7;
		else if(black[c][j] == 2)
			black[c][j] = 8;
		else if(black[c][j] == 3)
			black[c][j] = 9;
		else if(black[c][j] == 4)
			black[c][j] = 18;
		else if(black[c][j] == 5)
			black[c][j] = 17;
		else if(black[c][j] == 6)
			black[c][j] = 16;
		else if(black[c][j] == 7)
			black[c][j] = 25;
		else if(black[c][j] == 8)
			black[c][j] = 26;
		else if(black[c][j] == 9)
			black[c][j] = 27;
		else if(black[c][j] == 10)
			black[c][j] = 6;
		else if(black[c][j] == 11)
			black[c][j] = 5;
		else if(black[c][j] == 12)
			black[c][j] = 4;
		else if(black[c][j] == 16)
			black[c][j] = 24;
		else if(black[c][j] == 17)
			black[c][j] = 23;
		else if(black[c][j] == 18)
			black[c][j] = 22;
		else if(black[c][j] == 19)
			black[c][j] = 1;
		else if(black[c][j] == 20)
			black[c][j] = 2;
		else if(black[c][j] == 21)
			black[c][j] = 3;
		else if(black[c][j] == 22)
			black[c][j] = 12;
		else if(black[c][j] == 23)
			black[c][j] = 11;
		else if(black[c][j] == 24)
			black[c][j] = 10;
		else if(black[c][j] == 25)
			black[c][j] = 19;
		else if(black[c][j] == 26)
			black[c][j] = 20;
		else if(black[c][j] == 27)
			black[c][j] =21;
	}
	check_limits();
}

void updateBlack_Y(){
	int j;
	int c = label_counter;

	for(j=0;j<27;j++){
		if(black[c][j] == 1)
			black[c][j] = 19;
		else if(black[c][j] == 2)
			black[c][j] = 10;
		else if(black[c][j] == 3)
			black[c][j] = 1;
		else if(black[c][j] == 4)
			black[c][j] = 6;
		else if(black[c][j] == 5)
			black[c][j] = 15;
		else if(black[c][j] == 6)
			black[c][j] = 24;
		else if(black[c][j] == 7)
			black[c][j] = 25;
		else if(black[c][j] == 8)
			black[c][j] = 16;
		else if(black[c][j] == 9)
			black[c][j] = 7;
		else if(black[c][j] == 10)
			black[c][j] = 20;
		else if(black[c][j] == 12)
			black[c][j] = 2;
		else if(black[c][j] == 13)
			black[c][j] = 5;
		else if(black[c][j] == 15)
			black[c][j] = 23;
		else if(black[c][j] == 16)
			black[c][j] = 26;
		else if(black[c][j] == 18)
			black[c][j] = 8;
		else if(black[c][j] == 19)
			black[c][j] = 21;
		else if(black[c][j] == 20)
			black[c][j] = 12;
		else if(black[c][j] == 21)
			black[c][j] = 3;
		else if(black[c][j] == 22)
			black[c][j] = 4;
		else if(black[c][j] == 23)
			black[c][j] = 13;
		else if(black[c][j] == 24)
			black[c][j] = 22;
		else if(black[c][j] == 25)
			black[c][j] = 27;
		else if(black[c][j] == 26)
			black[c][j] = 18;
		else if(black[c][j] == 27)
			black[c][j] = 9;
	}
	check_limits();
}

void updateBlack_Z(){
	int j;
	int c = label_counter;

	for(j=0;j<27;j++){
		if(black[c][j] == 1)
			black[c][j] = 7;
		else if(black[c][j] == 2)
			black[c][j] = 6;
		else if(black[c][j] == 3)
			black[c][j] = 1;
		else if(black[c][j] == 4)
			black[c][j] = 2;
		else if(black[c][j] == 6)
			black[c][j] = 8;
		else if(black[c][j] == 7)
			black[c][j] = 9;
		else if(black[c][j] == 8)
			black[c][j] = 4;
		else if(black[c][j] == 9)
			black[c][j] = 3;
		else if(black[c][j] == 10)
			black[c][j] = 16;
		else if(black[c][j] == 11)
			black[c][j] = 15;
		else if(black[c][j] == 12)
			black[c][j] = 10;
		else if(black[c][j] == 13)
			black[c][j] = 11;
		else if(black[c][j] == 15)
			black[c][j] = 17;
		else if(black[c][j] == 16)
			black[c][j] = 18;
		else if(black[c][j] == 17)
			black[c][j] = 13;
		else if(black[c][j] == 18)
			black[c][j] = 12;
		else if(black[c][j] == 19)
			black[c][j] = 25;
		else if(black[c][j] == 20)
			black[c][j] = 24;
		else if(black[c][j] == 21)
			black[c][j] = 19;
		else if(black[c][j] == 22)
			black[c][j] = 20;
		else if(black[c][j] == 24)
			black[c][j] = 26;
		else if(black[c][j] == 25)
			black[c][j] = 27;
		else if(black[c][j] == 26)
			black[c][j] = 22;
		else if(black[c][j] == 27)
			black[c][j] = 21;
	}

	check_limits();
}

/*	EASTER EGG !!!!	*/

bool easter_egg=false;
int easter_secs=0;
void easter_egg_light(){
	glClear(GL_COLOR_BUFFER_BIT);

/*	LIGHT2 = easter egg light	*/
	GLfloat qaDiffuseLight2[] = {0.0, 1.0, 0.0, 1.0};
	GLfloat lightPosition2[] = { 0,0,K+2, 1.0f };
	glLightfv(GL_LIGHT2, GL_DIFFUSE, qaDiffuseLight2);
	glLightfv(GL_LIGHT2, GL_POSITION, lightPosition2);

	glEnable(GL_LIGHT2);
}

void easter_egg_draw(){
	iter=shapelist.begin();
	for(iter; iter!=--shapelist.end();iter++){
		redraw( (iter->retX()), (iter->retY()), (iter->retZ()) , (iter->retlabel()) ,GREEN);
	}					
	
	redraw(syn_x,syn_y,syn_z,label_counter,GREEN);
}

/* EASTER EGG END */

int start=0;
bool paused=false;
int game_speed = 1000;
bool lights_on=true;

void renderScene(void) {

glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

// reset transformations
glLoadIdentity();

camera();

if(game_over==true){
	close_lights();
	camera_count=0;
	glPushMatrix();								
		glRotatef(rotation,0,1,0);				   
	glPopMatrix();
	rotation += rotation_speed/15;
}
else{		
	if(lights_on==true){
		open_lights();
	}
	else{
		glDisable(GL_LIGHT3);
		lights_on=true;
	}
		

	if(score==500){
		easter_egg=true;
		easter_egg_light();
		glDisable(GL_LIGHT0);
		glDisable(GL_LIGHT1);
		camera_count=0;
		score +=50;
	}

	if(paused == false){
		fpstime=glutGet(GLUT_ELAPSED_TIME);
	}
}
	
	if(camera_count==0){					  
		glPushMatrix();						
			glRotatef(rotation,0,1,0);				   
			myPlex();
			
			if(game_over==false){
			if(((limits_d == DOWN_LEVEL)&&(syn_y == -K+1)) ||
			   ((limits_d == MIDDLE_LEVEL)&&(syn_y == -K)) ||
			   ((limits_d == UP_LEVEL)&&(syn_y == -K-1))   || (reached==true)) {
				if(start==1){ // if there is at least one volume					
					puttocosmos();
					ifgameover();
					reached=false;
					score++;
				}
				Create_Shape();
				check_limits();
				if(limits_d == DOWN_LEVEL){
					syn_y = K+1;
				}
				else if(limits_d == MIDDLE_LEVEL){
					syn_y = K;
				}
				else if(limits_d == UP_LEVEL){
					syn_y = K-1;
				}
				start=1;
				
				if(easter_egg==true)
					easter_secs++;
			}

			if (fpstime - timebase > game_speed) {		
				check_aroundY();
				timebase = fpstime;
			}
		}//end game over if
			if(easter_egg==false){
				iter2=cubelist.begin();
				for(iter2; iter2!=cubelist.end();iter2++){
					element_cube( (iter2->retX()), (iter2->retY()), (iter2->retZ()) ,(iter2->retcolor()));
				}					
				redraw(syn_x,syn_y,syn_z,label_counter,temp_color);
			}
			else if(easter_egg==true){
				if(easter_secs<4){
					easter_egg_draw();
				}
				else{
					easter_egg=false;
					glDisable(GL_LIGHT2);
					glEnable(GL_LIGHT0);
					glEnable(GL_LIGHT1);
				}
			}

		glPopMatrix();
	}
	else if(camera_count==1){			
			glEnable(GL_LIGHT3);
			lights_on=true;			
			glPushMatrix();						
			glRotatef(rotation2,0,1,0);				   
			myPlex();

			if(game_over==false){
			if(((limits_d == DOWN_LEVEL)&&(syn_y == -K+1)) ||
			   ((limits_d == MIDDLE_LEVEL)&&(syn_y == -K)) ||
			   ((limits_d == UP_LEVEL)&&(syn_y == -K-1))   || (reached==true)) {
				if(start==1){
					puttocosmos();
					ifgameover();
					reached=false;
					score++;
				}
				Create_Shape();
				check_limits();
				if(limits_d == DOWN_LEVEL){
					syn_y = K+1;
				}
				else if(limits_d == MIDDLE_LEVEL){
					syn_y = K;
				}
				else if(limits_d == UP_LEVEL){
					syn_y = K-1;
				}
				start=1;

				if(easter_egg==true)
					easter_secs++;
			}
			
			if (fpstime - timebase > game_speed) {		
				check_aroundY();
				timebase = fpstime;
			}
		}//end game over if
			if(easter_egg==false){
				iter2=cubelist.begin();
				for(iter2; iter2!=cubelist.end();iter2++){
					element_cube( (iter2->retX()), (iter2->retY()), (iter2->retZ()) ,(iter2->retcolor()));
				}					
				redraw(syn_x,syn_y,syn_z,label_counter,temp_color);
			}
			else if(easter_egg==true){
				if(easter_secs<4){
					easter_egg_draw();
				}
				else{
					easter_egg=false;
					glDisable(GL_LIGHT2);
					glEnable(GL_LIGHT0);
					glEnable(GL_LIGHT1);
				}
			}
		glPopMatrix();
	}
	else if(camera_count==2){
			glPushMatrix();						
			glRotatef(rotation3,0,1,0);				   
			myPlex();

			if(game_over==false){
			if(((limits_d == DOWN_LEVEL)&&(syn_y == -K+1)) ||
			   ((limits_d == MIDDLE_LEVEL)&&(syn_y == -K)) ||
			   ((limits_d == UP_LEVEL)&&(syn_y == -K-1))   || (reached==true)) {
				if(start==1){
					puttocosmos();
					ifgameover();
					reached=false;
					score++;
				}
				Create_Shape();
				check_limits();
				if(limits_d == DOWN_LEVEL){
					syn_y = K+1;
				}
				else if(limits_d == MIDDLE_LEVEL){
					syn_y = K;
				}
				else if(limits_d == UP_LEVEL){
					syn_y = K-1;
				}
				start=1;
				
				if(easter_egg==true)
					easter_secs++;
			}
			
			if (fpstime - timebase > game_speed) {		
				check_aroundY();
				timebase = fpstime;
			}
		}//end game over if
			if(easter_egg==false){
				iter2=cubelist.begin();
				for(iter2; iter2!=cubelist.end();iter2++){
					element_cube( (iter2->retX()), (iter2->retY()), (iter2->retZ()) ,(iter2->retcolor()));
				}					
				redraw(syn_x,syn_y,syn_z,label_counter,temp_color);
			}
			else if(easter_egg==true){
				if(easter_secs<4){
					easter_egg_draw();
				}
				else{
					easter_egg=false;
					glDisable(GL_LIGHT2);
					glEnable(GL_LIGHT0);
					glEnable(GL_LIGHT1);
				}
			}
		glPopMatrix();
	}

			sprintf(s1,"%d",score);

			if(paused == true){
				if(camera_count==0){
					glPushMatrix();	
						glColor3f(0,1,0);		
						draw_text(x+3,y-2.8,z-5,(char*)"GAME PAUSED");
						draw_text(x+2.5,y-3.25,z-5,(char*)"PRESS");
						glColor3f(1 ,0.11,0.68);					
						draw_text(x+3.5,y-3.63,z-5,(char*)"S");
						glColor3f(0,1,0);		
						draw_text(x+4,y-3.8,z-5,(char*)"TO CONTINUE");
					glPopMatrix();	

					glPushMatrix();	
						glColor3f(1 ,0.11,0.68);					
						draw_text(x+7,y-2.5 ,z-5,(char*)"SCORE : ");
						glColor3f(0,1,0);
						draw_text(x+9,y-2.9 ,z-5,s1);
					glPopMatrix();
				}
				else if(camera_count==1){
					glPushMatrix();	
						glColor3f(0,1,0);			
						draw_text(-0.5,camera2_y-K ,0,(char*)"GAME PAUSED");
						draw_text(-1.0,camera2_y-K,0.5,(char*)"PRESS");
						glColor3f(1 ,0.11,0.68);					
						draw_text(0,camera2_y-K,0.5,(char*)"S");
						glColor3f(0,1,0);		
						draw_text(0.5,camera2_y-K,0.5,(char*)"TO CONTINUE");
					glPopMatrix();	

					glPushMatrix();	
						glColor3f(1 ,0.11,0.68);					
						draw_text(1,camera2_y-3 ,-0.2,(char*)"SCORE : ");
						glColor3f(0,1,0);	
						draw_text(1.4,camera2_y-3, -0.2,s1);
					glPopMatrix();
				}
				else if(camera_count==2){
					glPushMatrix();	
						glColor3f(0,1,0);			
						draw_text(-0.5,0.5 ,camera3_z-5,(char*)"GAME PAUSED");
						draw_text(-0.8,-0.2,camera3_z-5,(char*)"PRESS");
						glColor3f(1 ,0.11,0.68);					
						draw_text(-0.2,-0.2,camera3_z-5,(char*)"S");
						glColor3f(0,1,0);			
						draw_text(0,-0.2,camera3_z-5,(char*)"TO CONTINUE");
					glPopMatrix();

					glPushMatrix();	
						glColor3f(1 ,0.11,0.68);					
						draw_text(0.65,0.65 ,camera3_z-2,(char*)"SCORE : ");
						glColor3f(0,1,0);	
						draw_text(0.92,0.65, camera3_z-2,s1);
					glPopMatrix();
				}
			}	

			if(camera_count==0){
				glPushMatrix();	
					glColor3f(1 ,0.11,0.68);					
					draw_text(x+7,y-2.5 ,z-5,(char*)"SCORE : ");
					glColor3f(0,1,0);
					draw_text(x+9,y-2.9 ,z-5,s1);
				glPopMatrix();	

				if(game_over==true){
					glPushMatrix();	
						glColor3f(0,1,0);		
						draw_text(x+3,y-4.5 ,z-5,(char*)"GAME OVER");
					glPopMatrix();
				}
			}
			else if(camera_count==1){
				glPushMatrix();	
					glColor3f(1 ,0.11,0.68);					
					draw_text(1,camera2_y-3 ,-0.2,(char*)"SCORE : ");
					glColor3f(0,1,0);	
					draw_text(1.4,camera2_y-3, -0.2,s1);
				glPopMatrix();
			}
			else if(camera_count==2){
				glPushMatrix();	
					glColor3f(1 ,0.11,0.68);					
					draw_text(0.65,0.65 ,camera3_z-2,(char*)"SCORE : ");
					glColor3f(0,1,0);	
					draw_text(0.92,0.65, camera3_z-2,s1);
				glPopMatrix();
			}

	//easter egg text
	if(easter_egg==true){	
		glPushMatrix();	
			glColor3f(1,0,0);			
			draw_text(x+0.5,y-2.0,z-3,(char*)"YOU ARE EXCELENT!!  YOU MANAGED TO MAKE 500 POINTS!!!!");
		glPopMatrix();
	}

if(rotation>360.0f)
	rotation -= 360.0f;
if(rotation2>360.0f)
	rotation2 -= 360.0f;
if(rotation3>360.0f)
	rotation3 -= 360.0f;

glutSwapBuffers();
}

void changeSize(int w, int h) {

if (h == 0) // Prevent a divide by zero, when window is too short
h = 1;
float ratio = w * 1.0 / h;

glMatrixMode(GL_PROJECTION); // Use the Projection Matrix

glLoadIdentity(); // Reset Matrix

glViewport(0, 0, w, h); // Set the viewport to be the entire window

gluPerspective(45.0f, ratio, 0.1f, 100.0f); // Set the correct perspective.

glMatrixMode(GL_MODELVIEW); // Get Back to the Modelview
}

void processNormalKeys(unsigned char key, int xx, int yy) {

if (key == 27){
	exit(0);
}
else if((key == 77) || (key == 109)){
	if(game_over==false){
	if((paused==false) && (easter_egg==false)){
		camera_count++;

		if(camera_count==0){
			rotation3 = 0;
			camera3_z = N+12.0f;
		}	
		else if(camera_count==1){
			rotation = 0;
			x=-K-5.0f;
			y=K+8.0f;	// default position of camera 1
			z=K+10.0f;
		}
		else if(camera_count==2){
			rotation2 = 0;
			camera2_y = K+20.0f;
		}
		else if(camera_count==3){
			rotation3 = 0;
			camera3_z = N+12.0f;	
			camera_count=0;
		}
	}
	}
}
else if((key == 83) || (key == 115)){ // "s" start-pause
	if(game_over==false){	
	if(paused == true){
		paused=false;
	}		
	else{
		paused=true;
	}
	}
}

else if((key == 88) || (key == 120)){ // "x" x axis rotation
     if(game_over==false){
	if(paused==false)	
		updateBlack_X();
     }
}
else if((key == 86) || (key == 118)){ // "v" y axis rotation
   if(game_over==false){		
	if(paused==false)		
		updateBlack_Y();
  }
}
else if((key == 67) || (key == 99)){ // "c" z axis rotation
  if(game_over==false){		
	if(paused==false)		
		updateBlack_Z();
  }
}

else if((key == 74) || (key == 106)){ // turn camera left
	if(game_over==false){		
	if(paused==false){	
		if(camera_count==0)
			rotation += 2*rotation_speed;
		else if(camera_count==1)
			rotation2 += 2*rotation_speed;
		else if(camera_count==2)
			rotation3 += 2*rotation_speed;
	}
	}
}
else if((key == 76) || (key == 108)){ // turn camera right
	if(game_over==false){	
	if(paused==false){	
		if(camera_count==0)
			rotation -= 2*rotation_speed;
		else if(camera_count==1)
			rotation2 -= 2*rotation_speed;
		else if(camera_count==2)
			rotation3 -= 2*rotation_speed;
	}
	}
}
else if((key == 73) || (key == 105)){ // zoom in

	if(game_over==false){	
	if(paused==false){
		if(camera_count==0){
			if( (x<(K-2)) && (z>-K+1) ){
				x += zoomx * rotation_speed;
				z -= zoomz * rotation_speed;
				y -= zoomy * rotation_speed;
			} 
		}
		else if(camera_count==1){
			if( camera2_y > -K+3){
				camera2_y -= zoomy * rotation_speed;
			} 
		}
		else if(camera_count==2){
			if(camera3_z > -K+15){
				camera3_z -= zoomz * rotation_speed;
			}
		}
	}
	}
}
else if((key == 75) || (key == 107)){ // zoom out
   if(game_over==false){		
	if(paused==false){	
		if(camera_count==0){		
			if(K >= 20){
				if( (x>-3*K) && (y<K) && (z<K) ){
					x -= zoomx * rotation_speed;
					z += zoomz * rotation_speed;
					y += zoomy * rotation_speed;
				}
			}
			else if(K < 20){
				if( (x>-3*K) && (y<4*K) && (z<4*K) ){
					x -= zoomx * rotation_speed;
					z += zoomz * rotation_speed;
					y += zoomy * rotation_speed;
				}
			}
		}
		else if(camera_count==1){
			if(K >= 20){
				if(camera2_y < K+8){
					camera2_y += zoomy * rotation_speed;
				}
			}
			else if(K < 20){
				if(camera2_y < K+30){
					camera2_y += zoomy * rotation_speed;
				}
			}
		}
		else if(camera_count==2){
			if(K >= 20){
				if(camera3_z < K+8){
					camera3_z += zoomz * rotation_speed;
				}
			}
			else if(K < 20){
				if(camera3_z < N+20){
					camera3_z += zoomz * rotation_speed;
				}
			}
		}
	}//end if paused game
   }//end if game over
}

/* '+' and '-' to adjust game speed */

else if(key == 43){ //  +
	if(game_speed> 300){
		game_speed -=300;
	}
}
else if(key == 45){	//  -
	if(game_speed<3000){
		game_speed +=300;
	}
}
}

void processSpecialKeys(int key, int xx, int yy) {

if(game_over==false){		

switch (key) {

case GLUT_KEY_LEFT :
	if(paused==false){
		if(limits_l == LEFT_COLUMN){
			if(syn_x > -K)
				check_aroundX(0);		
		}
		else if(limits_l == MIDDLE_COLUMN){
			if(syn_x > -K-1)
				check_aroundX(0);						
		}
		else if(limits_l == RIGHT_COLUMN){
			if(syn_x > -K-2)
				check_aroundX(0);						
		}
	}
break;

case GLUT_KEY_RIGHT :
	if(paused==false){			
		if(limits_r == RIGHT_COLUMN){	
			if(syn_x < K-3)
				check_aroundX(1);						
		}
		else if(limits_r == MIDDLE_COLUMN){	
			if(syn_x < K-2)	
				check_aroundX(1);						
		}
		else if(limits_r == LEFT_COLUMN){	
			if(syn_x < K-1)	
				check_aroundX(1);						
		}
	}
	
break;

case GLUT_KEY_UP :
	if(paused==false){
		if(limits_b == BACK_COLUMN){	
			if(syn_z>-K+3)	
				check_aroundZ(0);						
		}
		else if(limits_b == KENTRO_COLUMN){	
			if(syn_z>-K+2)	
				check_aroundZ(0);						
		}
		else if(limits_b == FRONT_COLUMN){	
			if(syn_z>-K+1)	
				check_aroundZ(0);						
		}
	}
break;
case GLUT_KEY_DOWN :
	if(paused==false){
		if(limits_f == FRONT_COLUMN){		
			if(syn_z<K)	
				check_aroundZ(1);						
		}
		else if(limits_f == KENTRO_COLUMN){		
			if(syn_z<K+1)	
				check_aroundZ(1);						
		}
		else if(limits_f == BACK_COLUMN){		
			if(syn_z<K+2)	
				check_aroundZ(1);						
		}
	}
break;
}
}
}

int main(int argc, char **argv) {

glutInit(&argc, argv); // init GLUT and create window
glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
glutInitWindowPosition(100,100);
glutInitWindowSize(800,600);
glutCreateWindow("Tetris 3d");

init();

// register callbacks
glutDisplayFunc(renderScene);
glutReshapeFunc(changeSize);
glutIdleFunc(renderScene);
glutKeyboardFunc(processNormalKeys);
glutSpecialFunc(processSpecialKeys);

glEnable(GL_DEPTH_TEST); // OpenGL init

glutMainLoop(); // enter GLUT event processing cycle

}
