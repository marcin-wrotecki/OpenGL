#pragma once
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <iostream>]
#include <regex>
#include <exception>
#include <fstream>
#include "GL\glew.h"
#include "GL\freeglut.h"

#include "shaderLoader.h" //narzŕdzie do │adowania i kompilowania shaderˇw z pliku


//funkcje algebry liniowej
#include "glm/vec3.hpp" // glm::vec3
#include "glm/vec4.hpp" // glm::vec4
#include "glm/mat4x4.hpp" // glm::mat4
#include "glm/gtc/matrix_transform.hpp" // glm::translate, glm::rotate, glm::scale, glm::perspective

//Some function are from template from my teacher (these in polish language) !!!!!!!!

#define PI 3.14159265359
#define MULTIPLIER 1.2 // it is related to drawing network
#define NUMBER_OF_PARAMETERS 4 //I use this constant because one of assumption of my task
//was to ignore parameters which define colour 
#define DIMENSIONS 3

int Load(std::string filename);
void klawisz(GLubyte key, int x, int y);
void mysz(int button, int state, int x, int y);
void mysz_ruch(int x, int y);
void idle();
void timer(int value);
void rozmiar(int width, int height);
void createAxisAndNetwork();




//Wymiary okna
int screen_width = 640;
int screen_height = 480;

float dx = 0;
float dz = 1.0;


int pozycjaMyszyX; // na ekranie
int pozycjaMyszyY;
int mbutton; // wcisiety klawisz myszy

double kameraX = 10.0;
double kameraZ = 10.0;
double kameraD = -10.0;
double kameraPredkosc;
double kameraKat = 20;
double kameraPredkoscObrotu;
double poprzednie_kameraX;
double poprzednie_kameraZ;
double poprzednie_kameraD;

double rotation = 0;
//macierze
glm::mat4 MV; //modelview - macierz modelu i świata
glm::mat4 P;  //projection - macierz projekcji, czyli naszej perspektywy


float* vertex_indices;
int *face_elements;
int sizeOfFaceElements;
int sizeOfVertexIndices;
float maxX = INT_MIN, minX = INT32_MAX;
float maxY = INT_MIN, minY = INT32_MAX;
float maxZ = INT_MIN, minZ = INT32_MAX;
int figure;

float axis_vertex[12] = { 0 };
float network_vertex[200] = {0};

GLuint elementsOfAxis[]{
0,1,
1,2,
2,3
};
GLuint elementsOfNetwork[] = {
0,1,
2,3,
4,5,
6,7,
8,9,
10,11,
12,13,
1,14,
3,15,
5,16,
7,17,
9,18,
11,19,
13,20,
21,22,
23,24,
25,26,
27,28,
29,30,
31,32,
22,33,
24,34,
26,35,
28,36,
30,37,
32,38,
39,40,
41,42,
43,44,
45,46,
47,48,
49,50,
40,51,
42,52,
44,53,
46,54,
48,55,
50,56
};


std::string sizeOfElementsVertex;
std::string sizeOfElementsFaces;

//shaders
GLuint programID = 0;
GLuint programID2 = 0;


unsigned int VBO, VBO2,VBO3;
unsigned int VAO[3];




