#include <fstream>
#include "render_handler.h"
#include "ofMain.h"


RenderHandler::RenderHandler()
{
  initialized = false;
    texture = 0;
}

void RenderHandler::init(void)
{
  GLuint vertexShader = compileShader(GL_VERTEX_SHADER, ofToDataPath("shaders/gui.vert").c_str());
    cout << "vert " << endl;
    
  GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, ofToDataPath("shaders/gui.frag").c_str());
    cout << "frag " << endl;
    
  program = glCreateProgram();

  glAttachShader(program, vertexShader);
  glAttachShader(program, fragmentShader);
  glLinkProgram(program);
  glDetachShader(program, vertexShader);
  glDetachShader(program, fragmentShader);

  positionLoc = glGetAttribLocation(program, "position");

  float coords[] = {-1.0,-1.0,-1.0,1.0,1.0,-1.0,1.0,-1.0,-1.0,1.0,1.0,1.0};

  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(coords), coords, GL_STATIC_DRAW);
  glEnableVertexAttribArray(positionLoc);
  glVertexAttribPointer(positionLoc, 2, GL_FLOAT, GL_FALSE, 0, 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0);

  initialized = true;
}

void RenderHandler::draw(void)
{
  glUseProgram(program);
  glBindVertexArray(vao);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  glBindVertexArray(0);
  glUseProgram(0);
}

void RenderHandler::reshape(int w_, int h_)
{
  w = w_;
  h = h_;
    
    //cout << "asdflksadfjlkasdf" << endl;

    if (tex.getWidth() != w || tex.getHeight() != h){
        tex.allocate(w, h, GL_RGBA);
    }
}

bool RenderHandler::GetViewRect(CefRefPtr<CefBrowser> browser, CefRect &rect)
{

    //printf ("GetViewRect \n");
  rect = CefRect(0, 0, w, h);
    
    if (tex.getWidth() != w || tex.getHeight() != h){
        tex.allocate(w, h, GL_RGBA);
    }
  return true;
}

void RenderHandler::OnPaint(CefRefPtr<CefBrowser> browser,
                            PaintElementType type,
                            const RectList &dirtyRects,
                            const void* buffer,
                            int width,
                            int height){

    
    tex.loadData((unsigned char*) buffer, width, height, GL_BGRA_EXT);

}

GLuint RenderHandler::compileShader(GLenum shaderType, const char* path)
{
  GLuint shader = glCreateShader(shaderType);

  std::ifstream ifs(path);
  std::string shaderStr( (std::istreambuf_iterator<char>(ifs) ),
                         (std::istreambuf_iterator<char>()  ) );

  const char* shaderData = shaderStr.c_str();

  //  cout << shaderData << endl;
  glShaderSource(shader, 1, &shaderData, NULL);
  glCompileShader(shader);

    char buffer[513];
    glGetShaderInfoLog(shader, 512, NULL, buffer );
    std::cout << buffer << std::endl;
    
    
  GLint status;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
  if (status == GL_FALSE)
    printf("SHADER COMPILE ERROR\n");

    
    
    
    
  return shader;
}
