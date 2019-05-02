#include <stdlib.h>
#include <ruby.h>
#include <ctype.h>

static VALUE soundx_frozen_empty_string;
static VALUE rb_m_SoundX;

static VALUE
soundx_encode(const char* input, const long len)
{
  if (len < 1) return soundx_frozen_empty_string;
  if (input == 0) return soundx_frozen_empty_string;

  char result[8];
  int i = 0;
  int j = 0;
  int code = 0;

  static const char icode[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 1, 2, 3, 0, 1, 2, 0, 0, 2, 2, 4, 5, 5, 0,
    1, 2, 6, 2, 3, 0, 1, 0, 2, 0, 2, 0, 0, 0, 0, 0,
    0, 0, 1, 2, 3, 0, 1, 2, 0, 0, 2, 2, 4, 5, 5, 0,
    1, 2, 6, 2, 3, 0, 1, 0, 2, 0, 2, 0, 0, 0, 0, 0,
  };

  char prevcode = icode[input[i]];
  result[0] = toupper(input[i]);
  for(j = 1; j < 4 && input[i]; i++){
    code = icode[input[i]];
    if (code > 0) {
      if (code != prevcode) {
        prevcode = code;
        result[j++] = code + '0';
      }
    }
  }
  while (j < 4) {
    result[j++] = '0';
  }
  result[j] = 0;
  return rb_str_new_frozen(rb_str_new_cstr(result));
}

VALUE
rb_SoundX_encode(VALUE self, VALUE input)
{
  if (TYPE(input) != T_STRING) {
    rb_raise(rb_eTypeError, "expected a String");
  }

  if (!rb_funcall(input, rb_intern("ascii_only?"), 0)) {
    rb_raise(rb_eArgError, "non-ASCII character found");
  }

  return soundx_encode(StringValueCStr(input), RSTRING_LEN(input));
}

void
Init_soundx() {
  soundx_frozen_empty_string = rb_str_new_frozen(rb_str_new_cstr(""));
  rb_m_SoundX = rb_define_module("SoundX");

  rb_define_module_function(rb_m_SoundX, "encode", rb_SoundX_encode, 1);
}