/*###############################################################*/
void rysuj(void)
{

	//GLfloat color[] = { 1.0f, 0.0f, 0.0f, 1.0f };
	//glClearBufferfv(GL_COLOR, 0, color);
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Kasowanie ekranu

	glUseProgram(programID); //u┐yj programu, czyli naszego shadera  

	MV = glm::mat4(1.0f);  //macierz jednostkowa
	MV = glm::translate(MV, glm::vec3(dx, 0, kameraD) - dz);
	MV = glm::rotate(MV, (float)glm::radians(kameraZ), glm::vec3(1, 0, 0));
	MV = glm::rotate(MV, (float)glm::radians(kameraX), glm::vec3(0, 1, 0));
	if (((maxX - minX) > 1000 || maxY -minY)> 1000 || (maxZ - minZ) > 1000)
	{
		MV = glm::scale(MV, glm::vec3(0.1f,0.1f,0.1f)); //aby uniknac zanikania obiektu przy oddalaniu go oraz dla latwiejszego ogladania go
	}
	glm::mat4 MVP = P * MV;

	/*Zmienne   jednorodne   (ang. uniform variable), zwane  także  zmiennym  globalnymi,sązmiennymi,
		których  wartośc  jest  stała  w  obrębie  obiektu programu. Shadery  mogą zmienne jednorodne tylko  odczytywac */

	GLuint MVP_id = glGetUniformLocation(programID, "MVP"); // pobierz lokalizację zmiennej 'uniform' "MV" w programie
	glUniformMatrix4fv(MVP_id, 1, GL_FALSE, &(MVP[0][0]));     // wyślij tablicę mv do lokalizacji "MV", która jest typu mat4   


	
	if (figure == 3) {
		glVertexAttrib3f(1, 0, 0, 0);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glBindVertexArray(VAO[0]);
		glDrawElements(GL_TRIANGLES, stoi(sizeOfElementsFaces) * figure, GL_UNSIGNED_INT, 0);
	}
	else if (figure == 4)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glVertexAttrib3f(1, 0, 0, 0);
		glBindVertexArray(VAO[0]);
		glDrawElements(GL_QUADS, stoi(sizeOfElementsFaces) * figure, GL_UNSIGNED_INT, 0);

	}
	
		glVertexAttrib1f(3, minZ);
		glVertexAttrib1f(4, maxZ);
	
	glUseProgram(programID2); //u┐yj programu, czyli naszego shadera  
	 MVP_id = glGetUniformLocation(programID2, "MVP"); // pobierz lokalizację zmiennej 'uniform' "MV" w programie
	 if (figure == 3) {
		 glUniformMatrix4fv(MVP_id, 1, GL_FALSE, &(MVP[0][0]));
		 glVertexAttrib3f(1, 0, 0, 0);
		 glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		 glBindVertexArray(VAO[0]);
		 glDrawElements(GL_TRIANGLES, stoi(sizeOfElementsFaces) * figure, GL_UNSIGNED_INT, 0);
	 }
	 else if (figure == 4) {
		 glUniformMatrix4fv(MVP_id, 1, GL_FALSE, &(MVP[0][0]));
		 glVertexAttrib3f(1, 0, 0, 0);
		 glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		 glBindVertexArray(VAO[0]);
		 glDrawElements(GL_QUADS, stoi(sizeOfElementsFaces) * figure, GL_UNSIGNED_INT, 0);
	 }
	glVertexAttrib3f(1, 0, 0, 0);
	 glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	 glBindVertexArray(VAO[1]);
	 glLineStipple(1, 0x3F07);
	 glEnable(GL_LINE_STIPPLE);
	 glDrawElements(GL_LINES, sizeof(elementsOfNetwork), GL_UNSIGNED_INT, 0);
	 glDisable(GL_LINE_STIPPLE);
	
	 glVertexAttrib3f(1, 0, 0, 0);
	 glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	 glBindVertexArray(VAO[2]);
	 glDrawElements(GL_LINES, sizeof(elementsOfAxis), GL_UNSIGNED_INT, 0);
	


	glFlush();
	glutSwapBuffers();

}

