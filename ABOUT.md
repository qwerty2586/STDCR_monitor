# Stimulator Control
Stimulator control je konfigurační utilita pro nastavení 
neurostimulačních experimentů na stimulátoru pro neuroinformatické experimenty vyvíjeném na ZČU. 
Program je určený k běhu na Raspberry PI připojeném ke stimulátoru přez UART a GPIO sběrnici. Projekt sestává ze 
dvou částí
 
  -  stimulator_control
    - umožňuje nastavení stimulátoru, komunikaci s mobilní verzí programu, spuštení SDL_Output
    
  - SDL_Output - rozšiřuje stimulátor o možnost zobrazení/přehrání obrázků 
  a zvuků na HDMI monitoru připojeném k Raspberry PI  
  
## stimulator_control
Program je napsán v C++ s pomocí toolkitu Qt verze 4. Qt je rozsáhlá knihovna 
jejíž primární účel je grafické rozhraní programu ale svým rozměrem značně rozšiřuje možnosti jazyka C++.
Například umožňuje propojení objektů na bázi takzvaných "signálů a slotů" a nabízí tak příjemnou alternativu k 
nepříjemným callbackům z jiných jazyků.

![Obrázek slotů a signálů](http://doc.qt.io/qt-4.8/images/abstract-connections.png)

Signály byli použity v programu k zachytávání uživatelského vstupu(tlačítka, zadávací pole) a komunikaci 
mezi částmi programu.  

Komunikaci se stimulátorem po seriové lince je realizována pomocí knihovny [qextserialport](https://qextserialport.github.io/)
a fronta zpráv je realizována pomocí signálů a slotů.

Součástí programu je i souborový server umožňující vzdálenou správu souborů v lokálním adresáři. 
Umožňuje tak nahrávání a stahování souborů mobilní variantě programu [Remote stimulator control](https://github.com/stechy1/Remote-stimulator-control)
 Tento program vyvíjí Petr Štechmüller pro platformu Android. Komunikační protokol je navržen 
 tak aby vyžadoval minimální zásah do původního protokolu stimulátoru a je hrubě nastíněn 
 [zde](https://github.com/qwerty2586/stimulator_control/blob/master/stimulator_fileserver/transfer_protocol.h)
 a [zde](https://github.com/qwerty2586/stimulator_control/blob/master/stimulator_fileserver/data_packets.md).
Důvodem k implementaci tohoto souborového serveru bylo zpřístupnéní mobilnímu programu přístup ke konfiguraci a spuštění SDL_Output.
 
 ##SDL_Output
 

