#include <GL/glut.h>
#include<string.h>
#include<stdio.h>
#include<stdbool.h>
//#include<GLFW/glfw3.h>
char inputText[256] = "Salutare";
char locatie[256];
int num=0;
FILE *fp;
FILE* file2;
FILE* rezultat;

struct baza{
	char id[100];
	char parola[100];
	unsigned long long seed;
	char hash[256];
	int nr_seed;
	int p_size;
};

void gui_cutii_4f(float culoare1,float culoare2,float culoare3,float culoare4,float cordx,float cordy,float cord_height,float cord_width){   //culoarea cutiei urmata de dimensiune
	glColor4f(culoare1,culoare2,culoare3,culoare4);
	glBegin(GL_QUADS);
		glVertex2f(cordx, cordy);
		glVertex2f(cordx, cordy - cord_height);
		glVertex2f(cordx + cord_width, cordy - cord_height);
		glVertex2f(cordx + cord_width, cordy);
	glEnd();
}
void gui_cutii_3f(float culoare1,float culoare2,float culoare3,float cordx,float cordy,float cord_height,float cord_width){   //culoarea cutiei urmata de dimensiune
	glColor3f(culoare2,culoare2,culoare3);
	glBegin(GL_QUADS);
		glVertex2f(cordx, cordy);
		glVertex2f(cordx, cordy + cord_height);
		glVertex2f(cordx + cord_width, cordy + cord_height);
		glVertex2f(cordx + cord_width, cordy);
	glEnd();
}

struct baza* mare = NULL;
void numarare_randuri(){
	char continut[100];
		if(fp==NULL){
			printf("Nu am putut deschide fisierul.\n");
			return;
		}
		while(fscanf(fp,"%s %s",continut,continut)!=EOF){
	if(strlen(continut)>100){
		printf("In document a fost un id/parola de peste 100 de caractere (incercati sa verificati randul %d).",num);
		return;
	}
	num++;
	}
	fclose(fp);
	fp=fopen(locatie,"r");
	mare = malloc(num * sizeof(struct baza));
	for(int i=0;i<num;i++){
		fscanf(fp,"%s %s",mare[i].id,mare[i].parola);
		mare[i].p_size=strlen(mare[i].parola);
	}
	fclose(fp);
}

void afisare_rezultat(){
	char string4[]="Conversia a fost facuta cu succes!";
	glColor3f(0.0f, 0.0f, 0.0f);
	glRasterPos2f(-0.5f,0.2f);
	for (int i = 0; i < strlen(string4); i++) {
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15,string4[i]);
	}
	glutSwapBuffers();
}

void afisare_problema(){
	char string4[]="Aceasta locatie nu exista sau a aparut o problema.";
	glColor3f(0.0f, 0.0f, 0.0f);
	glRasterPos2f(-0.7f,0.1f);
	for (int i = 0; i < strlen(string4); i++) {
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15,string4[i]);
	}
	glutSwapBuffers();
}

void creare_seed(){
	for(int i=0;i<num;i++){
		unsigned long long z = 1;
		for (int j = 0; mare[i].id[j] != '\0'; j++) {
		    z *= (unsigned long long)(mare[i].id[j]);
		}
		for (int j = 0; mare[i].parola[j] != '\0'; j++) {
		    z += (unsigned long long)(mare[i].parola[j]);
		}
		mare[i].seed = z;
		}
}
void dim_seed(){
	int i;
	for(i=0;i<num;i++){
		unsigned long long temp;
		if(mare[i].seed==0){
				printf("A aparut o problema.");
				return ;
			}
		temp=mare[i].seed;
		mare[i].nr_seed=0;
		while(temp!=0){
			temp=temp/10;
			mare[i].nr_seed++;
		}
		if(mare[i].nr_seed<6){
			printf("Parola %d este prea scurta.",i);
			return ;
		}
	}
}

void hash(struct baza* mare,int i){
	srand((unsigned int)mare[i].seed);
		for(int j=0;j<mare[i].nr_seed;j++){
			mare[i].hash[j]=(char)((rand()%(126-33+1))+33);
		}
		mare[i].hash[mare[i].nr_seed] = '\0';
}

void buton1_apasat() {
	file2 = fopen("locatie.txt", "w");
	if (file2 == NULL) {
		printf("Nu am putut crea documentul.\n");
	    return;
	}
	fprintf(file2, "%s", inputText);
	fclose(file2);

	file2=fopen("locatie.txt", "r");
	if (file2 == NULL) {
	    printf("Nu am putut deschide documentul.\n");
	    return;
	}
	fscanf(file2,"%s",locatie);
	fclose(file2);
	fp=fopen(locatie,"r");
	if (fp == NULL) {
	        afisare_problema();
	        return;
	    }
	numarare_randuri();
	creare_seed();
	dim_seed();
	for (int i = 0; i < num; i++) {
	        hash(mare, i);
	    }
	rezultat=fopen("rezultat.txt","w");
	fprintf(rezultat,"%30s%30s%30s%30s%30s","id","parola","seed","hash","nr_seed\n");
	for(int i=0;i<num;i++){
		fprintf(rezultat, "%30s%30s%30lld%30s%30d\n", mare[i].id, mare[i].parola, mare[i].seed, mare[i].hash, mare[i].nr_seed);
	}
	fclose(rezultat);
	afisare_rezultat();
}