int main(int argc, char **argv)
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(screen_width, screen_height);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Przyklad 3");

	glewInit(); //init rozszerzeszeń OpenGL z biblioteki GLEW

	glutDisplayFunc(rysuj);         // def. funkcji rysuj¦cej
	glutIdleFunc(idle);         // def. funkcji rysuj¦cej w czasie wolnym procesoora (w efekcie: ci¦gle wykonywanej)
	glutTimerFunc(20, timer, 0);
	glutReshapeFunc(rozmiar); // def. obs-ugi zdarzenia resize (GLUT)

	glutKeyboardFunc(klawisz);      // def. obsługi klawiatury
	glutMouseFunc(mysz);        // def. obsługi zdarzenia przycisku myszy (GLUT)
	glutMotionFunc(mysz_ruch); // def. obsługi zdarzenia ruchu myszy (GLUT)


	glEnable(GL_DEPTH_TEST);

	//glPointSize(3.0f);



	/* úadowanie shadera, tworzenie programu *************************/
	/* i linkowanie go oraz sprawdzanie b│ŕdˇw! **********************/
	programID = loadShaders("vertex_shader.glsl", "fragment_shader.glsl");
	programID2 = loadShaders("vertex_shader.glsl", "fragment_shader2.glsl");
	std::string fileName;
	printf("Prosze podac wzgledna sciezke do pliku .ply (np. examples/porsche.ply) \n");
	std::cin >> fileName;
	Load(fileName);

	createAxisAndNetwork();
	GLuint ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int)*stoi(sizeOfElementsFaces) * figure, face_elements, GL_STATIC_DRAW); // uwzglednic w zmiennej czy trojkaty czy czworokaty

	GLuint ebo2;
	glGenBuffers(1, &ebo2);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elementsOfNetwork), elementsOfNetwork, GL_STATIC_DRAW);

	GLuint ebo3;
	glGenBuffers(1, &ebo3);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo3);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elementsOfAxis), elementsOfAxis, GL_STATIC_DRAW);

	glGenVertexArrays(3, VAO);

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * DIMENSIONS * stoi(sizeOfElementsVertex), vertex_indices, GL_STATIC_DRAW);

	glGenBuffers(1, &VBO2);
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(network_vertex), network_vertex, GL_STATIC_DRAW); //wyslanie na karte graficzna

	glGenBuffers(1, &VBO3);
	glBindBuffer(GL_ARRAY_BUFFER, VBO3);
	glBufferData(GL_ARRAY_BUFFER, sizeof(axis_vertex), axis_vertex, GL_STATIC_DRAW); //wyslanie na karte graficzna

	glBindVertexArray(VAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // VBO zawiera wierzcholki
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindVertexArray(VAO[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo2);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindVertexArray(VAO[2]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO3);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo3);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	kameraZ = 1.5*maxZ;
	kameraD = -3.5*abs((minY+maxY)/2);
	kameraX = 1.2*minX;
	glutMainLoop();                 // start
	
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &VBO2);
	glDeleteBuffers(1, &VBO3);

	free(face_elements);
	free(vertex_indices);
	return(0);
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
		dx += 0.5;
		break;
	case '2':
		dx -= 0.5;
		break;
	case '3':
		dz += 0.5;
		break;
	case '4':
		dz -= 0.5;
		break;
	}


}

