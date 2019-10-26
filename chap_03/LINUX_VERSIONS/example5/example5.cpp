//
// Display a color cube
//
// Colors are assigned to each vertex and then the rasterizer interpolates
//   those colors across the triangles.  We us an orthographic projection
//   as the default projetion.
// Add translating , scaling and rotating function.
// 再增加纹理贴图
// 增加bmp文件贴图

#include "Angel.h"
#include "stb_image.h"

typedef Angel::vec4  color4;
typedef Angel::vec4  point4;

const int NumTriangles = 12;// 6面 , 每面2个三角形
const int NumVertices = 3 * NumTriangles;
const int TexturesSize = 64;

//纹理对象(2个) textures[0]为黑白棋盘纹理 [1]为jpg图像纹理
//与 存纹理对象0的数组
GLuint textures[2];
GLubyte image[TexturesSize][TexturesSize][3];

//存顶点数据的数组
point4 points[NumVertices];
color4 colors[NumVertices];
vec2 tex_coords[NumVertices];

// Vertices of a unit cube centered at origin, sides aligned with axes
point4 vertices[8] = {
        point4( -0.5, -0.5,  0.5, 1.0 ),
        point4( -0.5,  0.5,  0.5, 1.0 ),
        point4(  0.5,  0.5,  0.5, 1.0 ),
        point4(  0.5, -0.5,  0.5, 1.0 ),
        point4( -0.5, -0.5, -0.5, 1.0 ),
        point4( -0.5,  0.5, -0.5, 1.0 ),
        point4(  0.5,  0.5, -0.5, 1.0 ),
        point4(  0.5, -0.5, -0.5, 1.0 )
};

// RGBA colors
color4 vertex_colors[8] = {
        color4( 0.0, 0.0, 0.0, 1.0 ),  // black
        color4( 1.0, 0.0, 0.0, 1.0 ),  // red
        color4( 1.0, 1.0, 0.0, 1.0 ),  // yellow
        color4( 0.0, 1.0, 0.0, 1.0 ),  // green
        color4( 0.0, 0.0, 1.0, 1.0 ),  // blue
        color4( 1.0, 0.0, 1.0, 1.0 ),  // magenta
        color4( 1.0, 1.0, 1.0, 1.0 ),  // white
        color4( 0.0, 1.0, 1.0, 1.0 )   // cyan
};

// Array of rotation angles (in degrees) for each coordinate axis
// 旋转增量
enum { Xaxis = 0, Yaxis = 1, Zaxis = 2, NumAxes = 3 };
int      Axis = Xaxis;
GLfloat  Theta[NumAxes] = { 0.0, 0.0, 0.0 };
GLfloat  incrementTheta = 0.5;

// 平移增量 与 缩放增量
GLfloat  incrementXYZ[NumAxes] = { 0.0, 0.0, 0.0 };
GLfloat  incrementScale = 1.0;

//----------------------------------------------------------------------------



//----------------------------------------------------------------------------

// quad generates two triangles for each face and assigns colors
//    to the vertices
//  quad 函数为立方体一个面生成两个三角型，并分配颜色，同时分配贴图坐标
int Index = 0;

void
quad( int a, int b, int c, int d )
{
    colors[Index] = vertex_colors[a]; points[Index] = vertices[a];
    tex_coords[Index] = vec2(0.0 , 0.0);
    Index++;

    colors[Index] = vertex_colors[b]; points[Index] = vertices[b];
    tex_coords[Index] = vec2(1.0 , 0.0);
    Index++;

    colors[Index] = vertex_colors[c]; points[Index] = vertices[c];
    tex_coords[Index] = vec2(1.0 , 1.0);
    Index++;

    colors[Index] = vertex_colors[a]; points[Index] = vertices[a];
    tex_coords[Index] = vec2(0.0 , 0.0);
    Index++;

    colors[Index] = vertex_colors[c]; points[Index] = vertices[c];
    tex_coords[Index] = vec2(1.0 , 1.0);
    Index++;

    colors[Index] = vertex_colors[d]; points[Index] = vertices[d];
    tex_coords[Index] = vec2(0.0 , 1.0);
    Index++;
}

