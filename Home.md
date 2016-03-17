# 	Obsah

[1. Úvod](https://github.com/neurofishteam/STDCR_monitor/wiki#1----%C3%9Avod)

[2. Obecný popis](https://github.com/neurofishteam/STDCR_monitor/wiki#2----obecn%C3%BD-popis)

[3. Funkce systému](https://github.com/neurofishteam/STDCR_monitor/wiki#3----funkce-syst%C3%A9mu)

[4. Požadavky na vnější rozhraní](https://github.com/neurofishteam/STDCR_monitor/wiki#4----po%C5%BEadavky-na-vn%C4%9Bj%C5%A1%C3%AD-rozhran%C3%AD)

[5. Další parametrické (mimofunkční) požadavky](https://github.com/neurofishteam/STDCR_monitor/wiki#5----dal%C5%A1%C3%AD-parametrick%C3%A9-mimofunk%C4%8Dn%C3%AD-po%C5%BEadavky)

[6. Ostatní požadavky](https://github.com/neurofishteam/STDCR_monitor/wiki#6----ostatn%C3%AD-po%C5%BEadavky)


### Verze dokumentu 1.0


# 1.	Úvod

## 1.1	Předmět specifikace

Cílem projektu je navrhnout a implementovat aplikaci pro linux, která bude umožňovat do grafického rozhraní zadávat parametry a ovládat stimulátor pro neuroinformatické experimenty vytvořený na KIV. Komunikace se stimulátorem bude vedena přes Bluetooth, případně přes sériový port. Aplikace bude umožňovat zadávání parametrů stimulace, ovládání stimulátoru (spuštění a zastavení stimulace) a zobrazení výsledků některých experimentů (reakční doba měřené osoby).

Stimulátor je používán pro psychologický výzkum a klinickou diagnostiku. Je vybaven generátorem signálů pro synchronizaci s počítačem, na kterém jsou zaznamenávány vyšetřované mozkové aktivity.

## 1.2	Cílové publikum, návod ke čtení

Specifikace slouží k popisu celého produktu. Primárně slouží jako dohoda mezi zadavatelem Ing. Pavlem Mautnerem, Ph.D. a týmem. Další osobou, které je tento dokument určen, je přednášející a cvičící předmětu KIV/ZSWI Ing. Roman Mouček, Ph.D.

## 1.3	Rozsah projektu

Naším úkolem je vytvořit grafické rozhraní v linuxu pro ovládání stimulátoru. Definovat a implementovat přenos parametrů mezi aplikací a stimulátorem přes sériové rozhraní a Bluetooth.

## 1.4	Odkazy

•	hardware stimulator - [neuroinformatics.kiv.zcu.cz](http://neuroinformatics.kiv.zcu.cz/articles/read/hardware-stimulator-for-cognitive-research_2015-01-15)

•	Raspberry Pi 2 - [raspberrypi.org](https://www.raspberrypi.org/products/raspberry-pi-2-model-b/)

•	Qt 4 knihovna - [qt.io](http://www.qt.io/)

•	protokol pro přenos - [protokol](https://github.com/neurofishteam/STDCR_monitor/blob/master/novy_protokol.pdf)

# 2.	Obecný popis

## 2.1	Kontext systému

Cílem aplikace je nahradit dotykový displej umístěný na stimulátoru. Aplikace bude umožňovat snadné a přehledné zadávání parametrů pro stimulátor.


![vztahy systému](https://github.com/neurofishteam/STDCR_monitor/blob/master/obrazek_2.1.png)

## 2.2	Funkce produktu

•	snadné a přehledné ovládání stimulátoru, 

•	komunikace stimulátoru s počítačem Raspberry Pi 2. 

## 2.3	Třídy uživatelů

•	ODBORNÍK - osoba, která rozumí problematice a využije zaznamenaných dat, případně data pomocí aplikace pouze zaznamená.

## 2.4	Provozní prostředí

Aplikace bude provozována na počítači Raspberry Pi 2 s linuxovým systémem. Parametry hardwaru jsou následující: procesor ARM Broadcom BCM2836 (0.9 GHz) a 1 GB RAM.
Na počítači je vyžadována přítomnost Bluetooth modulu a podpora frameworku Qt 4. 
Aplikace je primárně vyvíjena pro potřeby výzkumu na NTIS v Plzni. 

## 2.5	Omezení návrhu a implementace

Aplikace musí být psána v jazyce C++ s knihovnou Qt 4. Přenos parametrů je určen protokolem pro stimulátor. 

## 2.6	Uživatelská dokumentace 

Uživatelská dokumentace bude obsahovat návod k ovládání programu pro běžné užívání. Dokumentace bude dodána v pdf souboru při odevzdání produktu.

## 2.7	Předpoklady a závislosti

Aplikace je primárně vyvíjena pro linuxový operační systém, podpora jiných operačních systémů není zajištěna. Vyžadována je přítomnost Qt 4 frameworku. 

# 3.	Funkce systému

Funkce jsou řazeny podle priorit zadavatele a nutnosti splnění požadavku.

Priorita vysoká - nejvyšší priorita, požadavek musí být splněn

Priorita střední - práce na těchto funkcích začíná až po splnění požadavků s vysokou prioritou

## 3.1	Nastavení parametrů stimulátoru

### 3.1.1	Popis a priorita

Nastaví parametry stimulátoru přes rozhraní Bluetooth nebo sériový port. Parametry budou zadávány do GUI aplikace. Priorita vysoká. 

###  3.1.2	Události a odpovědi

Nastavení a uložení parametrů a následné odeslání do stimulátoru. 

###  3.1.3	Funkční požadavky

•	POŽADAVEK-1: komunikace pomocí Bluetooth 
	
•	POŽADAVEK-2: komunikace pomocí sériového portu 

•	POŽADAVEK-3: vytvoření GUI aplikace

## 3.2	Uložení naměřených dat

### 3.2.1	Popis a priorita

Funkce bude ukládat naměřená data do souboru. Priorita střední.

### 3.2.2	Události a odpovědi

Aplikace uživateli umožní uložení přijatých dat do libovolného textového souboru. 

### 3.2.3	Funkční požadavky

•	POŽADAVEK-1: uložení získaných dat do xml souboru pro další zpracování
	 
## 3.3	Uložení a načtení schémat nastavení stimulátoru 

### 3.3.1	Popis a priorita

Funkce umožní uložit a načíst nastavení stimulátoru pro jednotlivé testy. Priorita střední.

### 3.3.2	Události a odpovědi

Aplikace uživateli umožní uložení a načtení nastavení stimulátoru pro jednotlivé testy.
	
### 3.3.3	Funkční požadavky

•	POŽADAVEK-1: uložení nastavení 	

•	POŽADAVEK-2: načtení nastavení   

# 4.	Požadavky na vnější rozhraní 

## 4.1	Uživatelská rozhraní

Uživatelské rozhraní bude obsahovat sadu displejů:

•	hlavní menu pro výběr požadovaného testu, 

•	displej pro nastavení ERP testu - obsahuje možnost přidat více LED, 

•	displej pro nastavení BCI testů (f-VEP, t-VEP, c-VEP),

•	displej pro nastavení testu reakční doby a průběhu testu,

•	displej pro testování stavu stimulátoru.

## 4.2	Hardwarová rozhraní 

Stimulátor pro měření experimentů - [popis stimulátoru](https://github.com/neurofishteam/STDCR_monitor/blob/master/Stimulation%20device%20for%20cognitive%20research.pdf)
## 4.3	Softwarová rozhraní

•	C++

•	Knihovna Qt

## 4.4	Komunikační rozhraní

•	protokol pro přenos - [protokol](https://github.com/neurofishteam/STDCR_monitor/blob/master/novy_protokol.pdf)

•	Bluetooth

•	sériový port

# 5.	Další parametrické (mimofunkční) požadavky

## 5.1	Výkonnostní požadavky

Omezení je dáno výkonem Raspberry Pi 2, který byl popsán v odstavci [2.4](https://github.com/neurofishteam/STDCR_monitor/wiki#24--provozn%C3%AD-prost%C5%99ed%C3%AD)

## 5.2	Bezpečnostní požadavky

Bezpečnostní požadavky nejsou kladeny. 

## 5.3	Kvalitativní parametry

•	intuitivní a přehledné ovládání,

•	zabránění zadání špatných parametrů.

# 6.	Ostatní požadavky

##	Dodatek A: Slovníček

•	Elektroencefalografie (EEG) - diagnostická metoda pro vyšetřování mozkové aktivity. Snímajícími elektrodami může být sledován elektrický potenciál mozku. 

•	Evokované potenciály (ERP) - elektrofyziologické odpovědi centrálního nervového systému na různé druhy stimulů. 

•	Brain-computer interface (BCI) - umožňuje lidem komunikovat a ovládat vnější svět bez cesty prostřednictvím mozku a periferních nervů a svalů