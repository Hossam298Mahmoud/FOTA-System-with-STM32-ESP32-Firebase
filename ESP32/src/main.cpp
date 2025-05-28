#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <mbedtls/aes.h>
#include "SPIFFS.h"
#include <HardwareSerial.h>
#include <EEPROM.h>

// Your WiFi credentials
#define WIFI_SSID  "Vodafone555"
#define WIFI_PASSWORD "Vodafone@1999"
const char* file_url = "http://esp32.unaux.com//update.txt";  // Replace with your actual file URL
/*********************************Firebase*******************************************/
// Your Firebase Project Web API Key
#define API_KEY "AIzaSyB11GQAtSKLsPvl_wFMU02yyQPs54fNgjU"
// Your Firebase Realtime database URL
#define DATABASE_URL "https://fota-8e8d6-default-rtdb.firebaseio.com"

//Define Firebase Data object
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;
/*****************************CONSTANTS********************************************/
#define	CHECK_UPDATE   0x50
#define	UPDATE_EXIST   0x51
#define UPDATE_DOWN    0X52


#define REQEST_UPDATE      'A'
#define REQEST_ACCEPT      'B'
#define REJECT_UPDATE      'F'


#define SEND_HEADER        'C'
#define FINISH_HEADER      'D'
#define ACK_HEADER         'E'

#define SEND_CODE          'F'
#define FINISH_CODE        'G'
#define ACK_CODE           'K'

#define FAILED             'N'

/**********************************************************************************/
/************************************************************************************/
float ECU1_Version=1.0;
float ECU2_Version =1.0;
const char* url ;  
float version ;  
int node_id ;
int app_crc ;
u32_t app_size=0;
char temp;
char header[4] , CURRENT_CASE=CHECK_UPDATE ,Received;
/*********************************************************************************/

 
/********************************************************************************/
const uint8_t key[32] = { 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
    0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF,
    0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
    0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF };

uint8_t iv[16] = { 0xFF, 0xEE, 0xDD, 0xCC, 0xBB, 0xAA, 0x99, 0x88,
   0x77, 0x66, 0x55, 0x44, 0x33, 0x22, 0x11, 0x00 };
/*------------------------------------------------------------------------------*/
void decryptFirmware(const char* encryptedFile, const char* outputFile) {
    File encFile = SPIFFS.open(encryptedFile, "r");
    if (!encFile) {
        Serial.println("Failed to open encrypted file!");
        return;
    }

    File decFile = SPIFFS.open(outputFile, "w");
    if (!decFile) {
        Serial.println("Failed to open output file!");
        encFile.close();
        return;
    }

    mbedtls_aes_context aes;
    mbedtls_aes_init(&aes);
    mbedtls_aes_setkey_dec(&aes, key, 256);  // AES-256

    size_t fileSize = encFile.size();
    uint8_t buffer[16];  // AES block size
    uint8_t decrypted[16];

    while (encFile.read(buffer, 16) == 16) {
        mbedtls_aes_crypt_cbc(&aes, MBEDTLS_AES_DECRYPT, 16, iv, buffer, decrypted);
        decFile.write(decrypted, 16);
    }
    
    mbedtls_aes_free(&aes);
    encFile.close();
    decFile.close();
    /*----------------------------------------------------------------------------*/
    File dec = SPIFFS.open(outputFile);
    Serial.printf("Decrypted bin File Size is: %d Bytes\n",dec.size());
    app_size=dec.size();
    dec.close();
    /*----------------------------------------------------------------------------*/
    Serial.println("Decryption complete! Ready for flashing.");
}
/**********************************************************************************/
// Function to download firmware.bin and save to SPIFFS
HardwareSerial SerialPort1(2); // use UART1
//HardwareSerial SerialPort2(2); // use UART2
/*********************************************************************************/
void printBuffer(const char* label, const uint8_t* buffer, size_t length) {
    Serial.print(label);
    Serial.print(": ");
    for (size_t i = 0; i < length; i++) {
        Serial.printf("%02X ", buffer[i]);  // Print as hex
    }
    Serial.println();
}