//----------------------------------------------------------------------------

// generate 12 triangles: 36 vertices and 36 colors
// 对应6个面
void
colorcube()
{
    quad( 1, 0, 3, 2 );
    quad( 2, 3, 7, 6 );
    quad( 3, 0, 4, 7 );
    quad( 6, 5, 1, 2 );
    quad( 4, 5, 6, 7 );
    quad( 5, 4, 0, 1 );
}

//----------------------------------------------------------------------------

// OpenGL initialization
void
init()
{
    colorcube();

    // 生成一个贴图图案
    for(int i=0;i<TexturesSize;++i){
        for(int j = 0;j<TexturesSize;++j){
            GLubyte c = (((i & 0x8) == 0) ^ ((j & 0x8) == 0)) * 255;
            image[i][j][0] = c;
            image[i][j][1] = c;
            image[i][j][2] = c;
        }
    }

    // 初始化纹理对象
    glGenTextures( 2 , textures);

    // 纹理对象[0]
    glBindTexture( GL_TEXTURE_2D, textures[0] );
    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, TexturesSize, TexturesSize, 0,
                  GL_RGB, GL_UNSIGNED_BYTE, image );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );

    // 纹理对象[1]
    glBindTexture( GL_TEXTURE_2D, textures[1] );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

    int width, height, nrChannels;
    unsigned char *data = stbi_load("whu.jpg", &width,
                                    &height, &nrChannels,0);
    if(data){
        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
                        GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap( GL_TEXTURE_2D );
    }else{
        std::cout << "Failed to load texture from whu.jpg " << std::endl;
    }
    stbi_image_free(data);

    //LoadGL

    glActiveTexture( GL_TEXTURE0 );
    glBindTexture( GL_TEXTURE_2D, textures[0] );

    // Create a vertex array object
    GLuint vao;
    glGenVertexArrays( 1, &vao );
    glBindVertexArray( vao );

    // Create and initialize a buffer object
    GLuint buffer;
    glGenBuffers( 1, &buffer );
    glBindBuffer( GL_ARRAY_BUFFER, buffer );
    glBufferData( GL_ARRAY_BUFFER,
                  sizeof(points) + sizeof(colors) + sizeof(tex_coords),
                  NULL, GL_STATIC_DRAW );

    // 分配一个偏移量 ， 以便记录数据在顶点数据缓冲区的位置
    GLuint offset = 0;
    glBufferSubData( GL_ARRAY_BUFFER, offset, sizeof(points), points );
    offset += sizeof(points);

    glBufferSubData( GL_ARRAY_BUFFER, offset, sizeof(colors), colors );
    offset += sizeof(colors);

    glBufferSubData( GL_ARRAY_BUFFER, offset, sizeof(tex_coords), tex_coords);

    // Load shaders and use the resulting shader program
    GLuint program = InitShader( "vshader35.glsl", "fshader35.glsl" );
    glUseProgram( program );

    // set up vertex arrays
    offset = 0;
    GLuint vPosition = glGetAttribLocation( program, "vPosition" );
    glEnableVertexAttribArray( vPosition );
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0,
                           BUFFER_OFFSET(offset) );
    offset += sizeof(points);

    GLuint vColor = glGetAttribLocation( program, "vColor" );
    glEnableVertexAttribArray( vColor );
    glVertexAttribPointer( vColor, 4, GL_FLOAT, GL_FALSE, 0,
                           BUFFER_OFFSET(offset) );
    offset += sizeof(colors);

    GLuint vTexCoord = glGetAttribLocation( program, "vTexCoord" );
    glEnableVertexAttribArray( vTexCoord );
    glVertexAttribPointer( vTexCoord, 2, GL_FLOAT, GL_FALSE, 0,
                           BUFFER_OFFSET(offset) );

    // Set the value of the fragment shader texture sampler variable
    //   ("texture") to the the appropriate texture unit. In this case,
    //   zero, for GL_TEXTURE0 which was previously set by calling
    //   glActiveTexture().
    // 将片元着色器中的纹理采样器变量("texture")的数值设为对应的纹理单元
    // 因之前有    glActiveTexture( GL_TEXTURE0 );   故将其设为0
    glUniform1i( glGetUniformLocation(program, "texture"), 0);

    glEnable( GL_DEPTH_TEST );
    glClearColor( 1.0, 1.0, 1.0, 1.0 );
}

