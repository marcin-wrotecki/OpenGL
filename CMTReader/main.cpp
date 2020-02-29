#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include <iostream>
#include <vector>
#include <fstream>
#include <string>


#include "GL\glew.h"
#include "GL\freeglut.h"

#include "shaderLoader.h" //narzŕdzie do │adowania i kompilowania shaderˇw z pliku
#include "tekstura.h"

//funkcje algebry liniowej
#include "glm/vec3.hpp" // glm::vec3
#include "glm/vec4.hpp" // glm::vec4
#include "glm/mat4x4.hpp" // glm::mat4
#include "glm/gtc/matrix_transform.hpp" // glm::translate, glm::rotate, glm::scale, glm::perspective

// Program bazuje na tutorialu ze strony: https://learnopengl.com/
#define NUMBER_OF_LINES 40000
#define OFFSETY 220
#define OFFSETX 150
#define DIMENSION 3
#define SCALE 0.001f

void idle();
void timer(int value);
void rozmiar(int width, int height);
void rysuj(void);
void klawisz(GLubyte key, int x, int y);
void mysz_ruch(int x, int y);
void mysz(int button, int state, int x, int y);
int Load(std::string filename);



//Wymiary okna
int screen_width = 640;
int screen_height = 480;


int pozycjaMyszyX; // na ekranie
int pozycjaMyszyY;
int mbutton; // wcisiety klawisz myszy

double kameraX=0.0;
double kameraZ = 0.0;
double kameraD = -5;
double kameraPredkosc;
double kameraKat = -20;
double kameraPredkoscObrotu;
double poprzednie_kameraX;
double poprzednie_kameraZ;
double poprzednie_kameraD;

double rotation = 0;

//macierze

glm::mat4 MV; //modelview - macierz modelu i świata
glm::mat4 P;  //projection - macierz projekcji, czyli naszej perspektywy
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);//ODPOWIADA ZA POZYCJE SWIATLA!!!!!!!!
GLuint objectColor_id = 0;
GLuint lightColor_id = 0;
GLuint lightPos_id = 0;
GLuint viewPos_id = 0;
float maxX = INT_MIN, minX = INT32_MAX;
float maxY = INT_MIN, minY = INT32_MAX;
float maxZ = INT_MIN, minZ = INT32_MAX;
float strengthOfLight = 0.1;

float vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

	0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};

/*float vertices2[] = {
	-1,-1,0,
	3,-1,0,
	3,-1,4,
	-1,-1,4,

};*/

float textureCoords[] = {
0,0,
1,0,
1,1,
0,1,

};

float vertices2[NUMBER_OF_LINES*DIMENSION];// = { 0, 0, 561, 220, 0, 521.615, 440, 0, 555.097,

GLuint elements[]={
	0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36
};
GLuint elements2[300000];

/*GLuint elements2[] = {
	0,1,2,3
	
};*/
GLuint tex_id0;
GLint uniformTex0;
//shaders
GLuint programID = 0;
GLuint lamp_ID = 0;

unsigned int VBO, VBO2,VAO[2],ebo,ebo2,vtex;
unsigned int lightVAO;


