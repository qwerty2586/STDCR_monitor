#ifndef STIMULATOR_FILESERVER_TRANSFER_PROTOCOL_H
#define STIMULATOR_FILESERVER_TRANSFER_PROTOCOL_H

#include <cstdint>

// v protokolu znamena 62 znaku dlouhou zpravu
#define FULL_LENGTH_MESSAGE 0x3E


// v protokolu definovan jako posledni z bloku reserved
#define COMMUNICATION_OP_CODE 0xBF //TODO domluvit se na jinou konstantu

#define TRASFER_DATA_SIZE 60
// pri prijeti nevidim prvni dva byte musim je tedy odecist
#define PREFIX 2
// indexy bytu
#define INDEX_COMMAND           2
#define INDEX_ITER              3
#define INDEX_HELLO_VERSION     4
#define INDEX_RESPONSE          4
// prichozi zprava od nove prihlaseneho klienta
#define INDEX_HELLO_DATA        5
// data v nomalnim requestu
#define INDEX_DATA              4


// MD5 ma delku 16
#define HASH_SIZE               16

// na velikosti souboru a zaroven soucast hlavicek u prenosu je velikost uvadena 4 bajty
// poradi je Big-endian
#define SIZE_BYTES_COUNT        4
// na pocet polozek v LS 2 bajty, Big-Endian
#define LS_lENGHT_BYTES_COUNT   2

// SECTION_* se da pouzit na parsovani z COMMAND bytu pomoci &
#define SECTION_TYPE    0xC0
// to co posila klient serveru
#define TYPE_REQUEST    0x00
// to co posila server klientu
#define TYPE_RESPONSE   0x40
// presun dat smerem do klieta
#define TYPE_DOWNLOAD   0x80
// resun dat smerem od klienta
#define TYPE_UPLOAD     0xC0

// prenosy jsou vzdy uvozeny prikazy GET PUT LS GET_PREVIEW v pripade GET a PUT jeste i responsem s delkou a hashem


#define SECTION_PART    0x20
// zprave pokracuje dalsim paketem
#define PART_CONTINUE   0x00
// posledni paket zpravy
#define PART_LAST       0x20

#define SECTION_OP      0x1F

// v pripade transferu; zadny prikaz
#define OP_ZERO         0x00
// oznamuji serveru ze existuji zaroven testuji jestli tam vubec malina je
// 1. parametr verze protokolu, nechat 0
// 2. parametr jmeno kliena
#define OP_HELLO        0x01
// rozlouceni, zadny parametr
#define OP_BYE          0x02
// vytvoreni adresare, parametr cela vzdalena cesta
#define OP_MD           0x03
// listuje adresar jako parametr ma cestu k adresari ~ je domovsky adresar pro soubory
// v odpovedi posilam delku souboru a sha-1 hash, pak zahajim transfer
#define OP_LS           0x04
// stahne soubor parametr je nazev souboru na serveru vcetne cesty
// v odpovedi posilam delku souboru a sha-1 hash, pak zahajim transfer
#define OP_GET          0x05
// nahrava soubor, parametry: delka souboru(4 byte),sha-1(20 byte),nazev souboru na serveru
#define OP_PUT          0x06
// smaze polozku - prazdny adresar nebo soubor
#define OP_DEL          0x07
// startne proces s obrazem a zvuky na serveru, jako parametr je cesta ke config.xml
#define OP_START        0x08
// stopne proces
#define OP_STOP         0x09
// vraci soubor s nahledem obrazku, je ve formatu jpg s nizkym rozlisenim , prenos probiha stejne jako GET
#define OP_GET_PREVIEW  0x10

// operace probehla v poradku
// nenulova znamena ze se to nejak nepovedlo, soubor se nepodarilo ulozit, mas jinou verzi protokolu, hash nesedi
// response kody budu postupne doplnovat, tobe zatim staci !=0 je chyba
#define RESPONSE_OK     0x00

//dalsi response kody
#define RESPONSE_MD_DIR_EXIST       0x01
#define RESPONSE_MD_FAIL            0x02
#define RESPONSE_LS_DIR_NOT_FOUND   0x03
#define RESPONSE_DEL_FAIL           0x04
#define RESPONSE_PUT_MD5_FAIL      0x05
#define RESPONSE_GET_FILE_NOT_FOUND 0x06

// LS flagy
#define LS_FLAG_NODIRS              0x00
#define LS_FLAG_DIRS                0x01



/*
 Kazdy respose od serveru ma prakticky stejny format,
 stejny command jenom TYPE_RESPONSE bit a stejny ITER byte(ITER si urcuje klient)
 nasleduje response kod RESPONSE_OK pokud se povedlo

 Priklad navazujeme spojeni se serverem
 0. byte 0x3E - tim rikam stimulatoru ze zprava ma 62 znaku - cely paket
 1. byte 0xBF - kod pro stimulator aby predal zpravu
 2. byte 0x21 - TYPE_REQUEST + PART_LAST + OP_HELLO
 3. byte 0x00 - cislo prikazu ktere si urcuje klient sam muze byt cokoli, ale mel by si to iterovat
 4. byte 0x00 - verze protokolu
 5-63. "Nexus 5X\0" rika co je zac

 pokud se nevejde do jednoho paketu pak
 0x3E 0xBF 0x01 0x00 0x00 prvni cast jmena
 0x3E 0xBF 0x01 0x00 druha cast jmena
 0x3E 0xBF 0x21 0x00 treti cast jmena

 jako odpoved rekne server
 3E BF 61(RESPONSE+LAST+HELLO) 00(stejny jako v pozadavku) 00(RESPONSE_OK) "Raspbian ....\0"(to co je v uname -a)

 jiny priklad... stahuji soubor ze serveru
 3E BF 25 01 "dir/obrazek.png\0"
 server odpovi
 3E BF 65 01 00(OK) 000035bf(4byte delka souboru) cf23df2207d99a74fbe169e3eba035e633b65d94(md5)
 3E BF 80 01 DATA
 3E BF 80 01 DATA
 3E BF 80 01 DATA
 ...
 3E BF A0 01 DATA konec je vystlan nulami

 upload probehne takle
 3E BF 26 02 000035bf(4byte delka souboru) cf23df2207d99a74fbe169e3eba035e633b65d94(md5) "dir/obrazek.jpg\0"
 3E BF C0 02 DATA
 3E BF C0 02 DATA
 3E BF C0 02 DATA
 ...
 3E BF E0 02 DATA
 server odpovi
 3E BF 66 02 00
*/


#endif //STIMULATOR_FILESERVER_TRANSFER_PROTOCOL_H
