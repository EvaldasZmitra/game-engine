#include <elf_str.h>
#include <stdlib.h>
#include <memory.h>

unsigned int get_null_index(const char *str, unsigned int max_len)
{
    for (unsigned int i = 0; i < max_len; i++)
    {
        if (str[i] == '\0')
        {
            return i;
        }
    }
    return 0;
}

elf_str elf_str_init(const char *chars, unsigned int len)
{
    unsigned int null_len = len;
    if (chars[len - 1] != '\0')
    {
        null_len += 1;
    }
    elf_str result;
    result.len = null_len;
    result.chars = malloc(null_len * sizeof(char));
    memcpy(result.chars, chars, len * sizeof(char));
    result.chars[null_len - 1] = '\0';
    return result;
}

elf_str elf_str_init_null(const char *chars, unsigned int max_len)
{
    return elf_str_init(chars, get_null_index(chars, max_len) + 1);
}

void elf_str_free(const elf_str *str)
{
    free(str->chars);
}

elf_str elf_str_copy(const elf_str *str)
{
    return elf_str_init(str->chars, str->len);
}

elf_str elf_str_slice(const elf_str *str, unsigned int start, unsigned int end)
{
    unsigned int length = end - start;
    return elf_str_init(&str->chars[start], length);
}

unsigned int elf_str_r_index(const elf_str *str, char symbol)
{
    for (unsigned int i = 0; i < str->len; i++)
    {
        unsigned int ind = str->len - i - 1;
        if (str->chars[ind] == symbol)
        {
            return ind;
        }
    }
    return str->len;
}

unsigned int elf_str_l_index(const elf_str *str, char symbol)
{
    for (unsigned int i = 0; i < str->len; i++)
    {
        if (str->chars[i] == symbol)
        {
            return i;
        }
    }
    return str->len;
}

elf_str elf_file_name(const elf_str *str)
{
    unsigned int start = elf_str_r_index(str, '/');
    if (start == str->len)
    {
        return elf_str_copy(str);
    }
    else
    {
        return elf_str_slice(str, start, str->len);
    }
}

elf_str elf_str_add(const elf_str *str, const elf_str *other)
{
    unsigned int size_str = (str->len - 1) * sizeof(char);
    unsigned int size_other = (other->len - 1) * sizeof(char);
    elf_str result;
    result.len = size_str + size_other + 1;
    result.chars = malloc(result.len * sizeof(char));
    memcpy(result.chars, str->chars, size_str);
    memcpy(&result.chars[size_str], other->chars, size_other);
    result.chars[size_other + size_str] = '\0';
    return result;
}

elf_str elf_str_replace_path(const elf_str *path, const elf_str *replace)
{
    elf_str name = elf_file_name(path);
    elf_str result = elf_str_add(replace, &name);
    elf_str_free(&name);
    return result;
}

elf_str elf_str_get_path(const elf_str *path)
{
    unsigned int r_index_1 = elf_str_r_index(path, '/');
    unsigned int r_index_2 = elf_str_r_index(path, '\\');
    unsigned int r_index = path->len;
    if (r_index_1 < path->len)
    {
        r_index = r_index_1;
    }
    if (r_index_2 < path->len && (r_index_2 > r_index_1 || r_index_1 == path->len))
    {
        r_index = r_index_2;
    }
    if (r_index < path->len)
    {
        return elf_str_slice(path, 0, r_index);
    }
    return elf_str_copy(path);
}