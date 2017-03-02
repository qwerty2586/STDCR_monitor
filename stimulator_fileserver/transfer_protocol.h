#ifndef STIMULATOR_FILESERVER_TRANSFER_PROTOCOL_H
#define STIMULATOR_FILESERVER_TRANSFER_PROTOCOL_H

#define FULL_LENGTH_MESSAGE 0x3E
// v protokolu znamena 62 znaku dlouhou zpravu

#define COMMUNICATION_OP_CODE 0xBF
// v protokolu definovan jako posledni z bloku reserved


// indexy bytu
#define INDEX_COMMAND           2
#define INDEX_ITER              3
#define INDEX_HELLO_VERSION     4
#define INDEX_RESPONSE          4

#define SECTION_TYPE    0xC0
#define TYPE_REQUEST    0x00
// to co posila klient serveru
#define TYPE_RESPONSE   0x40
// to co posila server klientu
#define TYPE_DOWNLOAD   0x80
// smerem do klieta
#define TYPE_UPLOAD    0xC0
//smerem od klienta
// prenosy jsou vzdy uvozeny prikazy GET PUT LS v pripade GET a PUT jeste i responsem s delkou a hashem

#define SECTION_PART    0x20
#define PART_CONTINUE   0x00
#define PART_LAST       0x20

#define SECTION_OP      0x1F
#define OP_ZERO         0x00 // v pripade
#define OP_HELLO        0x01
// oznamuji serveru ze existuji zaroven testuji jestli tam vubec malina je
// 1. parametr verze protokolu, nechat 0
// 2. parametr jmeno kliena
#define OP_BYE          0x02
// zadny parametr
#define OP_MD           0x03
// vytvoreni adresare, parametr cela vzdalena cesta
#define OP_LS           0x04
// listuje adresar jako parametr ma cestu k adresari ~ je domovsky adresar pro soubory
// v odpovedi posilam delku souboru a sha-1 hash, pak zahajim transfer
#define OP_GET          0x05
// stahne soubor parametr je nazev souboru na serveru vcetne cesty
// v odpovedi posilam delku souboru a sha-1 hash, pak zahajim transfer
#define OP_PUT          0x06
// nahrava soubor, parametry: delka souboru(4 byte),sha-1(20 byte),nazev souboru na serveru
#define OP_DEL          0x07
// smaze polozku - prazdny adresar nebo soubor
#define OP_START    0x08
// startne proces s obrazem a zvuky na serveru, jako parametr je cesta ke config.xml
#define OP_STOP 0x09
// stopne proces
#define OP_GET_PREVIEW 0x10
// vraci soubor s nahledem obrazku, je ve formatu jpg s nizkym rozlisenim , prenos probiha stejne jako GET

#define RESPONSE_OK     0x00
// operace probehla v poradku
// nenulova znamena ze se to nejak nepovedlo, soubor se nepodarilo ulozit, mas jinou verzi protokolu, hash nesedi
// response kody budu postupne doplnovat, tobe zatim staci !=0 je chyba

/*
 Kazdy respose od serveru ma prakticky stejny format,
 stejny command jenom TYPE_RESPONSE bit a stejny ITER byte(ITER si urcuje klient)

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
 3E BF 65 01 00(OK) 000035bf(4byte delka souboru) cf23df2207d99a74fbe169e3eba035e633b65d94(sha1)
 3E BF 80 01 DATA
 3E BF 80 01 DATA
 3E BF 80 01 DATA
 ...
 3E BF A0 01 DATA konec je vystlan nulami

 upload probehne takle
 3E BF 26 02 000035bf(4byte delka souboru) cf23df2207d99a74fbe169e3eba035e633b65d94(sha1) "dir/obrazek.jpg\0"
 3E BF C0 02 DATA
 3E BF C0 02 DATA
 3E BF C0 02 DATA
 ...
 3E BF E0 02 DATA
 server odpovi
 3E BF 66 02 00*
*/


#endif //STIMULATOR_FILESERVER_TRANSFER_PROTOCOL_H