int main(int argc, char **argv)
{
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(screen_width, screen_height);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Przyklad 3");

	glewInit(); //init rozszerzeszeń OpenGL z biblioteki GLEW

	glutDisplayFunc(rysuj);			// def. funkcji rysuj¦cej
	glutIdleFunc(idle);			// def. funkcji rysuj¦cej w czasie wolnym procesoora (w efekcie: ci¦gle wykonywanej)
	//glutTimerFunc(20, timer, 0);
	glutReshapeFunc(rozmiar); // def. obs-ugi zdarzenia resize (GLUT)

	glutKeyboardFunc(klawisz);		// def. obsługi klawiatury
	glutMouseFunc(mysz); 		// def. obsługi zdarzenia przycisku myszy (GLUT)
	glutMotionFunc(mysz_ruch); // def. obsługi zdarzenia ruchu myszy (GLUT)


	
	std::string filename;
	std::cout << "Prosze podac plik ze wspolrzednymi" << std::endl;
	std::cin >> filename;
	if(!Load(filename))
		return-1;
	programID = loadShaders("vertex_shader.glsl", "fragment_shader.glsl");

	
	glUseProgram(programID);

	objectColor_id = glGetUniformLocation(programID, "objectColor");
	lightColor_id = glGetUniformLocation(programID, "lightColor");
	lightPos_id = glGetUniformLocation(programID, "lightPos");

	glDepthMask(GL_FALSE);
	std::cout << "Prosze podac plik z tekstura" << std::endl;
	std::cin >> filename;
	 tex_id0 = WczytajTeksture(filename.c_str());
	if (tex_id0 == -1)
	{
		MessageBox(NULL, "Nie znaleziono pliku z teksturą", "Problem", MB_OK | MB_ICONERROR);
		exit(0);
	}

	

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

	glGenBuffers(1, &ebo2);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements2), elements2, GL_STATIC_DRAW);

	glGenBuffers(1, &vtex);
	glBindBuffer(GL_ARRAY_BUFFER, vtex);
	glBufferData(GL_ARRAY_BUFFER, sizeof(textureCoords), textureCoords, GL_STATIC_DRAW);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &VBO2);
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2),vertices2,GL_STATIC_DRAW);
	
	


	glGenVertexArrays(2, VAO);
	glBindVertexArray(VAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo2);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);



	// position attribute
	/*glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6* sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);*/

	/*glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_D, tex_id0);
	uniformTex0 = glGetUniformLocation(programID, "tex0");
	glUniform1i(programID, 0); 
	glBindVertexArray(VAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vtex);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);*/
	// second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
	


	lamp_ID = loadShaders("lamp_vshader.glsl", "lamp_fshader.glsl");
	

	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER,VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	// note that we update the lamp's position attribute's stride to reflect the updated buffer data
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	kameraZ = (minZ + maxZ)*0.5* SCALE;
	kameraD = -abs((minY + maxY) *0.5*SCALE);
	kameraX = (minX+maxX)*0.5* SCALE;
	
	lightPos[0] = (minX + maxX) / 2;
	lightPos[1] = (maxY+minY)/2;
	lightPos[2] = 5*maxZ;


	glutMainLoop();					// start

	glDeleteVertexArrays(2, VAO);
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &VBO);

	return(0);

}

void rysuj(void)
{


	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(programID); //u┐yj programu, czyli naszego shadera	

	glVertexAttrib1f(3, minZ);
	glVertexAttrib1f(4, maxZ);
	glVertexAttrib1f(5, strengthOfLight);

	GLuint MVP_id = glGetUniformLocation(programID, "MVP"); // pobierz lokalizację zmiennej 'uniform' "MV" w programie


	MV = glm::mat4(1.0f);  //macierz jednostkowa
	MV = glm::translate(MV, glm::vec3(0, -1, kameraD));

	MV = glm::rotate(MV, (float)glm::radians(kameraZ + 10), glm::vec3(1, 0, 0));
	MV = glm::rotate(MV, (float)glm::radians(kameraX + 30), glm::vec3(0, 1, 0));
	MV = glm::scale(MV, glm::vec3(SCALE, SCALE, SCALE)); //aby uniknac zanikania obiektu przy oddalaniu go oraz dla 
	glm::mat4 MVP = P * MV;
	glUniformMatrix4fv(MVP_id, 1, GL_FALSE, &(MVP[0][0]));

	////////////////////////////////////////////////////

	

	glUniform3f(lightColor_id, 1.0f, 1.0f, 1.0f);
	glUniform3f(lightPos_id, lightPos[0], lightPos[1], lightPos[2]);

	/////////////////////////////////////////////////

	// render 

	/*glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex_id0);
	glUniform1i(uniformTex0, 0);*/
	glBindVertexArray(VAO[0]);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glVertexAttrib3f(1, 0, 1, 0);
	glDrawElements(GL_TRIANGLES, sizeof(elements2), GL_UNSIGNED_INT, 0);

	glUseProgram(lamp_ID);
	;

	GLuint MVPlamp_id = glGetUniformLocation(lamp_ID, "MVP");
	MV = glm::translate(MV, lightPos);
	MV = glm::scale(MV, glm::vec3(1/SCALE, 1/SCALE, 1/SCALE));
	MV = glm::rotate(MV, (float)glm::radians(0.0), glm::vec3(0, 1, 0));
	MVP = P * MV;
	glBindVertexArray(lightVAO);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glUniformMatrix4fv(MVPlamp_id, 1, GL_FALSE, &(MVP[0][0]));

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);





	glFlush();
	glutSwapBuffers();

}
/*###############################################################*/

