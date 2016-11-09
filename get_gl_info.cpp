#include <iostream>
#include <string>
#include <sstream>

#include <GL/glew.h>
#ifdef __APPLE__
	#include <OpenGL/gl.h>
	#include <OpenGl/glu.h>
	#include <GLUT/glut.h>
#else
	#include <GL/freeglut.h>
#endif



void
add_line (std::stringstream& json_ss, std::stringstream& line, int indent)
{
	json_ss << std::string(indent, '\t') << line.str() << std::endl;
	line.str(std::string());
}

void
add_line (std::stringstream& json_ss, std::string line, int indent)
{
	json_ss << std::string(indent, '\t') << line << std::endl;
}

int
main (int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutCreateWindow("blank");
	glewInit();

	GLint major_version, minor_version;
	const GLubyte* ogl_version = glGetString(GL_VERSION),
		*glsl_version = glGetString(GL_SHADING_LANGUAGE_VERSION),
		*vendor = glGetString(GL_VENDOR),
		*renderer = glGetString(GL_RENDERER);
	glGetIntegerv(GL_MAJOR_VERSION, &major_version);
	glGetIntegerv(GL_MINOR_VERSION, &minor_version);

	GLint glsl_versions_count;
	glGetIntegerv(GL_NUM_SHADING_LANGUAGE_VERSIONS, &glsl_versions_count);

	std::stringstream json_stream, curr_stream;
	int indent = 0;
	add_line(json_stream, "{", indent++);
	// OpenGL version
	curr_stream << "\"GL_VERSION\" : \"" << ogl_version << "\",";
	add_line(json_stream, curr_stream, indent);
    // OpenGL major and minor versions
	curr_stream << "\"GL_MAJOR_VERSION\" : \"" << major_version << "\",";
	add_line(json_stream, curr_stream, indent);
	curr_stream << "\"GL_MINOR_VERSION\" : \"" << minor_version << "\",";
	add_line(json_stream, curr_stream, indent);
	// GLSL version
	curr_stream << "\"GL_SHADING_LANGUAGE_VERSION\" : \"" << glsl_version << "\",";
	add_line(json_stream, curr_stream, indent);
	// Vendor
	curr_stream << "\"GL_VENDOR\" : \"" << vendor << "\",";
	add_line(json_stream, curr_stream, indent);
	// Device
	curr_stream << "\"GL_RENDERER\" : \"" << renderer << "\",";
	add_line(json_stream, curr_stream, indent);
	// Supported GLSL versions
	curr_stream << "\"Supported_GLSL_versions\" : [";
	int i;
	for (i = 0; i < glsl_versions_count - 2; i++)
		curr_stream << "\"" <<  glGetStringi(GL_SHADING_LANGUAGE_VERSION, i) << "\", ";
	curr_stream << "\"" <<  glGetStringi(GL_SHADING_LANGUAGE_VERSION, i) << "\"]";
	add_line(json_stream, curr_stream, indent);
	add_line(json_stream, "}", --indent);
	std::cout << json_stream.str() << std::endl;
}

