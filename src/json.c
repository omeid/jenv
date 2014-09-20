#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>
#include "error.c"

const char node_closing_tag(char tag) {
  switch(tag) {
    case '"':
      return '"';
    case '{':
      return '}';
    case '[':
      return ']';
  }
}


void replace_node_value(char *json, const int json_size, const char *key, const int key_len, const char *value, const int value_len){

  if(key_len > json_size) return;

  char *new_json;

  for(int j = 0; j < json_size; j++){
    int k = 0;
    int v_start = 0;
    int v_end = 0;

    //Lets find the key.
    if(!(json[j] == '"' && json[j+1] == key[k])) continue;

    j++; //Previous line: for j and k where json[j+1] == key[k] then json[j] != key[k] is true;


    //walk the key
    while(key[k] == json[j]){
      k++; j++;
    }

    //Is this really the key you want?
    if(k == key_len && json[j] == '\"') {
      while(isspace(json[j++]));   // Escape '"' and then any whitespace.
      if(json[j] != ':') continue; // Expect colon.
      while(isspace(json[++j]));   // Escape ':' and then any whitespace.

      char v_opening_tag = json[j]; //Don't care about grand-childern now.
      char v_closing_tag = node_closing_tag(v_opening_tag);

      v_start = j;

      int inception = 0; // Implemnt this!
      bool is_string = false;

      while(json[j++]) {
        switch(json[j]) {

          case '\\':
            switch(json[j+1]) {

              case '"' : 
              case '\\': 
              case '/' : 
              case 'b' :
              case 'f' :
              case 'n' :
              case 'r' :
              case 't' :
                j++;
                break;

              case 'u' :
                j = j+4;
                break;
            }
            break;


          case '"':
            is_string = v_closing_tag != json[j] && !is_string;
            break;

          default:
            if(!is_string && (json[j] == v_closing_tag || json[j] == v_opening_tag))
              break;
        }
        if(json[j] == v_opening_tag){
          inception++;
          continue;
        }

        if(json[j] == v_closing_tag) {
          if(inception) {
            inception--;
            continue;
          }
          if(!is_string){
            v_end = j;
            break;
          }
        }
      }

      int new_size = json_size - v_end + v_start + value_len;
      new_json = malloc(new_size);
      if(new_json == NULL) {
        Fatal("Can't allocate memory.");
      };

      int cur = 0;
      strncpy(new_json, json, v_start);
      strcat(new_json, value);
      strcat(new_json, json+v_end+1);

      strcpy(json, new_json);
    }
  }
  return;
};

