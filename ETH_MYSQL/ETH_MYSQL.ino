/*
  Arduino leonardo eth v3 and mysql 
  This example demonstrates how to store data in a
  table in mysql from 4 button connected to pins 2, 3, 4, 5 
  For this, we will create a special database and table for testing.
  The following are the SQL commands you will need to run in order to setup
  your database for running this sketch.

  CREATE DATABASE machine;
  CREATE TABLE machine.state (
    num integer primary key auto_increment,
    message char(40),
    recorded timestamp
  );

  Here we see one database and a table with three fields; a primary key that
  is an auto_increment, a string, and a timestamp. This will demonstrate how
  to save a date and time of when the row was inserted, which can help you
  determine when data was recorded or updated.

  INSTRUCTIONS FOR USE

  1) Create the database and table as shown above.
  2) Change the address of the server to the IP address of your MySQL server in my case my IP is 192.186.157.77
  3) Change the user and password to a valid MySQL user and password in my case it is sa and 156444
  4) install mysql library in your ide go to sketch > include library > manage library
  and in the filter your search type mysql then install it
  5) arduino leonardo use etehrnet2 library so you need to install it in your ide go to sketch > include library > manage library
  and in the filter your search type ethernet2 then install it
  6) also you need to modify this file MySQL_Packet.h the original created by Dr.Charles is - include <Ethernet.h> 
  the new is - include <Ethernet2.h> otherwise it will give your error message and not compiled  
  7) Connect a USB cable to your Arduino
  8) Select the arduino leonardo eth board and port 
  9) Compile and upload the sketch to your Arduino
  10) connect 4 push button between ground and pins 2, 3, 4, 5 through 10k Ohm resistor 
  11) After the sketch has run for some time , press some push buttons and open a mysql client and issue
     the command: SELECT * FROM machine.state; to see the data
     recorded. Note the field values and how the database handles both the
     auto_increment and timestamp fields for us. You can clear the data with.
      "DELETE FROM machine.state".

  Note: The MAC on your board in back if not you can put any thing but make sure it is unique in your network .
  Note your mysql default port address is 3306 but this will conflict with Skype so you can change it to 3307 in both your 
  sketch and your Mysql and make sore this port is open in your server firewall .

  Created by: Samir Mohamad tawfik 28-1-2018
  samirtwf@gmail.com
*/

#include <Ethernet2.h>
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>

byte mac_addr[] = { 0x90, 0xA2, 0xDA, 0x10, 0xBD, 0x8C };

IPAddress server_addr(192,168,1,100);  // IP of the MySQL *server* here
char user[] = "gateway";              // MySQL user login username
char password[] = "password";        // MySQL user login password

EthernetClient client;
MySQL_Connection conn((Client *)&client);

void setup() {
  EthernetClass(mac_addr);
   if (conn.connect(server_addr, 3306, user, password)) {
    delay(1000);
  }
 }
void loop() {
  delay(1000);  
  char INSERT_SQL[] = "INSERT INTO machine.state (message) VALUES ('machine one on')";
  MySQL_Cursor *cur_mem = new MySQL_Cursor(&conn);
  cur_mem->execute(INSERT_SQL);
  delete cur_mem;
  delay(1000); 
 }