void send_file(void){
    char x = 0 ;
    char y = 0 ;
    Serial.print("here");
    File dec2 = SPIFFS.open("/decrypt.bin");
    if (!dec2) 
    {
      Serial.println("file open failed");
       return;
    }
    else
    {
        Serial.println("Reading Data from File:");
        //Data from file
        for(int i=0;i<dec2.size();i++) //Read upto complete file size
        {
          x = dec2.read();
          Serial.printf("0x%02x\n\r",x);  // Print as hex
          SerialPort1.write(x);
          //Serial.printf("0x%02x ", x);  // Print as hex
          delay(20);

         /* if (x == '\r')
          {
           x = f.read();
           Serial.print(x);
           Serial.println ("f");
           while ( Serial.available() == 0);
           y = Serial.read();
           Serial.println (y);
          }
          else {
          Serial.print(x);
           }*/
        }
        dec2.close();  //Close file
        Serial.println("File Closed");
    }
   }
/********************************************************************************/
int Download_Firmware(const char* firm_url) 
{
    HTTPClient http;
    http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
    SPIFFS.format();
    File encfile = SPIFFS.open("/encrypt.bin", FILE_WRITE);
    if(!encfile)
    {
        Serial.println("There was an error opening the file for writing");
        return false;
    }
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("WiFi not connected!");
      }

    http.begin(firm_url);
    int httpCode = http.GET();
    while (httpCode <= 0)
    {
        Serial.printf("HTTP failed, error: %s\n", 
        http.errorToString(httpCode).c_str());
       // return false ;
       WiFi.begin(WIFI_SSID , WIFI_PASSWORD);

       http.begin(firm_url);
       httpCode = http.GET();
       delay(5000);
    }

    if (httpCode == HTTP_CODE_OK) 
    {
        if(http.writeToStream(&encfile)>0)
        {
            Serial.println("File was written to encfile");
            if(http.getSize()%2)encfile.write('M');
            //Serial.println(http.getString());
        } 
        else 
        {
            Serial.println("File write failed");
        }

    }
    encfile.close();
    /*-------------------------------------------------------------------------------*/
    Serial.printf("http response size is: %d Bytes\n", http.getSize());
    
    File file1 = SPIFFS.open("/encrypt.bin");
    if(!file1){
        Serial.println("Failed to open file for reading");
        return false;
    }
    Serial.printf("Encrypted bin File Size is: %d Bytes\n" ,file1.size());
    file1.close();
    /*----------------------------Encryption Section-------------------------------------------*/
    File decfile = SPIFFS.open("/decrypt.bin");
    if(!decfile)
    {
        Serial.println("There was an error opening decfile for writing");
        return false;
    }
    decryptFirmware("/encrypt.bin" , "/decrypt.bin");
    /*------------------------------------------------------------------------------------------*/
                            /*--------For Testing----------*/
    /*File dec = SPIFFS.open("/decrypt.bin");
    Serial.printf("testing 1 decrypted bin File Size is: %d Bytes\n" ,dec.size());
    char x;
    while(dec.available())
    {
        x=dec.read();

        Serial.printf("0x%02x", x);  // Print as hex
       // Serial.printf("%c", x);  // Print as hex
    }
    dec.close();*/
    //send_file();
    /*------------------------------------------------------------------------------------------*/
    http.end();
    return true ;
  } 
/***********************************************************************/
/**********************************************************************************************/
void downloadAndParseJSON() 
{
    if (Firebase.RTDB.getJSON(&fbdo, "/ota")) 
    {  
        String jsonData=fbdo.to<String>();  //store raw JSON response
        StaticJsonDocument<200>jsondoc;
        // Deserialize JSON
        DeserializationError error = deserializeJson(jsondoc, jsonData);
        // Extract values
        url = jsondoc["url"];  
        version = jsondoc["version"];  
        node_id = jsondoc["node_id"];
        app_crc = jsondoc["app_crc"];
        Download_Firmware(url);
    }
}

