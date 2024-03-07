#include <elf_import.h>
#include <ImageMagick-7/MagickWand/MagickWand.h>

GLuint elf_load_shader(const GLchar *src_code, GLenum type)
{
    GLuint vertex_shader_id = glCreateShader(type);
    glShaderSource(vertex_shader_id, 1, &src_code, NULL);
    glCompileShader(vertex_shader_id);

    GLuint result = GL_FALSE;
    glGetShaderiv(vertex_shader_id, GL_COMPILE_STATUS, &result);
    if (!result)
    {
        GLint error_length;
        glGetShaderiv(vertex_shader_id, GL_INFO_LOG_LENGTH, &error_length);
        GLchar *error_msg = malloc(sizeof(GLchar) * error_length);
        glGetShaderInfoLog(vertex_shader_id, error_length, NULL, error_msg);
        fprintf(stderr, "%s\n", error_msg);
        return -1;
    }
    return vertex_shader_id;
}

GLuint elf_load_comp(const GLchar *src_code)
{
    return elf_load_shader(src_code, GL_COMPUTE_SHADER);
}

GLuint elf_load_vert(const GLchar *src_code)
{
    return elf_load_shader(src_code, GL_VERTEX_SHADER);
}

GLuint elf_load_tess_ctrl(const GLchar *src_code)
{
    return elf_load_shader(src_code, GL_TESS_CONTROL_SHADER);
}

GLuint elf_load_tess_eval(const GLchar *src_code)
{
    return elf_load_shader(src_code, GL_TESS_EVALUATION_SHADER);
}

GLuint elf_load_geom(const GLchar *src_code)
{
    return elf_load_shader(src_code, GL_GEOMETRY_SHADER);
}

GLuint elf_load_frag(const GLchar *src_code)
{
    return elf_load_shader(src_code, GL_FRAGMENT_SHADER);
}

elf_shd elf_load_program(
    const GLchar *name,
    const GLuint name_len,
    const GLuint comp,
    const GLuint vert,
    const GLuint tess_ctrl,
    const GLuint tess_eval,
    const GLuint geom,
    const GLuint frag)
{
    elf_shd shader;
    memcpy(shader.name, shader.name, name_len * sizeof(GLchar));
    shader.binary_data = NULL;
    GLuint program_id = glCreateProgram();
    if (comp != NULL)
    {
        glAttachShader(program_id, comp);
    }
    if (vert != NULL)
    {
        glAttachShader(program_id, vert);
    }
    if (tess_ctrl != NULL)
    {
        glAttachShader(program_id, tess_ctrl);
    }
    if (tess_eval != NULL)
    {
        glAttachShader(program_id, tess_eval);
    }
    if (geom != NULL)
    {
        glAttachShader(program_id, geom);
    }
    if (frag != NULL)
    {
        glAttachShader(program_id, frag);
    }
    glLinkProgram(program_id);
    GLint result = GL_FALSE;
    glGetProgramiv(program_id, GL_LINK_STATUS, &result);
    if (!result)
    {
        GLint error_length;
        glGetProgramiv(program_id, GL_INFO_LOG_LENGTH, &error_length);
        GLchar *error_msg = malloc(sizeof(GLchar) * error_length);
        glGetProgramInfoLog(program_id, error_length, NULL, error_msg);
        fprintf("%s\n", error_msg);
        return shader;
    }
    glGetProgramiv(program_id, GL_PROGRAM_BINARY_LENGTH, &shader.binary_length);
    shader.binary_data = malloc(shader.binary_length * sizeof(GLchar));
    glGetProgramBinary(program_id, shader.binary_length, NULL, &shader.format, shader.binary_data);
    return shader;
}

void createDDSFromPixels(const char *outputFilename, const unsigned char *pixels, int width, int height)
{
    MagickWandGenesis();
    MagickWand *wand = NewMagickWand();
    MagickSetSize(wand, width, height);
    MagickSetImageFormat(wand, "RGBA");
    MagickSetImagePixels(wand, 0, 0, width, height, "RGBA", CharPixel, pixels);
    MagickWriteImage(wand, outputFilename);
    DestroyMagickWand(wand);
    MagickWandTerminus();
}