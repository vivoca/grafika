/*
 * Grafika beadando 2023 - Gorbe rajzolas Lagrange polinom hasznalataval
 *
 * Egyud Vivien - C11M1L
 *
 * A program celja 2D görbe kirajzolasa pontokra Lagrange polinom segitsegevel.
 *
 * A program alapvető interrakciora kepes a lentebb irtak alapjan
 * Kilepes : 0
 * Demo elore beallitott pontokkal : 1
 * Gorbe rajzolas sajat pontokra : 2
 */

//#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
//#include <GLUT/glut.h>
//#else
//#include <GL/glut.h>
//#endif
#include <GLUT/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#define PMSIZE 200

struct Data {
    float x, y;
};
using namespace std;

int customCount = 0;
Data customData[25];
int numberOfPoints = 5;
GLfloat controlPoints[PMSIZE][3];
GLfloat curvePoints[PMSIZE][3];


/*
 *  Kiszamolja Lagrange-polinom segitsegevel az f(xi)-t
 */
float interpolate(Data function[], float xi, int n) {
    float result = 0;
    for (int i = 0; i < n; i++) {
        float term = function[i].y;
        for (int j = 0; j < n; j++) {
            if (j != i) {
                term = term * (xi - function[j].x) / float(function[i].x - function[j].x);
            }
        }
        result += term;
    }
    return result;
}

/*
 *  Feltolti a pontok és az illeszkedo görbe adatait tartalmazo tombot
 */
void generatePoints(Data function[], int n) {
    for (int i = 0; i <= n; i++) {
        controlPoints[i][0] = function[i].x / 10;
        controlPoints[i][1] = function[i].y / 10;
    }

    float xi = -1.0;
    for (int i = 0; i < 200; i++) {
        curvePoints[i][0] = xi;
        float yi = interpolate(function, xi * 10, n);
        curvePoints[i][1] = yi / 10;
        xi = xi + 0.01;
    }
}

/*
 *  Eldonti hogy az elore betoltott demo vagy a felhasznalo megadott pontjaira rajzol
 */
void generatePointsBase(bool customData, int numberOfCustomPoints, Data customPoints[]) {
    if (customData) {
        numberOfPoints = numberOfCustomPoints;
        generatePoints(customPoints, numberOfPoints);
    } else {
        Data function[] = {{-9, -8},
                           {-6, 0},
                           {-3, -1},
                           {2,  4},
                           {8,  -3}};
        numberOfPoints = 5;
        generatePoints(function, numberOfPoints);
    }

}


void psInit() {
    glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
}

static void psDisplay() {
    glClear(GL_COLOR_BUFFER_BIT);

    //======================================================
    /*
     * Felrajzoljuk a koordinata rendszert
     */
    glLineWidth(0.2);
    glColor3f(0.5, 0.5, 0.5);
    glBegin(GL_LINE_STRIP);
    glVertex3f(-1.0, 0.0, 0.0);
    glVertex3f(1.0, 0.0, 0.0);
    glEnd();
    glLineWidth(0.2);
    glColor3f(0.5, 0.5, 0.5);
    glBegin(GL_LINE_STRIP);
    glVertex3f(0.0, -1.0, 0.0);
    glVertex3f(0.0, 1.0, 0.0);
    glEnd();


    /*
     *  Megjelenitjuk a pontmatrix elemeit
     */
    glPointSize(6);
    glColor3f(0.0, 1.0, 0.0);
    glBegin(GL_POINTS);
    for (int i = 0; i < numberOfPoints; i++) {
        glVertex3fv((GLfloat *) &controlPoints[i][0]);
    }
    glEnd();


    /*
     *  A megadott pontokbol kiszamoljuk és abrazoljuk a fuggveny tobbi pontjat is
     */
    glLineWidth(0.2);
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < 200; i++) {
        glVertex3f(curvePoints[i][0], curvePoints[i][1], 0.0);
    }
    glEnd();
    glFinish();
}


static void psKey(unsigned char key, int x, int y) {
    switch (key) {
        case '0': {
            exit(0);
            break;
        }
        case '1': {
            // Demo
            generatePointsBase(false, 0, 0);
            break;
        }
        case '2': {
            // Sajat pontok
            if (customCount != 0) {
                generatePointsBase(true, customCount, customData);
            } else {
                cout << "\nHiba: Nincsenek meghatarozva sajat pontok!";
            }
            break;
        }
    }
    glutPostRedisplay();
}

void printMessage() {
    printf("Grafika beadando 2023 - Gorbe rajzolas Lagrange polinom hasznalataval\n\r");
    printf("Egyud Vivien - C11M1L\n\r");
    for (int k = 0; k < 51; k++) {
        printf("*");
    }
    printf("\n\r * 0 - Kilepes");
    printf("\n\r * 1 - Demo elore beallitott pontokkal");
    printf("\n\r * 2 - Gorbe rajzolas sajat pontokra\n");
    for (int k = 0; k < 51; k++) {
        printf("*");
    }
    printf("\n");
}


int main(int argc, char *argv[]) {
    printMessage();

    int x;
    cout << "Valassz az opciok kozul: ";
    cin >> x;

    switch (x) {
        case 0:
            return EXIT_SUCCESS;
        case 1:
            cout << "\nDemo pontokra rajzolas: ";
            cout << "\n{ {-9, -8}, {-6, 0}, {-3, -1}, {2, 4}, {8, -3} }";
            generatePointsBase(false, 0, 0);
            break;
        case 2:
            // todo ellenőrizni hogy 25-nél ne legyen több pont
            // todo ellenőrizni hogy 10?nél ne legyen nagyobb? ezt meg kell nézni
            // todo egy x csak egyszer szerepelhet
            cout << "\nMennyi pontra rajzoljon gorbet? (Max 25) ";
            cin >> customCount;
            cout << "\nAdd meg a szamokat enterrel elvalasztva (x1 y1 x2 y2 ... sorrendben) \n";
            cout
                    << "\nA tengelyek -10 es 10 kozott latszodnak, az ennel kisebb illetve nagyobb pontok nem latszodnak az ablakban! \n";
            for (int i = 0; i < customCount; i++) {
                int inputX;
                cout << "x" << i + 1 << ": ";
                cin >> inputX;
                customData[i].x = inputX;
                int inputY;
                cout << "y" << i + 1 << ": ";
                cin >> inputY;
                customData[i].y = inputY;
            }
            generatePointsBase(true, customCount, customData);
            break;
    }

    glutInit(&argc, argv);
    glutInitWindowSize(1024, 768);
    glutInitWindowPosition(250, 250);
    glutInitDisplayMode(GLUT_RGB);

    glutCreateWindow("me-grafika-beadando");
    psInit();
    glutDisplayFunc(psDisplay);
    glutKeyboardFunc(psKey);

    glutMainLoop();

    return EXIT_SUCCESS;
}


