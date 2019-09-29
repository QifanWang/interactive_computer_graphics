/* sierpinski gasket with vertex arrays */

#include "Angel.h"

using namespace std;
using namespace Angel;

float angle = 0.001f * DegreesToRadians;
vec2 points[3];
//vec2 points[3];
int Index = 0;
int count = 0;
int Window_w , Window_h;
//----------------------------------------------------------------------------

void
init( void )
{
    vec2 vertices[3] = {
            vec2( -1.0, -1.0 ), vec2( 0.0, 1.0 ), vec2( 1.0, -1.0 )
    };
    for(int i = 0 ;i<3;++i)
        points[i] = vertices[i];
    // Subdivide the original triangle
//    divide_triangle( vertices[0], vertices[1], vertices[2],
//                     NumTimesToSubdivide );

    // Create a vertex array object
    GLuint vao;
    glGenVertexArrays( 1, &vao );
    glBindVertexArray( vao );

    // Create and initialize a buffer object
    GLuint buffer;
    glGenBuffers( 1, &buffer );
    glBindBuffer( GL_ARRAY_BUFFER, buffer );
    glBufferData( GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW );

    // Load shaders and use the resulting shader program
    GLuint program = InitShader( "vshader24c.glsl", "fshader24c.glsl" );
    glUseProgram( program );

    // Initialize the vertex position attribute from the vertex shader
    GLuint loc = glGetAttribLocation( program, "vPosition" );
    glEnableVertexAttribArray( loc );
    glVertexAttribPointer( loc, 2, GL_FLOAT, GL_FALSE, 0,
                           BUFFER_OFFSET(0) );

    glClearColor( 1.0, 1.0, 1.0, 1.0 ); /* white background */
}

//----------------------------------------------------------------------------

void
display( void )
{
    for (int i = 0; i < 3; ++i) {
        float x = cos(angle)*points[i].x - sin(angle)*points[i].y;
        float y = sin(angle)*points[i].x + cos(angle)*points[i].y;
        points[i].x = x;
        points[i].y = y;
    }
    glBufferData(GL_ARRAY_BUFFER, sizeof(points),points,
            GL_STATIC_DRAW);
    glClear( GL_COLOR_BUFFER_BIT );
    glDrawArrays( GL_TRIANGLES, 0,3);
    glFlush();
}

//----------------------------------------------------------------------------

void
keyboard( unsigned char key, int x, int y )
{
    switch ( key ) {
        case 033:
            exit( EXIT_SUCCESS );
            break;
    }
}


//----------------------------------------------------------------------------

void idle(){
    glutPostRedisplay();
}

//----------------------------------------------------------------------------

void
myMouse( int button, int state, int x, int y )
{
    if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN){
        exit(0);
    }

    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
        glutIdleFunc(nullptr);
        points[count].x = (float) x / (Window_w/2) - 1.0;
        points[count].y = (float) (Window_h-y) / (Window_h/2) - 1.0;
//        points[NumVertices+count].x = (float) x - 1.0;
//        points[NumVertices+count].y = (float) (Window_h-y) - 1.0;
        count ++;
    }

    if(count == 3){
        glutIdleFunc(idle);
        glutPostRedisplay();
        count = 0;
    }
}

//----------------------------------------------------------------------------`


int
main( int argc, char **argv )
{
    Window_w = 512;
    Window_h = 512;

    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_RGBA );
    glutInitWindowSize( Window_w, Window_h );
    glutInitContextVersion( 3, 2 );
    glutInitContextProfile( GLUT_CORE_PROFILE );
    glutCreateWindow( "Simple GLSL example" );

    glewInit();
	
    init();
//
    glutDisplayFunc( display );
    glutKeyboardFunc( keyboard );
    glutMouseFunc(myMouse);

    glutMainLoop();
    return 0;
}
