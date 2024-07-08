#include "gui.h"

void gui::realize(GtkWidget *widget){

  GdkGLContext *context;
  gtk_gl_area_make_current (GTK_GL_AREA (widget));
  if (gtk_gl_area_get_error (GTK_GL_AREA (widget)) != NULL)
    return;

  context = gtk_gl_area_get_context (GTK_GL_AREA (widget));

  // init object here
  lg.init();
  i3d.init();
  // end init objects

  GLuint vertex, fragment;
  int status;

  vertex = create_shader (GL_VERTEX_SHADER);

  if (vertex == 0){
      return;
  }

  fragment = create_shader (GL_FRAGMENT_SHADER);

  if (fragment == 0){
      glDeleteShader (vertex);
      return;
    }

  Program3D = glCreateProgram ();
  glAttachShader (Program3D, vertex);
  glAttachShader (Program3D, fragment);

  glLinkProgram (Program3D);

  glGetProgramiv (Program3D, GL_LINK_STATUS, &status);
  if (status == GL_FALSE)
  {
    int log_len;
    char *buffer;

    glGetProgramiv (Program3D, GL_INFO_LOG_LENGTH, &log_len);

    buffer = (char*)g_malloc (log_len + 1);
    glGetProgramInfoLog (Program3D, log_len, NULL, buffer);

    g_warning ("Linking failure:\n%s", buffer);

    g_free (buffer);

    glDeleteProgram (Program3D);
    Program3D = 0;

    glDeleteShader (vertex);
    glDeleteShader (fragment);

    return;
  }

  glDetachShader (Program3D, vertex);
  glDetachShader (Program3D, fragment);

  glEnable(GL_CULL_FACE);
  glFrontFace(GL_CCW);
  glCullFace(GL_BACK);
  glEnable(GL_DEPTH_TEST);

}

bool gui::render(GtkGLArea *area, GdkGLContext *context){

  if (gtk_gl_area_get_error (area) != NULL)
    return FALSE;

  /* Clear the viewport */
  glClearColor (0.0, 0.0, 0.0, 1.0);
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  /* Draw our object */

  //lg.draw(Program3D);
  i3d.draw(Program3D, msg);

  /* Flush the contents of the pipeline */
  glFlush ();
  gtk_gl_area_queue_render (area);
  return TRUE;

}

void gui::unrealize(GtkWidget *widget){
  gtk_gl_area_make_current (GTK_GL_AREA (widget));

  if (gtk_gl_area_get_error (GTK_GL_AREA (widget)) != NULL)
    return;

  // Clear objects glDeleteBuffers
  lg.reset();

  //

  glDeleteProgram (Program3D);

}

GLuint gui::create_shader(int type){

  GLuint shader;
  int status;
  shader = glCreateShader (type);
  if (type== GL_FRAGMENT_SHADER){
    glShaderSource (shader, 1, &FRAGMENT_SOURCE, NULL);
  }

  if (type== GL_VERTEX_SHADER){
    glShaderSource (shader, 1, &VERTEX_SOURCE, NULL);
  }

  glCompileShader (shader);
  glGetShaderiv (shader, GL_COMPILE_STATUS, &status);

  if (status == GL_FALSE){

    int log_len;
    char *buffer;
    glGetShaderiv (shader, GL_INFO_LOG_LENGTH, &log_len);
    buffer = (char*)g_malloc (log_len + 1);
    glGetShaderInfoLog (shader, log_len, NULL, buffer);
    g_warning ("Compile failure in %s shader:\n%s",
               type == GL_VERTEX_SHADER ? "vertex" : "fragment",
               buffer);
    g_free (buffer);
    glDeleteShader (shader);
    return 0;

  }

  return shader;
}
