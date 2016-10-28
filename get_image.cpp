#if defined(_MSC_VER) || defined(WINDOWS)
#define _USE_MATH_DEFINES
#endif
#include <cmath>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <cassert>
#include "lodepng.h"
#include <GL/glew.h>
#include <GL/freeglut.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#if defined(_MSC_VER) || defined(WINDOWS)
#include <windows.h>
#include <rtcapi.h>

int
exception_handler(LPEXCEPTION_POINTERS p)
{
  printf("Exception detected during test execution!\n");
  exit(1);
}

int
runtime_check_handler(int errorType, const char *filename, int linenumber,
              const char *moduleName, const char *format, ...)
{
  printf("Error type %d at %s line %d in %s", errorType, filename, linenumber, moduleName);
  exit(1);
}
#endif

#define WIDTH 640
#define HEIGHT 480
#define CHANNELS 4
#define OUTPUT_IMAGE_MAX_LINE_LEN 70
#define OUTPUT_IMAGE_MAX_PIXEL_VAL_LEN 4
#define OUTPUT_IMAGE_MAX_PIXEL_VAL 255

#define COMPILE_ERROR_EXIT_CODE 101
#define LINK_ERROR_EXIT_CODE 102
#define RENDER_ERROR_EXIT_CODE 103

std::string terminate_flag = "",
  output = "output.png",
  vertex_shader = "",
  fragment_shader = "";
bool persist = false,
  exit_compile = false,
  exit_linking = false,
  animate = false;
GLuint program;

bool rendered = false;

bool
parse_args(int argc, char* argv[])
{
  vertex_shader = "";
  fragment_shader = "";
  std::string curr_arg;
  for (int i = 1; i < argc; i++) {
    curr_arg = std::string(argv[i]);
    if (!curr_arg.compare(0, 2, "--")) {
      if (!curr_arg.compare("--persist")) {
    persist = true;
    continue;
      }
      else if (!curr_arg.compare("--animate")) {
    animate = true;
    continue;
      }
      else if (!curr_arg.compare("--exit_compile")) {
    exit_compile = true;
    continue;
      }
      else if (!curr_arg.compare("--exit_linking")) {
    exit_linking = true;
    continue;
      }
      else if (!curr_arg.compare("--output")) {
    output = argv[++i];
    continue;
      }
      printf("Unknown argument %s\n", curr_arg.c_str());
      continue;
    }
    if (vertex_shader == "")
      vertex_shader = curr_arg;
    else if (fragment_shader == "")
      fragment_shader = curr_arg;
    else printf("Ignoring extra argument %s\n", curr_arg.c_str());
  }
  return (vertex_shader != "" && fragment_shader != "");
}

void
report_error_and_exit(GLuint oglPrg, int exitCode)
{
  GLint maxLength = 0;
  glGetShaderiv(oglPrg, GL_INFO_LOG_LENGTH, &maxLength);

  // The maxLength includes the NULL character
  std::vector<GLchar> errorLog(maxLength);
  glGetShaderInfoLog(oglPrg, maxLength, &maxLength, &errorLog[0]);

  // Provide the infolog in whatever manor you deem best.
  for (GLchar c : errorLog)
    std::cout << c;
  std::cout << "\n";
  // Exit with failure.
  glDeleteShader(oglPrg);
  exit(exitCode);
}

void
keyboard_listener(unsigned char key, int x, int y)
{
  exit(0);
}

