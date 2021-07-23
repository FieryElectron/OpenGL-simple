#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <sstream>
#include <chrono>
#include <cstddef>

using namespace std;
using namespace glm;

struct Vertex
{
    vec3 pos;
    vec3 color;
};

class Frame
{
public:
    vector<Vertex> verts;
    GLuint vbo;
};

vector<Frame *> Frames;

void init()
{
    Frame *f = new Frame();
    for (float i = -1; i < 1; i += 0.1)
    {
        for (float j = -1; j < 1; j += 0.1)
        {
            for (float k = -1; k < 0; k += 0.1)
            {
                Vertex vert;
                vert.pos = vec3(i, j, k);
                vert.color = vec3(0, 1, 0);
                f->verts.push_back(vert);
            }
        }
    }

    glGenBuffers(1, &f->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, f->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * f->verts.size(), f->verts.data(), GL_STREAM_DRAW);

    Frames.push_back(f);
}

int nowIndex = 0;

void display()
{

    static std::chrono::steady_clock::time_point prv = std::chrono::steady_clock::now();
    std::chrono::steady_clock::time_point cur = std::chrono::steady_clock::now();
    const float dt = std::chrono::duration<float>(cur - prv).count();
    prv = cur;

    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    double w = glutGet(GLUT_WINDOW_WIDTH);
    double h = glutGet(GLUT_WINDOW_HEIGHT);
    gluPerspective(60.0, w / h, 0.1, 10.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(2, 2, 2, 0, 0, 0, 0, 0, 1);

    for (int i = nowIndex; i < Frames.size(); ++i)
    {
        glBindBuffer(GL_ARRAY_BUFFER, Frames[i]->vbo);
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);
        glVertexPointer(3, GL_FLOAT, sizeof(Vertex), (void *)offsetof(Vertex, pos));
        glColorPointer(3, GL_FLOAT, sizeof(Vertex), (void *)offsetof(Vertex, color));
        glDrawArrays(GL_POINTS, 0, Frames[i]->verts.size());
        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_COLOR_ARRAY);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        nowIndex = i;
    }

    std::stringstream msg;
    msg << "Frame time: " << (dt * 1000.0f) << " ms";
    glColor3ub(255, 255, 0);
    glWindowPos2i(10, 25);
    glutBitmapString(GLUT_BITMAP_9_BY_15, (unsigned const char *)(msg.str().c_str()));
    glutSwapBuffers();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
    glutInitWindowSize(600, 600);
    glutCreateWindow("GLUT");
    glewInit();
    init();
    glutIdleFunc(display);
    glutMainLoop();
    return 0;
}