void setup()  
{
    Serial.begin(115200);
    pinMode(16, INPUT_PULLUP);  // Internal pull-up to reduce noise on RX
    SerialPort1.begin(9600, SERIAL_8N1, 16, 17); 
    /*************************************************************/
    EEPROM.begin(10);
    if(EEPROM.readByte(0) == 255)
    {
        EEPROM.writeFloat(0,1.0);
        EEPROM.commit();   // Required to actually save to flash!
    }
    Serial.print("Current Version: ");
    Serial.println(EEPROM.readFloat(0),2);
    /*************************************************************/
    WiFi.begin(WIFI_SSID , WIFI_PASSWORD);

    if(!SPIFFS.begin(true))
    {
        Serial.println("An Error has occurred while mounting SPIFFS");
        return;
    }
    SPIFFS.format();

    Serial.print("Connecting to WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.print("\nConnected with IP: ");
    Serial.println(WiFi.localIP());

    /*********************************--Firebase Config--****************************************/
    /* Assign the api key (required) */
    config.api_key = API_KEY;

    /* Assign the RTDB URL (required) */
    config.database_url = DATABASE_URL;

      // Assign the user sign in credentials
    auth.user.email ="hossam338mahmoud@gmail.com";
    auth.user.password = "01028581757";
    Firebase.reconnectWiFi(true);
    fbdo.setResponseSize(4096);
    /* Sign up */
   /* if (Firebase.signUp(&config, &auth, "hossam338mahmoud@gmail.com", "01028581757")){
        Serial.println("SignUp to Firebase Successfully!");
    }
    else{
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
    }*/
    Firebase.begin(&config, &auth);
      // Getting the user UID might take a few seconds
    Serial.println("Getting User UID");
    while ((auth.token.uid) == "") {
        Serial.print('.');
        delay(1000);
    }
    // Print user UID
    String uid = auth.token.uid.c_str();
    Serial.print("User UID: ");
    Serial.println(uid);
    /*********************************************************************************************/
    /*******************************************************/
    //delay(5000);
    //downloadAndParseJSON() ;
    //send_file();
    //while(1);
    
}


void loop() 
{
    
    
    switch (CURRENT_CASE)
    {
        case CHECK_UPDATE:
            delay(5000);
            Serial.println("Check for New Update to ECU");
            if (Firebase.RTDB.getJSON(&fbdo, "/ota")) 
            {  
                String jsonData=fbdo.to<String>();  //store raw JSON response
                StaticJsonDocument<200>jsondoc;
                // Deserialize JSON
                DeserializationError error = deserializeJson(jsondoc, jsonData);
                // Extract values
                url = jsondoc["url"];  
                version = jsondoc["version"];  
                node_id = jsondoc["node_id"];
                app_crc = jsondoc["app_crc"];

                if(EEPROM.readFloat(0)<version)
                {
                    Serial.println("A New Update is available for ECU1");
                    Serial.println(url);
                    CURRENT_CASE =UPDATE_EXIST;
                }
            } else {
                Serial.print("Failed to get JSON, reason: ");
                Serial.println(fbdo.errorReason());
            }
            break;
            /*****************************************************************************/
        case UPDATE_EXIST:
            Serial.println("Sending Update Request to ECU2");    
            delay(1000);
            SerialPort1.write(REQEST_UPDATE); 
            while(1)
            {
                if(SerialPort1.available())
                {
                    temp=  SerialPort1.read();
                }
                if(temp==REQEST_ACCEPT || temp==REJECT_UPDATE)
                    break;
            }
            
            if(temp==REQEST_ACCEPT)
            {
              CURRENT_CASE = UPDATE_DOWN;
              Serial.println("Update Request is Accepted");

            }
            else if(temp==REJECT_UPDATE)
            {
                while(1)
                {
                    delay(1000);
                    Serial.println("Update Request is Rejected");
                }
            }
        
            break;

        case UPDATE_DOWN:
         /*****************************************************************************/
                    /*********************SEND HEADER**********************/
            Serial.println("Downloading the New Update Firmware");

            downloadAndParseJSON() ;
            
            header[0]=app_size & 0xff ;
            header[1]=(app_size>>8) & 0xff ;
            header[2]=(app_size>>16) & 0xff ;
            header[3]=(app_size>>24) & 0xff ;

            Serial.println("Sending the header New Update Firmware");                 


            SerialPort1.write(SEND_HEADER);
            delay(1000);
            for(int i=0 ; i<4 ;i++)
            {
                SerialPort1.write(header[i]);
                Serial.write(header[i]);
                delay(5000);

            }
            SerialPort1.write(FINISH_HEADER);
            while ( SerialPort1.available() == 0);
            if(SerialPort1.read() == ACK_HEADER)
            {
                CURRENT_CASE = SEND_CODE;


            }
            break;
        /*****************************************************************************/
                  /******************Sending Code****************************/
        case SEND_CODE:  

            SerialPort1.write(SEND_CODE);
            send_file();
            delay(1000);
            SerialPort1.write(FINISH_CODE);
            while(!SerialPort1.available());
            if(SerialPort1.read() == ACK_CODE)
            {
                Serial.println("Code is sent Successfully");
            } 
            EEPROM.writeFloat(0,version);
            EEPROM.commit();   // Required to actually save to flash!      
            while (1);

            break; 
        /**************************************************************************/
        case FAILED:
         /*****************************************************************************/
                    /*********************SEND HEADER**********************/
            Serial.println("Failed to Downloading the New Update Firmware");
            break;


        default:
            break;
    }
  
    
     /********************************************************************************************/
    
 
}