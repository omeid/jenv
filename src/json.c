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
    default:
      return ','; //For premative types.
  }
}


void replace_node_value(char *json, int json_size, const char *key, const int key_len, const char *value, const int value_len){

  char *new_json; //The result.

  if(key_len > json_size) return;


  for(int j = 0; j < json_size; j++){
    //j               //  json cursor
    int k = 0;        //  key  cursor
    int v_start = 0;  //  the start point of old value in json
    int v_end = 0;    //  the end   point of old value in json

    //Lets find the key.
    if(!(json[j] == '"' && json[j+1] == key[k])) continue;

    j++; //step over start of the key.

    // Step until matching key.
    while(key[++k] == json[++j]);

    //if size missmatch, next.
    if(k != key_len) continue;

    //if not end of the key, that means 
    // Key is longer than needle, next.
    if(json[j] != '"') continue;

    // Step over the closing tag: " 
    // and then any whitespace.
    while(isspace(json[++j]));

    // if not a colon,
    // Most probably we found a value
    // that is equal to our key, next.
    if(json[j] != ':') continue;

    // Found the key.
    // Step over ':' and then any whitespace.
    while(isspace(json[++j]));

    char v_opening_tag = json[j];
    char v_closing_tag = node_closing_tag(v_opening_tag);

    v_start = j;

    // Target object nested items book-keeping.
    // Only objects of the same type.
    // Doesn't apply to "string".
    int inception = 0;

    // string flag, to keep track
    // of when we are within a string value.
    bool is_string = false;

    while(json[j++]) {

      const char c = json[j];

      bool closing = (c == v_closing_tag);
      //If we see an unescaped '"' and that isn't our closing tag
      // and it isn't escaped we are now entering or leaving a string.
      if(c == '"' &&  !closing && json[j-1] != '\\' ) is_string = !is_string;

      //If we are within a string, the value doesn't matter, next.
      //for backslash, escape the next one too.
      if(is_string || c == '\\' && j++) continue;

      //if we see a closing tag and are not 
      // withing a nested item
      //this is the end of the value, break out.
      if(closing && !inception) break;

      //if we see an opening tag do book keeping. 
      if(closing && inception) inception--;

      //If we see another another opening tag, it is a nested element,
      //do book keeping then next.
      if(c == v_opening_tag) inception++;

    }

    //If not a premative type, step over the closing tag.
    if(v_closing_tag != ',') ++j;

    //For clearity and consistency with v_start.
    v_end = j;

    //Make the new size: before old value + new value + after old value.
    int new_size = v_start + value_len + (json_size - v_end);

    new_json = malloc(new_size);
    if(new_json == NULL) {
      Fatal("Can't allocate memory.");
    };


    strncpy(new_json, json, v_start); //before old value
    strcat(new_json, value);          //new value
    strcat(new_json, json+v_end);     // after old value

    strcpy(json, new_json);

  }
};
