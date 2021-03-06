
 # Datový protokol
 
Tento dokument popisuje struktury datového protokolu.
Každý paket má na začátku dva bajty.
```
[FULL_LENGTH_MESSAGE][COMMUNICATION_OP_CODE]
```
Dále jsou popsány pakety o délkách 62.

 ## Typy paketů
 - *TYPE_REQUEST* odesílán z klienta, zahájí operaci.
 - *TYPE_RESPONSE* odpověď od serveru na REQUEST.
 - *TYPE_DOWNLOAD* data odesílaná ze serveru na klienta.
 - *TYPE_UPLOAD* data odesílaná z klienta na server.

 UPLOAD, DOWNLOAD jsou doprovodná data v případě jistých operací které vyžadují větší přenos.
 Jednotlivé zprávy se od sebě dělí zvýšením iterátoru ITER.

 ## Rozdělení paketů
Pokud je zapotřebí data rozdělit do více paketů, děje se to následujícím způsobem.


LONG REQUEST
```
[OP+TYPE_REQUEST+PART_CONTINUE][ITER]{ARGS1}
[OP+TYPE_REQUEST+PART_CONTINUE][ITER]{ARGS2}
[OP+TYPE_REQUEST+PART_LAST][ITER]{ARGS3}
```


LONG RESPONSE
```
[OP+TYPE_RESPONSE+PART_CONTINUE][ITER]{ARGS1}
[OP+TYPE_RESPONSE+PART_CONTINUE][ITER]{ARGS2}
[OP+TYPE_RESPONSE+PART_LAST][ITER]{ARGS3}
```


LONG DOWNLOAD
```
[TYPE_DOWNLOAD+PART_CONTINUE][ITER]{DATA1}
[TYPE_DOWNLOAD+PART_CONTINUE][ITER]{DATA2}
[TYPE_DOWNLOAD+PART_LAST][ITER]{DATA3}
```


LONG UPLOAD
```
[TYPE_UPLOAD+PART_CONTINUE][ITER]{DATA1}
[TYPE_UPLOAD+PART_CONTINUE][ITER]{DATA2}
[TYPE_UPLOAD+PART_LAST][ITER]{DATA3}
```


## Popis operací

Řetězce jsou ukončené nulovým bajtem.
V případě použití zprávy s DOWNLOAD nebo UPLOAD přenosem, tento přenos je zabezpečen MD5 hashem,
který je součástí REQUEST nebo RESPONSE.

 HELLO
 
```
 [OP_HELLO+TYPE_REQUEST][ITER][HELLO_VER]{NAME}[0]
```
 
 HELLO_RESPONSE
```
 [OP_HELLO+TYPE_RESPONSE][ITER][RESPONSE_OK]{NAME}[0]
``` 
BYE
```
 [OP_BYE+TYPE_REQUEST][ITER]
 ```
BYE_RESPONSE
```
[OP_BYE+TYPE_RESPONSE][ITER][RESPONSE_OK]
```
 MD
```
[OP_MD+TYPE_REQUEST][ITER]{DIR_PATH}[0]
```
 MD_RESPONSE
```
[OP_MD+TYPE_RESPONSE][ITER][RESPONSE_OK]
or
[OP_MD+TYPE_RESPONSE][ITER][RESPONSE_MD_DIR_EXIST]
or
[OP_MD+TYPE_RESPONSE][ITER][RESPONSE_MD_FAIL]
```
   
 LS
 
 ```
 [OP_LS+TYPE_REQUEST][ITER][FLAGS]{DIR_PATH}[0]{MASK}[0]
 ```
 
 LS_RESPONSE
 ```
 [OP_LS+TYPE_RESPONSE][ITER][RESPONSE_OK][4:DATA_SIZE][16:DATA_MD5]
 [TYPE_DOWNLOAD][ITER][2:COUNT][4:FILE1_SIZE][16:FILE1_MD5]{FILE1_NAME}[0][4:FILE2_SIZE][16:FILE2_MD5]{FILE2_NAME}[0]...
 or 
 [OP_LS+TYPE_RESPONSE][ITER][RESPONSE_LS_DIR_NOT_FOUND]
 ```
 
  GET
  
  ```
  [OP_GET+TYPE_REQUEST][ITER]{FILE_PATH}[0]
  ```
 
 GET_RESPONSE
  ```
  [OP_GET+TYPE_RESPONSE][ITER][RESPONSE_OK][4:DATA_SIZE][16:DATA_MD5]
  [TYPE_DOWNLOAD][ITER]{FILE_BYTES}
  or
  [OP_GET+TYPE_RESPONSE][ITER][RESPONSE_GET_FILE_NOT_FOUND]
  ```
  
 GET_PREVIEW
  ```
  [OP_GET_PREVIEW+TYPE_REQUEST][ITER]{FILE_PATH}[0]
  ```
   
 GET_PREVIEW_RESPONSE
 ```
 [OP_GET_PREVIEW+TYPE_RESPONSE][ITER][RESPONSE_OK][4:DATA_SIZE][16:DATA_MD5]
 [TYPE_DOWNLOAD][ITER]{PREVIEW_FILE_BYTES}
 ```
 
 PUT
 ```
 [OP_PUT+TYPE_REQUEST][ITER][4:DATA_SIZE][16:DATA_MD5]{FILE_PATH}[0]
 [TYPE_UPLOAD][ITER]{FILE_BYTES}
 ```
 PUT_RESPONSE
 ```
 [OP_PUT+TYPE_RESPONSE][ITER][RESPONSE_OK]
 or
 [OP_PUT+TYPE_RESPONSE][ITER][RESPONSE_PUT_MD5_FAIL]
 ```  
 
 DEL
 ```
 [OP_DEL+TYPE_REQUEST][ITER]{FILE_PATH}[0]
 ```
 DEL_RESPONSE
 ```
 [OP_DEL+TYPE_RESPONSE][ITER][RESPONSE_OK]
 or
 [OP_DEL+TYPE_RESPONSE][ITER][RESPONSE_DEL_FAIL]
 ```  
 START
 ```
 [OP_START+TYPE_REQUEST][ITER]{CONFIG_PATH}[0]
 ```
 START_RESPONSE
 ```
 [OP_START+TYPE_RESPONSE][ITER][RESPONSE_OK]
 ```
 STOP
 ```
 [OP_STOP+TYPE_REQUEST][ITER]
 ```
 STOP_RESPONSE
 ```
 [OP_STOP+TYPE_RESPONSE][ITER][RESPONSE_OK]
 ```
 
## Konstanty

 ```
FULL_LENGTH_MESSAGE         0x3E
COMMUNICATION_OP_CODE       0xEF

TYPE_REQUEST                0x00
TYPE_RESPONSE               0x40
TYPE_DOWNLOAD               0x80
TYPE_UPLOAD                 0xC0

PART_CONTINUE               0x00
PART_LAST                   0x20

OP_HELLO                    0x01
OP_BYE                      0x02
OP_MD                       0x03
OP_LS                       0x04
OP_GET                      0x05
OP_PUT                      0x06
OP_DEL                      0x07
OP_START                    0x08
OP_STOP                     0x09
OP_GET_PREVIEW              0x10

RESPONSE_OK                 0x00
RESPONSE_MD_DIR_EXIST       0x01
RESPONSE_MD_FAIL            0x02
RESPONSE_LS_DIR_NOT_FOUND   0x03
RESPONSE_DEL_FAIL           0x04
RESPONSE_PUT_MD5_FAIL       0x05
RESPONSE_GET_FILE_NOT_FOUND 0x06
 ```
 