#include <ArduinoJson.h>


// allocate the memory for the document
#include <ArduinoJson.h>

// create an object

DynamicJsonDocument JSONIFY(String str) {
  const size_t capacity = JSON_OBJECT_SIZE(1) + 10;
  DynamicJsonDocument doc(capacity);

  char json[response.length()];
  str.toCharArray(json, response.length());
  
  deserializeJson(doc, json);
  
  return doc;
}



// serialize the object and send the result to Serial
