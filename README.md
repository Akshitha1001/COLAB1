# COLAB
Code for NODEMCU
## *HC-SR05 and HC-SR04* 
This folders contain code usage of HC-SR05 sensor as per our requirements

---
## *server*
This folder contains server code made using Flask and tests code 


*secret key* : **abcdef**

if depth is >5 then switch value is 1 (response)
### GET request
*url* : http://jayashankar.pythonanywhere.com/abcdef/depth?d=9

    *response* : {
                    "switch":1
                } ->status of switch


### POST request
*url* : http://jayashankar.pythonanywhere.com/abcdef/depth

    *body* : {
                "depth" : 2
            }
    
*response* : {
    "switch":0
                } ->status of switch 
->it stores the data recieved into the database and responds back
    
### Database:

|Field  | Type  | Null   | Key  |Default   |Extra |
|---|---|---|---|---| ---|
 id     | int(11)    | NO   | PRI | NULL    | auto_increment |
| time   | datetime   | YES  |     | NULL    |         fds       |
| depth  | float      | YES  |     | NULL    |        fs        |
| switch | tinyint(1) | YES  |     | NULL    |        s        |

---

## *NodeMCU_Redirects*
This folder contains code for making GET and POST to our spreadsheet

in NodeMCU_Redirect.ino file code
in setup we are connecting our our board our wifi newtwork
in loop only one time we a request to our server to establish a connection 
once the connection in esatblished we can make a requests to server in this ways

### GET request

    client->GET(url, host);
    String response = client->getResponseBody();
where response stores the returned data in the form of String

### POST request

    client->POST(url, host, payload, false);
    String response = client->getResponseBody();   
where payload is the data which we want to send to server

the response from POST request is whether to switch ON or OFF based on the depth of the water

    payload= {
                "command": "appendRow",
                "sheet_name": "Sheet1", 
                "depth":<value> 
             }
To check out how the data is getting stored in spread follow the link : https://docs.google.com/spreadsheets/d/12ZrdO3mdJyoQZkD3svH5aNOiOGL5sYk6pUXFRYs-xjw/edit?usp=sharing

				
Row no | Time | Depth | Power	
------ | ---- | ----- | -----
1236 |	20:9:12. |	5 |	0	
1237 | 	20:9:19. |	5 |	0	
1238 |	20:9:32. |	7 |	0	
1239 |	20:9:58. |	11 | 1	
1240 |	20:10:11. |	13 | 1	



## *appScripts*
This folder contains  appScripts for our SpreadSheets

in time_script1.txt  for testing purppose we can cacluate the row using the formula:

    var rowNumber = hour*60*4 - roundedMins*4 - roundedSeconds;