float inputBoxX = -0.85f;
float inputBoxY = -0.2f; //cutie text
float inputBoxWidth = 1.7f;
float inputBoxHeight = 0.1f;

float button_x2 = -0.30f;
float button_y2 = -0.6f;
float button_width2 = 0.6f;// buton utilizator
float button_height2 = 0.1f;

float button_x = -0.30f;
float button_y = -0.8f;
float button_width = 0.6f;// buton baza
float button_height = 0.1f;

void display_primar() {
	int lungime, i;
	char string[]="Care este locatia de unde veti sa extrageti baza de date ?";
	char string2[]="Introdu documentul";
	char string3[]="Iesire";
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	gui_cutii_3f(0.5f, 0.5f, 0.5f,button_x,button_y,button_height,button_width);
	gui_cutii_3f(0.5f, 0.5f, 0.5f,button_x2,button_y2,button_height2,button_width2);
	gui_cutii_4f(0.0f, 0.8f, 1.0f, 1.0f,inputBoxX,inputBoxY,inputBoxHeight,inputBoxWidth);

		glColor3f(0.0f, 0.0f, 0.0f);
		glRasterPos2f(inputBoxX + 0.01f, inputBoxY - 0.07f);
		for (int i = 0; i < strlen(inputText); i++) {
		    glutBitmapCharacter(GLUT_BITMAP_9_BY_15, inputText[i]);
		}

	glColor3f(0.0f, 0.0f, 0.0f);
	lungime = (int)strlen(string);
	glRasterPos2f(-0.8f,0.4f);
	for (i = 0; i < lungime; i++){
		  glutBitmapCharacter(GLUT_BITMAP_9_BY_15,string[i]);
	}
	glColor3f(0.0f, 0.0f, 0.0f);
		lungime = (int)strlen(string3);
		glRasterPos2f(button_x2 + 0.2f, button_y2 + 0.04f);
						for (int i = 0; i < strlen(string3); i++) {
							glutBitmapCharacter(GLUT_BITMAP_9_BY_15,string3[i]);
						}
	glColor3f(0.0f, 0.0f, 0.0f);
	lungime = (int)strlen(string2);
	glRasterPos2f(button_x + 0.04f, button_y + 0.04f);
	for (int i = 0; i < strlen(string2); i++) {
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15,string2[i]);
	}
	glutSwapBuffers();
	}

void mouse_locatie(int x, int y){
	bool a = false;
	float xPos = (float)x / (glutGet(GLUT_WINDOW_WIDTH) / 2.0f) - 1.0f;   //Aceasta formula ar trebui sa mearga pentru orice coordonate intre -1.0f si 1.0f
	float yPos = -((float)y / (glutGet(GLUT_WINDOW_HEIGHT) / 2.0f) - 1.0f);
		if(xPos<=inputBoxX	+inputBoxWidth && xPos>=inputBoxX && yPos <=inputBoxY && yPos >= inputBoxY-inputBoxHeight){
			glutSetCursor(GLUT_CURSOR_TEXT);
			a=true;
		        }
		else {
			a=false;
			glutSetCursor(GLUT_CURSOR_INHERIT);
		       }
}

void mouse(int button, int state, int x, int y) {
	float xPos = (float)x / (glutGet(GLUT_WINDOW_WIDTH) / 2.0f) - 1.0f;   //Aceasta formula ar trebui sa mearga pentru orice coordonate intre -1.0f si 1.0f
	float yPos = -((float)y / (glutGet(GLUT_WINDOW_HEIGHT) / 2.0f) - 1.0f);
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
    	if (xPos >= button_x && xPos <= button_x + button_width && yPos >= button_y && yPos <= button_y + button_height) {
            buton1_apasat();
        }
        else if(xPos >= button_x2 && xPos <= button_x2 + button_width2 && yPos >= button_y2 && yPos <= button_y2 + button_height2){
        	glutLeaveMainLoop();
        }
    }
}

void keyboard(unsigned char key,int a,int b) {
    if (key == 8 || key == 127) {
        if (strlen(inputText) > 0) {
            inputText[strlen(inputText) - 1] = '\0';
        }
    } else if (key >= 32 && key <= 126) {
        int len = strlen(inputText);
        if (len < sizeof(inputText) - 1) {
            inputText[len] = key;
            inputText[len + 1] = '\0';
        }
    }
    glutPostRedisplay();
}

void stocare(int num,struct baza* mare,FILE* fp){
	for(int i=0;i<num;i++){
		fscanf(fp,"%s %s",mare[i].id,mare[i].parola);
		mare[i].p_size=strlen(mare[i].parola);
	}
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitWindowSize(640, 640);
	glutCreateWindow("OpenGL Setup Test");
	glutDisplayFunc(display_primar);
	glutMouseFunc(mouse);
	glutKeyboardFunc(keyboard);
	glutPassiveMotionFunc(mouse_locatie);
	glutMainLoop();

	fclose(fp);
	free(mare);
return 0;
}