/*###############################################################*/
void rozmiar(int width, int height)
{
	screen_width = width;
	screen_height = height;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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
GLfloat k = 0.05;
GLfloat ad = 0.0;

void timer(int value) {

	ad += k;

	if (ad > 1 || ad < 0)
		k = -k;

	//GLfloat attrib[] = { ad, 0.0f, 0.0f };
	//Aktualizacja wartości atrybutu wejściowego 1.
	//glVertexAttrib3fv(0, attrib);

	/*

	W vertex_shader np:
	layout (location = 1) in vec3 incolor;

	*/
	glutTimerFunc(20, timer, 0);
}
/*###############################################################*/



int Load(std::string filename)
{
	std::vector<std::string> header;
	int decider = filename.find(".ply"); //it should be file extension

	if (decider == (filename.length() - 4))
	{
		std::fstream file;
		file.open(filename, std::ios::in | std::ios::out);
		std::regex rx("element vertex [0-9]+.*");
		std::regex number("[0-9]+");
		std::smatch m;
		int dimension = 0;
		if (file.good() == true)
		{
			std::string str;
			getline(file, str);
			if (str != "ply")
			{
				file.close();
				return 0;
			}
			while (!std::regex_match(str, rx)) {
				getline(file, str);
			}

			try {
				std::regex_search(str, m, number);
				//std::ssub_match base_sub_match = m[0].first;
				sizeOfElementsVertex = m[0].first._Ptr;
				getline(file, str);
				while (str.find("face") == -1)
				{
					getline(file, str);
					dimension++;
				}
				if (dimension > DIMENSIONS)
					dimension = DIMENSIONS;
				sizeOfVertexIndices = stoi(sizeOfElementsVertex) * dimension;
				vertex_indices = (float*)calloc(sizeof(float), sizeOfVertexIndices);
				std::regex_search(str, m, number);
				sizeOfElementsFaces = m[0].first._Ptr;
				sizeOfFaceElements = stoi(sizeOfElementsFaces) * 5;
				face_elements = (int*)calloc(sizeof(int), sizeOfFaceElements);
				getline(file, str);
				while (str.find("end_header"))
					getline(file, str);
			}
			catch (std::invalid_argument ex) {
				file.close();
				return 0;
			}
			int iterator = 0;
			std::string fetch;
		
			for (int i = 0; i < dimension; i++)
				fetch += "%f ";
			int first = 0;
			for (int i = 0; i < stoi(sizeOfElementsVertex); i++)
			{
				getline(file, str);

				sscanf(str.c_str(), fetch.c_str(), &vertex_indices[iterator], &vertex_indices[iterator + 1], &vertex_indices[iterator + 2]);

				if (vertex_indices[iterator] > maxX)
					maxX = vertex_indices[iterator];
				if (vertex_indices[iterator] < minX)
					minX = vertex_indices[iterator];
				
				if (vertex_indices[iterator + 1] > maxY)
					maxY = vertex_indices[iterator + 1];
				if (vertex_indices[iterator + 1] < minY)
					minY = vertex_indices[iterator + 1];
				
				if (vertex_indices[iterator + 2] > maxZ)
					maxZ = vertex_indices[iterator + 2];
				if (vertex_indices[iterator + 2] < minZ)
					minZ = vertex_indices[iterator + 2];

				iterator += dimension;
			}
			iterator = 0;
			fetch = "%d ";
			for (int i = 0; i < NUMBER_OF_PARAMETERS; i++)
				fetch += "%d ";

			for (int i = 0; i < stoi(sizeOfElementsFaces); i++)
			{
				getline(file, str);

				sscanf(str.c_str(), fetch.c_str(), &figure, &face_elements[iterator], &face_elements[iterator + 1], &face_elements[iterator + 2], &face_elements[iterator + 3]);
				if (figure == 3) //when figure is 3 -> it is triangle
					iterator += 3;
				else if (figure == 4) //when figure is 4 -> it is rectangle 
					iterator += 4;
			}
			file.close();
		}
	}
}



void createAxisAndNetwork()
{
	axis_vertex[0] = MULTIPLIER * minX;
	axis_vertex[1] = MULTIPLIER * maxY;
	axis_vertex[2] = MULTIPLIER * maxZ;
	axis_vertex[3] = MULTIPLIER * minX;
	axis_vertex[4] = MULTIPLIER * maxY;
	axis_vertex[5] = MULTIPLIER * minZ;
	axis_vertex[6] = MULTIPLIER * minX;
	axis_vertex[7] = MULTIPLIER * minY;
	axis_vertex[8] = MULTIPLIER * minZ;
	axis_vertex[9] = MULTIPLIER * maxX;
	axis_vertex[10] = MULTIPLIER * minY;
	axis_vertex[11] = MULTIPLIER * minZ;

	//0
	network_vertex[0] = MULTIPLIER * minX;
	network_vertex[1] = MULTIPLIER * maxY;
	network_vertex[2] = MULTIPLIER * maxZ;
	//1
	network_vertex[3] = MULTIPLIER * maxX;
	network_vertex[4] = MULTIPLIER * maxY;
	network_vertex[5] = MULTIPLIER * maxZ;
	//2
	network_vertex[6] = MULTIPLIER * minX;
	network_vertex[7] = MULTIPLIER * maxY;
	network_vertex[8] = MULTIPLIER * maxZ - MULTIPLIER * (maxZ - minZ) / 6;
	//3
	network_vertex[9] = MULTIPLIER * maxX;
	network_vertex[10] = MULTIPLIER * maxY;
	network_vertex[11] = MULTIPLIER * maxZ - MULTIPLIER * (maxZ - minZ) / 6;
	//4
	network_vertex[12] = MULTIPLIER * minX;
	network_vertex[13] = MULTIPLIER * maxY;
	network_vertex[14] = MULTIPLIER * maxZ - MULTIPLIER * (maxZ - minZ) / 3;
	//5
	network_vertex[15] = MULTIPLIER * maxX;
	network_vertex[16] = MULTIPLIER * maxY;
	network_vertex[17] = MULTIPLIER * maxZ - MULTIPLIER * (maxZ - minZ) / 3;
	//6
	network_vertex[18] = MULTIPLIER * minX;
	network_vertex[19] = MULTIPLIER * maxY;
	network_vertex[20] = MULTIPLIER * maxZ - MULTIPLIER * (maxZ - minZ) / 2;
	//7
	network_vertex[21] = MULTIPLIER * maxX;
	network_vertex[22] = MULTIPLIER * maxY;
	network_vertex[23] = MULTIPLIER * maxZ - MULTIPLIER * (maxZ - minZ) / 2;
	//8
	network_vertex[24] = MULTIPLIER * minX;
	network_vertex[25] = MULTIPLIER * maxY;
	network_vertex[26] = MULTIPLIER * maxZ - MULTIPLIER * 2 * (maxZ - minZ) / 3;
	//9
	network_vertex[27] = MULTIPLIER * maxX;
	network_vertex[28] = MULTIPLIER * maxY;
	network_vertex[29] = MULTIPLIER * maxZ - MULTIPLIER * 2 * (maxZ - minZ) / 3;
	//10
	network_vertex[30] = MULTIPLIER * minX;
	network_vertex[31] = MULTIPLIER * maxY;
	network_vertex[32] = MULTIPLIER * maxZ - MULTIPLIER * 5 * (maxZ - minZ) / 6;
	//11
	network_vertex[33] = MULTIPLIER * maxX;
	network_vertex[34] = MULTIPLIER * maxY;
	network_vertex[35] = MULTIPLIER * maxZ - MULTIPLIER * 5 * (maxZ - minZ) / 6;
	//12
	network_vertex[36] = MULTIPLIER * minX;
	network_vertex[37] = MULTIPLIER * maxY;
	network_vertex[38] = MULTIPLIER * minZ;
	//13
	network_vertex[39] = MULTIPLIER * maxX;
	network_vertex[40] = MULTIPLIER * maxY;
	network_vertex[41] = MULTIPLIER * minZ;
	//14
	network_vertex[42] = MULTIPLIER * maxX;
	network_vertex[43] = MULTIPLIER * minY;
	network_vertex[44] = MULTIPLIER * maxZ;
	//15
	network_vertex[45] = MULTIPLIER * maxX;
	network_vertex[46] = MULTIPLIER * minY;
	network_vertex[47] = MULTIPLIER * maxZ - MULTIPLIER * (maxZ - minZ) / 6;
	//16
	network_vertex[48] = MULTIPLIER * maxX;
	network_vertex[49] = MULTIPLIER * minY;
	network_vertex[50] = MULTIPLIER * maxZ - MULTIPLIER * (maxZ - minZ) / 3;
	//17
	network_vertex[51] = MULTIPLIER * maxX;
	network_vertex[52] = MULTIPLIER * minY;
	network_vertex[53] = MULTIPLIER * maxZ - MULTIPLIER * (maxZ - minZ) / 2;
	//18
	network_vertex[54] = MULTIPLIER * maxX;
	network_vertex[55] = MULTIPLIER * minY;
	network_vertex[56] = MULTIPLIER * maxZ - 2 * MULTIPLIER*(maxZ - minZ) / 3;
	//19
	network_vertex[57] = MULTIPLIER * maxX;
	network_vertex[58] = MULTIPLIER * minY;
	network_vertex[59] = MULTIPLIER * maxZ - 5 * MULTIPLIER*(maxZ - minZ) / 6;
	//20
	network_vertex[60] = MULTIPLIER * maxX;
	network_vertex[61] = MULTIPLIER * minY;
	network_vertex[62] = MULTIPLIER * minZ;


	//21
	network_vertex[63] = MULTIPLIER * maxX - MULTIPLIER * (maxX - minX) / 6;
	network_vertex[64] = MULTIPLIER * maxY;
	network_vertex[65] = MULTIPLIER * maxZ;
	//22
	network_vertex[66] = MULTIPLIER * maxX - MULTIPLIER * (maxX - minX) / 6;
	network_vertex[67] = MULTIPLIER * maxY;
	network_vertex[68] = MULTIPLIER * minZ;
	//23
	network_vertex[69] = MULTIPLIER * maxX - MULTIPLIER * (maxX - minX) / 3;
	network_vertex[70] = MULTIPLIER * maxY;
	network_vertex[71] = MULTIPLIER * maxZ;
	//24
	network_vertex[72] = MULTIPLIER * maxX - MULTIPLIER * (maxX - minX) / 3;
	network_vertex[73] = MULTIPLIER * maxY;
	network_vertex[74] = MULTIPLIER * minZ;
	//25
	network_vertex[75] = MULTIPLIER * maxX - MULTIPLIER * (maxX - minX) / 2;
	network_vertex[76] = MULTIPLIER * maxY;
	network_vertex[77] = MULTIPLIER * maxZ;
	//26
	network_vertex[78] = MULTIPLIER * maxX - MULTIPLIER * (maxX - minX) / 2;
	network_vertex[79] = MULTIPLIER * maxY;
	network_vertex[80] = MULTIPLIER * minZ;
	//27
	network_vertex[81] = MULTIPLIER * maxX - MULTIPLIER * 2 * (maxX - minX) / 3;
	network_vertex[82] = MULTIPLIER * maxY;
	network_vertex[83] = MULTIPLIER * maxZ;
	//28
	network_vertex[84] = MULTIPLIER * maxX - MULTIPLIER * 2 * (maxX - minX) / 3;
	network_vertex[85] = MULTIPLIER * maxY;
	network_vertex[86] = MULTIPLIER * minZ;
	//29
	network_vertex[87] = MULTIPLIER * maxX - MULTIPLIER * 5 * (maxX - minX) / 6;
	network_vertex[88] = MULTIPLIER * maxY;
	network_vertex[89] = MULTIPLIER * maxZ;
	//30
	network_vertex[90] = MULTIPLIER * maxX - MULTIPLIER * 5 * (maxX - minX) / 6;
	network_vertex[91] = MULTIPLIER * maxY;
	network_vertex[92] = MULTIPLIER * minZ;
	//31
	network_vertex[93] = MULTIPLIER * maxX;
	network_vertex[94] = MULTIPLIER * maxY;
	network_vertex[95] = MULTIPLIER * maxZ;
	//32
	network_vertex[96] = MULTIPLIER * maxX;
	network_vertex[97] = MULTIPLIER * maxY;
	network_vertex[98] = MULTIPLIER * minZ;
	//33
	network_vertex[99] = MULTIPLIER * maxX - MULTIPLIER * (maxX - minX) / 6;
	network_vertex[100] = MULTIPLIER * minY;
	network_vertex[101] = MULTIPLIER * minZ;
	//34
	network_vertex[102] = MULTIPLIER * maxX - MULTIPLIER * (maxX - minX) / 3;
	network_vertex[103] = MULTIPLIER * minY;
	network_vertex[104] = MULTIPLIER * minZ;
	//35
	network_vertex[105] = MULTIPLIER * maxX - MULTIPLIER * (maxX - minX) / 2;
	network_vertex[106] = MULTIPLIER * minY;
	network_vertex[107] = MULTIPLIER * minZ;
	//36
	network_vertex[108] = MULTIPLIER * maxX - MULTIPLIER * 2 * (maxX - minX) / 3;
	network_vertex[109] = MULTIPLIER * minY;
	network_vertex[110] = MULTIPLIER * minZ;
	//37
	network_vertex[111] = MULTIPLIER * maxX - MULTIPLIER * 5 * (maxX - minX) / 6;
	network_vertex[112] = MULTIPLIER * minY;
	network_vertex[113] = MULTIPLIER * minZ;
	//38
	network_vertex[114] = MULTIPLIER * maxX;
	network_vertex[115] = MULTIPLIER * minY;
	network_vertex[116] = MULTIPLIER * minZ;







	//39
	network_vertex[117] = MULTIPLIER * maxX;
	network_vertex[118] = MULTIPLIER * maxY - MULTIPLIER * (maxY - minY) / 6;
	network_vertex[119] = MULTIPLIER * maxZ;
	//40
	network_vertex[120] = MULTIPLIER * maxX;
	network_vertex[121] = MULTIPLIER * maxY - MULTIPLIER * (maxY - minY) / 6;
	network_vertex[122] = MULTIPLIER * minZ;

	//41
	network_vertex[123] = MULTIPLIER * maxX;
	network_vertex[124] = MULTIPLIER * maxY - MULTIPLIER * (maxY - minY) / 3;
	network_vertex[125] = MULTIPLIER * maxZ;
	//42
	network_vertex[126] = MULTIPLIER * maxX;
	network_vertex[127] = MULTIPLIER * maxY - MULTIPLIER * (maxY - minY) / 3;
	network_vertex[128] = MULTIPLIER * minZ;

	//43
	network_vertex[129] = MULTIPLIER * maxX;
	network_vertex[130] = MULTIPLIER * maxY - MULTIPLIER * (maxY - minY) / 2;
	network_vertex[131] = MULTIPLIER * maxZ;
	//44
	network_vertex[132] = MULTIPLIER * maxX;
	network_vertex[133] = MULTIPLIER * maxY - MULTIPLIER * (maxY - minY) / 2;
	network_vertex[134] = MULTIPLIER * minZ;

	//45
	network_vertex[135] = MULTIPLIER * maxX;
	network_vertex[136] = MULTIPLIER * maxY - MULTIPLIER * 2 * (maxY - minY) / 3;
	network_vertex[137] = MULTIPLIER * maxZ;
	//46
	network_vertex[138] = MULTIPLIER * maxX;
	network_vertex[139] = MULTIPLIER * maxY - MULTIPLIER * 2 * (maxY - minY) / 3;
	network_vertex[140] = MULTIPLIER * minZ;

	//47
	network_vertex[141] = MULTIPLIER * maxX;
	network_vertex[142] = MULTIPLIER * maxY - MULTIPLIER * 5 * (maxY - minY) / 6;
	network_vertex[143] = MULTIPLIER * maxZ;
	//48
	network_vertex[144] = MULTIPLIER * maxX;
	network_vertex[145] = MULTIPLIER * maxY - MULTIPLIER * 5 * (maxY - minY) / 6;
	network_vertex[146] = MULTIPLIER * minZ;

	//49
	network_vertex[147] = MULTIPLIER * maxX;
	network_vertex[148] = MULTIPLIER * minY;
	network_vertex[149] = MULTIPLIER * maxZ;
	//50
	network_vertex[150] = MULTIPLIER * maxX;
	network_vertex[151] = MULTIPLIER * minY;
	network_vertex[152] = MULTIPLIER * minZ;

	//51
	network_vertex[153] = MULTIPLIER * minX;
	network_vertex[154] = MULTIPLIER * maxY - MULTIPLIER * (maxY - minY) / 6;
	network_vertex[155] = MULTIPLIER * minZ;

	//52
	network_vertex[156] = MULTIPLIER * minX;
	network_vertex[157] = MULTIPLIER * maxY - MULTIPLIER * (maxY - minY) / 3;
	network_vertex[158] = MULTIPLIER * minZ;

	//53
	network_vertex[159] = MULTIPLIER * minX;
	network_vertex[160] = MULTIPLIER * maxY - MULTIPLIER * (maxY - minY) / 2;
	network_vertex[161] = MULTIPLIER * minZ;

	//54
	network_vertex[162] = MULTIPLIER * minX;
	network_vertex[163] = MULTIPLIER * maxY - MULTIPLIER * 2 * (maxY - minY) / 3;
	network_vertex[164] = MULTIPLIER * minZ;

	//55
	network_vertex[165] = MULTIPLIER * minX;
	network_vertex[166] = MULTIPLIER * maxY - MULTIPLIER * 5 * (maxY - minY) / 6;
	network_vertex[167] = MULTIPLIER * minZ;

	//56
	network_vertex[168] = MULTIPLIER * minX;
	network_vertex[169] = MULTIPLIER * minY;
	network_vertex[170] = MULTIPLIER * minZ;

}