int Load(std::string filename)
{
		std::fstream file;
		//vertices2 = (float*)calloc(sizeof(float), NUMBER_OF_LINES*DIMENSION);

		file.open(filename, std::ios::in | std::ios::out);

		if (file.good() == true)
		{
			std::string str;
			getline(file, str);
			
			int iterator = 0;
			std::string fetch ="%f %f %f";
			float firstX = 0;
			float firstY = 0;
			sscanf(str.c_str(), "%f %f", &firstX,&firstY);
			int multiplierX = 0;
			int multiplierY = 0;
			
			int iterator2 = DIMENSION;
	
			for (int i = 0; i < NUMBER_OF_LINES-1; i++)
			{

				sscanf(str.c_str(), fetch.c_str(), &vertices2[iterator], &vertices2[iterator + 1], &vertices2[iterator + 2]);
				

				if (vertices2[iterator + 2] > maxZ)
					maxZ = vertices2[iterator + 2];
				if (vertices2[iterator + 2] < minZ)
					minZ = vertices2[iterator + 2];

				if (vertices2[iterator] == firstX)
				{
					multiplierX = 0;
				}
				vertices2[iterator] = OFFSETX * multiplierX++;
				if (iterator>3 && (firstY != vertices2[iterator+1])) {
					firstY = vertices2[iterator + 1];
					multiplierY++;
					if (multiplierY > 1) {
						for (int j = i; j > (i - 200); j--)
						{
							elements2[iterator2] = j - 200;
							elements2[iterator2+1] = j - 199;
							elements2[iterator2+2] = j;
							iterator2 += DIMENSION;

							elements2[iterator2] = j - 199;
							elements2[iterator2 + 1] = j;
							elements2[iterator2 + 2] = j+1;
							iterator2 += DIMENSION;

						}
					}

				}

				vertices2[iterator + 1] = OFFSETY * multiplierY;
				getline(file, str);

				if (vertices2[iterator] > maxX)
					maxX = vertices2[iterator];
				if (vertices2[iterator] < minX)
					minX = vertices2[iterator];


				if (vertices2[iterator + 1] > maxY)
					maxY = vertices2[iterator + 1];
				if (vertices2[iterator + 1] < minY)
					minY = vertices2[iterator + 1];
				
				iterator += DIMENSION;
				/*if (i < DIMENSION)
					elements2[i] = i;
				else
				{
					
					elements2[iterator2] = i - 2;
					elements2[iterator2+1] = i - 1;
					elements2[iterator2 + 2] = i;
					iterator2 += DIMENSION;


				}*/
				
			}
			
			file.close();
			return 1;
		}
		else 
			return 0;
		
}


/*###############################################################*/
void mysz(int button, int state, int x, int y)
{
	mbutton = button;
	switch (state)
	{
	case GLUT_UP:
		break;
	case GLUT_DOWN:
		pozycjaMyszyX = x;
		pozycjaMyszyY = y;
		poprzednie_kameraX = kameraX;
		poprzednie_kameraZ = kameraZ;
		poprzednie_kameraD = kameraD;
		break;

	}
}
/*******************************************/
void mysz_ruch(int x, int y)
{
	if (mbutton == GLUT_LEFT_BUTTON)
	{
		kameraX = poprzednie_kameraX - (pozycjaMyszyX - x) * 0.1;
		kameraZ = poprzednie_kameraZ - (pozycjaMyszyY - y) * 0.1;
	}
	if (mbutton == GLUT_RIGHT_BUTTON)
	{
		kameraD = poprzednie_kameraD + (pozycjaMyszyY - y) * 0.1;
	}

}
/******************************************/


void klawisz(GLubyte key, int x, int y)
{
	switch (key) {

	case 27:    /* Esc - koniec */
		exit(1);
		break;

	case '1':
		lightPos[0] -= maxX/10;
		break;
	case '2':
		lightPos[0] += maxX/10;
		break;
	case '3':
		lightPos[1] -= maxY/10;
		break;
	case '4':
		lightPos[1] += maxY/10;
		break;

	case '5':
		lightPos[2] -= maxZ/10;
		break;
	case '6':
		lightPos[2] += maxZ/10;
		break;
	case '+':
		strengthOfLight += 0.1;
		if (strengthOfLight > 1)
			strengthOfLight = 1;
		break;
	case '-':
		strengthOfLight -= 0.1;
		if (strengthOfLight < -1)
			strengthOfLight =-1;
		break;
	}


}
/*###############################################################*/

void rozmiar(int width, int height)
{
	screen_width = width;
	screen_height = height;

	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, screen_width, screen_height);

	P = glm::perspective(glm::radians(60.0f), (GLfloat)screen_width / (GLfloat)screen_height, 1.0f, 1000.0f);

	glutPostRedisplay(); // Przerysowanie sceny
}

/*###############################################################*/
void idle()
{

	glutPostRedisplay();
}

/*###############################################################*/

GLfloat ad = 0.0;

void timer(int value) {


	glutTimerFunc(20, timer, 0);
}
/*###############################################################*/