//----------------------------------------------------------------------------

void
display( void )
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    mat4 transform ;  // diagional matrix
    //translation
    transform = transform * Translate(incrementXYZ[0],
                                      incrementXYZ[1],
                                      incrementXYZ[2]);
//    incrementXYZ[0] = incrementXYZ[1] = incrementXYZ[2] = 0.0;

    // rotation
    transform = transform *( RotateX( Theta[Xaxis] ) *
                             RotateY( Theta[Yaxis] ) *
                             RotateZ( Theta[Zaxis] ) );

    //scale
    transform = transform * Scale(incrementScale,
                                  incrementScale,
                                  incrementScale);
//    incrementScale = 1.0;

    point4  transformed_points[NumVertices];

    for ( int i = 0; i < NumVertices; ++i ) {
        transformed_points[i] = transform * points[i];
    }

    glBufferSubData( GL_ARRAY_BUFFER, 0, sizeof(transformed_points),
                     transformed_points );

    glDrawArrays( GL_TRIANGLES, 0, NumVertices );
    glutSwapBuffers();
}

//----------------------------------------------------------------------------

void
keyboard( unsigned char key, int x, int y )
{
    switch( key ) {
        case 033: // Escape Key
        case 'q': case 'Q':
            exit( EXIT_SUCCESS );
            break;
        case 'p': case 'P':// Pause rotation
            incrementTheta = 0.0;
            break;
        case 'b': case 'B':// Begin rotation
            incrementTheta = 0.5;
            break;
        case 'w': case 'W':// Up translation
            incrementXYZ[1] += 0.005;
            break;
        case 's': case 'S':// Down translation
            incrementXYZ[1] += -0.005;
            break;
        case 'a': case 'A':// Left translation
            incrementXYZ[0] += -0.005;
            break;
        case 'd': case 'D':// Right translation
            incrementXYZ[0] += 0.005;
            break;
        case 'f': case 'F':// Zoom in
            incrementScale  *= 1.01;
            break;
        case 'g': case 'G':// Zoom out
            incrementScale  *= 0.99;
            break;
        case '0':
            glBindTexture( GL_TEXTURE_2D, textures[0]);
            break;
        case '1':
            glBindTexture( GL_TEXTURE_2D, textures[1]);
            break;
    }
}

//----------------------------------------------------------------------------

void
mouse( int button, int state, int x, int y )
{
    if ( state == GLUT_DOWN ) {
        switch( button ) {
            case GLUT_LEFT_BUTTON:    Axis = Xaxis;  break;
            case GLUT_MIDDLE_BUTTON:  Axis = Yaxis;  break;
            case GLUT_RIGHT_BUTTON:   Axis = Zaxis;  break;
        }
    }
}

//----------------------------------------------------------------------------

void
idle( void )
{
    Theta[Axis] += incrementTheta;

    if ( Theta[Axis] > 360.0 ) {
        Theta[Axis] -= 360.0;
    }

    glutPostRedisplay();
}

//----------------------------------------------------------------------------

int
main( int argc, char **argv )
{
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
    glutInitWindowSize( 512, 512 );
    glutInitContextVersion( 3, 2 );
    glutInitContextProfile( GLUT_CORE_PROFILE );
    glutCreateWindow( "Color Cube" );

    glewInit();

    init();

    glutDisplayFunc( display );
    glutKeyboardFunc( keyboard );
    glutMouseFunc( mouse );
    glutIdleFunc( idle );

    glutMainLoop();
    return 0;
}
