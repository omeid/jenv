#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "error.c"
#include "json.c"

const char DEFAULT_PREFIX[] = "jenv_";
const int  BLOCK_SIZE       = 1024;
const int  NUM_TOKENS       = 100;

void printHelp() {

  printf("Jenv - version v0.1\n\n");
  printf("Jenv is a mini tool for editing JSON files based on environment variables.\n\n");

  printf("Usage:  jenv filename [Prefix]\n\n");


  printf("filename:  The JSON file path. The file is not altered.\n");
  printf("           This parameter is required.\n");
  printf("Prefix:    The env prefix that holds replacement values.\n");
  printf("           Must be valid shell variable name. No whitespace.\n");
  printf("           Default: '%s'.\n\n", DEFAULT_PREFIX);

  printf("Output:    The result is printed to standard output.\n\n");
};


int main(int argc, char **argv, char **envp)
{

  if(argc < 2) {
    printHelp();
    return 1;
  }
  //Escape the binary name name
  *argv++;

  //Grab the file name
  const char* file_path = *argv++;

  if(file_path == "help"){
    printHelp();
  }

  const char* prefix = (argc > 2) ? *argv++ : DEFAULT_PREFIX;
  const int   prefix_len = strlen(prefix); //Well, array starts from 0, and then it's null termnated                                            // no idea why there is an extra space at the end.

  for(int i = 0; i < prefix_len; i++) {
    if(isspace(prefix[i])) Fatal("Invalid Prefix. See `jenv help`");
  }

  FILE *f;
  char *json;
  int  filesize;


  f = fopen(file_path, "rb");
  if(f == NULL) {
    fprintf(stderr, "\tFile passed: %s\n", file_path);
    Fatal("Failed to open file.");
  }

  fseek (f, 0, SEEK_END);
  filesize = ftell(f);
  fseek (f, 0, SEEK_SET);
  json = malloc(2*filesize); //Twice the size, just to be safe.
  if(json == NULL) {
    Fatal("Can't allocate memory.");
  };

  size_t json_size = fread (json, 1, filesize, f);
  if(json_size != filesize) {
    Fatal("Something went wrong. Filesize missmatch.");
  };
  fclose(f);

  while(*envp) {

    char* kvp = *envp++;

    //if kvp is empty, give up.
    if(kvp == NULL) break;

    int cur  = 0;

    //if key value pair is smaller than prefix, escape.
    if(strlen(kvp) < prefix_len) continue;


    char* key_prefixed = strtok(kvp, "="); 
    int key_len = strlen(key_prefixed);

    //if key is smaller than prefix, escape.
    if(key_len < prefix_len) continue;


    for(int i = 0; i < prefix_len; i++) {
      if(key_prefixed[i] != prefix[i]) goto NEXT;
    }

    //Create a scope to avoid 'goto' jump into
    //a scope with "undefined" vars.
    {
      char key[key_len - prefix_len + 1];
      key_len = key_len - prefix_len;
      strncpy(key, key_prefixed+prefix_len, key_len);
      key[key_len] = '\0';

      char* value = kvp+key_len+prefix_len+1;
      int value_len = strlen(value); //Just for consistency.
      replace_node_value(json, filesize, key, key_len, value, value_len);
    }

    NEXT:
    ; //Stop label at end of compound statement error. /* */
  }
  printf(json);
  free(json);

  return 0;
}
