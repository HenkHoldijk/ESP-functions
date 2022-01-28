#include <time.h>
#include <LittleFS.h>

#define LOCAL_UTC_OFFSET_H  ( 1 )

// List the content of LittleFS (using recursive function call)
void listDir(String sRoot)
{
  static int dRecursionDepth;

  Dir dir = LittleFS.openDir(sRoot);
  FSInfo fs_info;

  if (dRecursionDepth == 0)
  {
    LittleFS.info(fs_info);
    Serial.println(F("LittleFS"));
    Serial.println(F("- Total bytes available : ") + String(fs_info.totalBytes));
    Serial.println(F("- Total bytes used      : ") + String(fs_info.usedBytes));
    Serial.println(F("- Max open files        : ") + String(fs_info.maxOpenFiles));
    Serial.println(F("- Max path length       : ") + String(fs_info.maxPathLength));
    Serial.println("Directory content of " + sRoot);
  }
  if (!LittleFS.exists(sRoot) && (dRecursionDepth == 0))
  {
    Serial.println(F("No files found."));
  }
  else
  {
    while(dir.next())
    {
      time_t tm_fct = dir.fileCreationTime() + (3600 * LOCAL_UTC_OFFSET_H);
      struct tm * fct = localtime(&tm_fct);
      Serial.printf("%02d-%02d-%04d  %02d:%02d", fct->tm_mday, fct->tm_mon + 1, fct->tm_year + 1900, fct->tm_hour, fct->tm_min);
      Serial.printf("%*s", 4, "");
      if (dir.isFile())
      {
        Serial.printf("%*d", 8, dir.fileSize());
        Serial.println(" " + sRoot + dir.fileName());
      }
      else if (dir.isDirectory())
      {
        Serial.printf("%-*s", 8, "<DIR>");
        Serial.println(" " + sRoot + dir.fileName() + "/");
        dRecursionDepth++;
        listDir(sRoot + dir.fileName() + "/");
        dRecursionDepth--;
      }
    }
  }
  if (dRecursionDepth == 0)
  {
    Serial.println("");
  }

  return;
}

// Setup of the LittleFS (File System)
void setup_FS()
{
  // Initialize LittleFS
  Serial.println(F("Mounting LittleFS"));
  if(!LittleFS.begin())
  {
    Serial.println(F("Error while mounting LittleFS"));
    return;
  }
  Serial.println(F("LittleFS Mounted"));
  Serial.println("");

  // Show the current File System content
  listDir("/");

  return;
}



void setup()
{
  // Set the Serial port to 115200 baud
  Serial.begin(115200);
  while (!Serial)
  {
    delay(100);
  }
  delay(1000);

  // Setup the File System
  setup_FS();
}

void loop()
{
}