void init(std::string vertexShader, std::string fragmentShader)
{
  int compile_ok;
  program = glCreateProgram();

  // Shader start
  GLuint shd0 = glCreateShader(GL_VERTEX_SHADER);
  std::ifstream shdf0(vertexShader);
  if(!shdf0) {
    fprintf(stderr, "Vertex shader %s not found\n", vertexShader.c_str());
    exit(1);
  }
  std::stringstream shds0;
  shds0 << shdf0.rdbuf();
  std::string shdstr0= shds0.str();
  const char *shdchr0= shdstr0.c_str();
  glShaderSource(shd0, 1, &shdchr0, NULL);
  fprintf(stderr, "Compiling vertex shader\n");
  glCompileShader(shd0);
  glGetShaderiv(shd0, GL_COMPILE_STATUS, &compile_ok);
  if (!compile_ok) {
    fprintf(stderr, "Error compiling vertex shader\n");
    report_error_and_exit(shd0, 1);
  }
  fprintf(stderr, "Vertex shader compiled successfully\n");
  glAttachShader(program, shd0);
  // Shader end

  // Shader start
  GLuint shd1 = glCreateShader(GL_FRAGMENT_SHADER);
  std::ifstream shdf1(fragmentShader);
  if(!shdf1) {
    fprintf(stderr, "Fragment shader %s not found\n", fragmentShader.c_str());
    exit(1);
  }
  std::stringstream shds1;
  shds1 << shdf1.rdbuf();
  std::string shdstr1= shds1.str();
  const char *shdchr1= shdstr1.c_str();
  glShaderSource(shd1, 1, &shdchr1, NULL);
  fprintf(stderr, "Compiling fragment shader\n");
  glCompileShader(shd1);
  glGetShaderiv(shd1, GL_COMPILE_STATUS, &compile_ok);
  if (!compile_ok) {
    fprintf(stderr, "Error compiling fragment shader\n");
    report_error_and_exit(shd1, COMPILE_ERROR_EXIT_CODE);
  }
  fprintf(stderr, "Fragment shader compiled successfully\n");
  if (exit_compile) {
    fprintf(stdout, "Exiting after fragment shader compilation...\n");
    exit(0);
  }
  glAttachShader(program, shd1);
  // Shader end

  fprintf(stderr, "Linking program\n");
  glLinkProgram(program);
  glGetProgramiv(program, GL_LINK_STATUS, &compile_ok);
  if (!compile_ok) {
    fprintf(stderr, "Error in linking program\n");
    report_error_and_exit(program, LINK_ERROR_EXIT_CODE);
  }
  fprintf(stderr, "Program linked successfully\n");
  if (exit_linking) {
    fprintf(stdout, "Exiting after program linking...\n");
    exit(0);
  }
  // Program 5 end
  glutKeyboardFunc(keyboard_listener);
}

void display()
{
  static int num_display_calls = 0;

  glClearColor(1.0, 1.0, 0.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);
  glUseProgram(program);

  const char* uniform_name;
  GLint uniform_loc;
  uniform_name = "resolution";
  uniform_loc = glGetUniformLocation(program, uniform_name);
  if (uniform_loc != -1)
    glUniform2f(uniform_loc, WIDTH, HEIGHT);
  uniform_name = "mouse";
  uniform_loc = glGetUniformLocation(program, uniform_name);
  if (uniform_loc != -1)
    glUniform2f(uniform_loc, 0.0, 0.0);
  uniform_name = "injectionSwitch";
  uniform_loc = glGetUniformLocation(program, uniform_name);
  if (uniform_loc != -1)
    glUniform2f(uniform_loc, 0.0, 1.0);
  uniform_name = "time";
  uniform_loc = glGetUniformLocation(program, uniform_name);
  if (uniform_loc != -1)
    glUniform1f(uniform_loc, (animate ? glutGet(GLUT_ELAPSED_TIME) / 1000.0 * (2*M_PI) / 5 : 0));

  glEnableVertexAttribArray(0);
  GLfloat arr_5_0[] = {
    -1, 1,
    -1, -1,
    1, -1,
  };
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, arr_5_0);
  glDrawArrays(GL_TRIANGLES, 0, 3.0);
  GLfloat arr_5_1[] = {
    -1, 1,
    1, 1,
    1, -1,
  };
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, arr_5_1);
  glDrawArrays(GL_TRIANGLES, 0, 3.0);
  glDisableVertexAttribArray(0);

  glFinish();

  glutSwapBuffers();

  num_display_calls++;

  rendered = (num_display_calls >= 2);

}

void idle()
{
  static bool saved = false;

  if (rendered && !saved) {
    saved = true;
    std::vector<std::uint8_t> data(WIDTH * HEIGHT * CHANNELS);
    glReadPixels(0, 0, WIDTH, HEIGHT, GL_RGBA, GL_UNSIGNED_BYTE, &data[0]);
    unsigned png_error = lodepng::encode(output, data, WIDTH, HEIGHT);
    if (png_error)
        printf("Error producing PNG file: %s", lodepng_error_text(png_error));
    if (persist)
      printf("Press any key to exit...\n");
    else
      exit(0);
  }
  glutPostRedisplay();
}

int main(int argc, char* argv[])
{

#ifdef _MSC_VER
  DWORD dwMode = SetErrorMode(SEM_NOGPFAULTERRORBOX);
  SetErrorMode(dwMode | SEM_NOGPFAULTERRORBOX);
  SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)&exception_handler);
  _RTC_SetErrorFunc(&runtime_check_handler);
#endif

  if(!parse_args(argc, argv)) {
    std::cerr << "Requires vertex shader and fragment shader arguments!" << std::endl;
    exit(1);
  }

  glutInit(&argc, argv);
  glutInitContextVersion(2,0);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowSize(WIDTH, HEIGHT);
  glutCreateWindow("get_image");
  glewInit();
  init(std::string(argv[1]), std::string(argv[2]));
  glutDisplayFunc(display);
  glutIdleFunc(idle);
  glutMainLoop();
  return EXIT_SUCCESS;
}
