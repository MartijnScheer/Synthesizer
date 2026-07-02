# PROJECT SYNTHESIZER
<img width="312" height="207" alt="image" src="https://github.com/user-attachments/assets/b2ea4d3d-0c79-49ab-b317-6fab1102fb9d" />

# Gemaakt door:
**PIB 2024/2025:**
* Patrick van Ommen & Rick Smelt (INPUT)
* Roelof Zijda & Relnout strating (SEQUENCER)
* Thijn Berkhoff & Ralph Hänninger (VCO)
* Ruben Boeve (LFO)
* Ingmar Kosmeijer & Hidde Huysman (ADSR + MIXER + VCA)
* Jens Kuizenga & Diémo Rijsdijk (FILTER)
* Jeroen Middel & Matthijs vos (POWER AMPLIFIER + POWER SUPPLY)
* Roelof Zijda (BEHUIZING)

**Project elektronica 2026: **
* Martijn Scheer (REVAMP INPUT MODULE +  & DOCUMENTATIE) 
* Hidde Huysmans (REVAMP ADSR + MIXER + VCA & DOCUMENTATIE)

Welkom bij de wiki van de synthesizer! Het grote idee is om verouderde documentatie hier te vervangen met de actuele. Als stukjes niet meer van toepassing zijn, haal ze lekker weg! Dat houd het overzichtelijk voor de groepjes in de volgende projecten.  

# Introductie

Tijdens het PIB-project van 2024/2025 is er een synthesizer gemaakt. Aan het einde van het project werkten echter nog niet alle onderdelen zoals bedoeld en was de documentatie nogal een zooitje. De documentatie was verspreid opgeslagen en ontbrak er een centrale plek waar alle informatie terug te vinden was. Hierdoor stond de synthesizer uiteindelijk ongebruikt in het lab en werd er niet meer aan verder gewerkt.

In 2026 is daarom, onder begeleiding van Bart Snijder, tijdens de minor elektronica een vervolgproject gestart. Het doel van dit project is om de bestaande synthesizer te herstellen en te verbeteren. Daarbij wordt eerst alle beschikbare documentatie verzameld en geplaatst in deze openbare GitHub (mapje documentatie 2024/2025), en de belangrijkste / meest actuele informatie gefilterd in deze wiki zodat toekomstige groepen eenvoudig verder kunnen werken aan het project.

Daarnaast worden de onderdelen die nog niet werken onderzocht en gerepareerd, problemen waar wij niet aan toe gekomen zijn staat onder issues, en het lijkt ons handig als die traditie wordt doorgevoerd voor goed overzicht! Het uiteindelijke doel van het 2026 project is om de synthesizer weer werkend te krijgen en daadwerkelijk geluid te laten produceren. 

![alt](https://www.image2url.com/r2/default/images/1781090389377-c58cb061-24d6-4a19-b808-d10436d3d412.jpeg)

# Uitleg per module

# Module 1: ADSR Envelope generator
<img width="529" height="568" alt="image" src="https://github.com/user-attachments/assets/eeb63e0a-23df-4d8e-8751-d5897221b8f7" />

Een ADSR envelope generator bepaalt hoe een geluid zich in de tijd ontwikkelt. Je kunt het zien als de “vorm” van een toon vanaf het moment dat een toets wordt ingedrukt tot het moment dat het geluid helemaal wegsterft. Dit is een belangrijk onderdeel van geluidssynthese, omdat het sterk bepaalt hoe natuurlijk, strak of elektronisch een geluid klinkt.
De ADSR-module is een envelope generator die een variërende stuurspanning genereert op basis van een gate-signaal van het keyboard.

ADSR staat voor Attack, Decay, Sustain & Release.

## Werking

Gate-trigger: Het gate-signaal van +3.3V wordt met transistoren omgezet naar een negatieve puls voor de trigger-pin van de NE555 timer.
Hierdoor start de envelope.

Attack: Een condensator laadt op via de attack-potmeter.
Hoe kleiner de weerstand -> hoe sneller de condensator oplaadt -> hoe sneller de uitgangsspanning stijgt

De spanning stijgt richting 8V intern en 5V aan de uitgang

Decay: Wanneer de condensatorspanning de threshold van de 555 bereikt:
schakelt het IC over
begint de condensator gedeeltelijk te ontladen
De spanning daalt naar het sustain-niveau.

Sustain: Tijdens sustain blijft de spanning constant zolang de gate actief blijft.
Het sustain-niveau bepaalt hoe hoog het uitgangssignaal blijft tijdens het vasthouden van een toets.

Release: Wanneer het gate-signaal terug naar 0V gaat: reset de 555 timer en 
ontlaadt de condensator via de release-potmeter
Hierdoor daalt de uitgangsspanning langzaam terug naar 0V.


afzonderlijke regeling van Attack, Decay, Sustain en Release
correcte envelope-vorm
instelbare timing en sustain-niveaus

## In- en uitgangssignalen

### Ingangen
Gate input: 0V / +3.3V
Voeding: ±12V
Potmeters: variabele weerstand

### Uitgangen
Envelope output: 0V – 5V
Interne condensatorspanning: 0V – 8V

<img width="1069" height="477" alt="image" src="https://github.com/user-attachments/assets/8cb0fb94-fc95-4b3c-94cb-d627c811f5d8" />

# Module 2: Mixer
Mengt verschillende audio signalen

## Werking
 De mixer is nodig om audiosignalen, afkomstig van andere modules (zoals een LFO) en optionele externe bronnen, samen te voegen en vervolgens door te sturen naar de VCA
De schakeling is opgebouwd als een inverterende sommator, waarbij drie ingangen via 100kΩ-weerstanden worden samengebracht op de inverterende ingang van de opamp. Samen met de terugkoppelweerstand ook 100kΩ zorgt dit voor een gelijke versterking van 1 per kanaal. De signalen worden lineair gecombineerd en aan de uitgang geïnverteerd, zonder versterking of verzwakking.

## In- en uitgangssignalen
### Ingangen
3X 0 - 5V
### Uitgangen (alleen via de VCA!)
1X gemengd 0 - 5V


# Module 3: VCA (Voltage controlled amplifier)
Een VCA(Voltage-Controlled Amplifier) is een module in een synthesizer die het geluidsniveau (amplitude) regelt op basis van een stuurspanning bijvoorbeeld de omhullende gegenereerd door de ADSR envelope generator. Een VCA wordt gebruikt om de intensiteit van een geluidssignaal te moduleren. De VCA versterkt de geluidssignalen die al gemengd zijn in de mixer. 

## Werking 

Dit design maakt gebruik van ‘Long tailed pair’  geschakelede NPN transistoren, toegepast in deze vorm wordt een verschilversterker gecreëerd. (zie foto schema hieronder)
Een LTP verschilversterker bestaat uit twee NPN transistoren welke via hun emitters verbonden worden met een stroombron, in ons geval dient een stuurspanning met een serieweerstand als stroombron(zie R1 in het LTP schema). De collectors van de transistoren worden gezamenlijk gevoed via belastingsweerstanden naar de voedingsspanning(zie R2 en R3 in het LTP schema). De totale stroom blijft constant, de stroom per transistor is afhankelijk van het spanningsverschil tussen de basissen, dit is ook de input. Op de ene base zit V_(in-) en op de andere V_(in+ ).	 De output zit op de collectors, hoe groter het stroomverschil, hoe groter de spanning op de uitgang.

<img width="324" height="335" alt="image" src="https://github.com/user-attachments/assets/42c2437e-2b7b-4adf-bc76-2adcbdc817af" />
### Ingangen
Output van ADSR  0 - 8V

### Uitgangen

Envelope output: 0V – 5V (20 - 20kHz)

<img width="509" height="225" alt="image" src="https://github.com/user-attachments/assets/f577598d-6d01-42d3-9870-5cc318b72634" />

#Module 3: VCO (Voltage Controlled Oscillator)

De VCO module is ontwikkeld door Thijn Berkhoff en Ralph Hänninger tijdens het PIB project van 2024/2025. De VCO zet de inkomende stuurspanning (CV) om in een signaal met een bijbehorende frequentie. De frequentie reageert volgens de 1V/octaaf standaard, waardoor de module direct aangestuurd kan worden door het keyboard van de synthesizer, kijk bijvoorbeeld ook naar de afbeelding van de input module, tussen elk octaaf zit een volt verschil!

## Werking

<img width="253" height="336" alt="image" src="https://github.com/user-attachments/assets/a8d54021-5d08-4e27-a29a-64dd504702ce" />

Het ontwerp is opgebouwd uit drie hoofdonderdelen: de oscillator, de inputschakeling en het modulatiegebied. De kern van de module bestaat uit een zaagtandoscillator op basis van een comparator en integrator door middel van opamps. Door gebruik te maken van een converter met transistoren reageert de frequentie exponentieel op de ingangsspanning, waardoor de gewenste 1V/octaaf wordt bereikt.

Vanuit de zaagtand worden vervolgens meerdere golfvormen gegenereerd. Door middel van een inverter en comparator kunnen naast de zaagtand ook een geïnverteerde zaagtand en een blokgolf worden geselecteerd. Daarnaast beschikt de module over schakelaars voor verschillende octaafbereiken en een "finetune" regeling waarmee de frequentie nauwkeurig kan worden afgesteld.

Het oorspronkelijke ontwerp was bedoeld als een dual VCO, waarbij twee oscillatoren op één module aanwezig zijn. Beide kanalen beschikken over een eigen ingang, uitgang, octaafschakelaar en finetune regeling. Hierdoor kunnen twee verschillende signalen tegelijkertijd worden gegenereerd en gebruikt binnen de synthesizer. Dit werkt momenteel helaas nog niet, maar is wel essentieel voor het maken van een polyfoon systeem.

De module is momenteel opgebouwd op breadboard, alleen de output 2 is aangesloten omdat er geen ruimte was voor een extra output op de module, het maken van de module op een PCB is dus erg aangeraden!

## In- en uitgangssignalen
### Ingangen
CV Spanning: 0 tot 3.3V (zie input module)
Voeding: ±12V

### Uitgangen
VCO output: tussen -2.2v en 2.2v

Output frequentie: ligt aan de finetune en octaaf offset instellingen. Meestal rond de 50hz (nog even nameten!)

<img width="379" height="306" alt="image" src="https://github.com/user-attachments/assets/bbc2506e-252d-4206-b5eb-ec150ac0c23c" />

# Module 4: Input Module

De input module is oorspronkelijk ontwikkeld door Patrick van Ommen en Rick Smelt tijdens het PIB project van 2024/2025 waarin de hoofdwerking gerealiseerd is en verbeterd door Martijn Scheer tijdens het 2026 Elektronica project waarin de software verbeterd is en stappen voor het uitbreiding naar polyfoon zijn uitgewerkt.

## Werking

De module bestaat uit twee hoofdonderdelen: het ingebouwde keyboard en een schakelaar waarmee gekozen kan worden tussen de keyboard en een MIDI. Hierdoor kan de synthesizer zowel zelfstandig gebruikt worden als aangestuurd worden via MIDI. Op het moment van schrijven is deze werking helaas niet aanwezig. 
Hoewel de hardware voor de MIDI aanwezig is, was de softwarematige verwerking van MIDI signalen aan het einde van de projecten van 2024/2025/2026 nog werkend.
**Momenteel staat in de software de output vastgesteld op keyboard modus, ongeacht de stand van de schakelaar! Dit omdat de schakelaar niet goed bleek te werken, zie het tabje issues voor meer uitleg**

Het keyboard bestaat uit 25 toetsen die door een AVR128DB48-microcontroller worden uitgelezen. Wanneer een toets wordt ingedrukt, wordt de bijbehorende toonhoogte omgerekend naar een analoge spanning via de ingebouwde DAC van de microcontroller, zie de afbeelding hieronder. Daarnaast wordt een gate-signaal gegenereerd om aan te geven dat een toets actief is, de gate is een puls van 5v (op het moment van schrijven is er een bug? de gate lijkt nu 3.3v te zijn) en 10ms lang.

![alt](https://www.image2url.com/r2/default/images/1781092390880-23bcb144-f601-4c8c-bbcb-b64a81e20a21.png)

## In- en uitgangssignalen

De output van de input module bestaat uit een toonhoogtesignaal (CV) en een gate-signaal. Deze signalen worden doorgestuurd naar de andere modules van de synthesizer, zoals de VCO en ADSR, en vormen daarmee de basis voor het genereren van geluid.

# Module 5: Sequencer

De sequencer is een onderdeel van een modulaire synthesizer. Het systeem leest analoge toonhoogte-signalen van een keyboard in, slaat deze digitaal op en speelt ze later herhalend af als een sequence.

De sequencer ondersteunt meerdere modi:Recording, Play & Replace

Daarnaast kan de afspeelsnelheid (BPM) aangepast worden met een potentiometer en geven LED’s visuele feedback over de huidige positie of status.

### Werking
Input
De sequencer ontvangt twee signalen: Toonhoogte-signaal en Gate-signaal

Het toonhoogte-signaal bepaalt welke noot gespeeld wordt.
Het gate-signaal geeft aan wanneer een toets wordt ingedrukt.

#### ADC

Het inkomende analoge toon-signaal wordt ingelezen via de ADC van de microcontroller.
Omdat de ingangsspanning hoger kan zijn dan de maximale ADC-spanning, wordt eerst een spanningsdeler toegepast.

##### Opslag

De gemeten toonwaardes worden opgeslagen in een array in het geheugen van de microcontroller.

Elke stap in de sequence bevat een 10-bit waarde.

####DAC

Tijdens het afspelen worden de opgeslagen waardes via de DAC terug omgezet naar een analoog spanningssignaal.

Dit signaal wordt gebruikt om een VCO (Voltage Controlled Oscillator) aan te sturen.

#### Gate generatie

Tijdens playback genereert de sequencer zelf een nieuw gate-signaal.

Bij iedere nieuwe noot wordt de gate kort laag gemaakt en daarna weer hoog gezet, zodat externe modules weten dat een nieuwe toon gestart is.

### States
Recording: Leest ADC-waardes in en slaat deze op.
Play: Speelt opgeslagen waardes af via de DAC.
Replace: Speelt de huidige sequence af en overschrijft tegelijk bestaande noten.

Tijdens testen bleek dat de uitgangsspanning een afwijking had van ongeveer 21–30% ten opzichte van de ideale toonstappen.

Mogelijke oorzaken:

ADC/DAC onnauwkeurigheid
toleranties van weerstanden
offset in de analoge versterking
Mogelijke verbeteringen
Microcontroller met nauwkeurigere ADC/DAC
Weerstanden met lagere toleranties
Softwarematige offset-correctie
Hogere resolutie voor toonopslag


(!) Sequencer leek te werken, maar we weten niet hoe we hem instellen.

# Module 6: Power amplifier + Power supply

De "Pulse Soundworks" Power Amplifier is een klasse-AB eindversterker, door te kiezen voor een klasse-AB ontwerp wordt een goede balans bereikt tussen geluidskwaliteit, efficiëntie en warmteontwikkeling.

Naast de eindversterker is ook een voedingsmodule ontworpen. Deze zet de spanning van de trafo om naar meerdere stabiele voedingsspanningen die gebruikt worden door zowel de versterker als andere onderdelen van de synthesizer.

## Werking
<img width="250" height="88" alt="image" src="https://github.com/user-attachments/assets/9fb0eb1a-0d83-49e0-9357-373cd6c0b133" />
<img width="192" height="143" alt="image" src="https://github.com/user-attachments/assets/900bcbc5-ca81-4f53-9154-240b5c46db5e" />

Het audiosignaal komt eerst binnen via een hoogdoorlaatfilter dat DC ruis verwijdert. Daarna gaat het signaal door een buffer zodat de bron niet belast wordt. Met de volumepotmeter kan vervolgens het gewenste geluidsniveau worden ingesteld.

Na deze voorbewerking wordt het signaal versterkt door een opamp die als spanningsversterker werkt. Vervolgens sturen de drivertransistoren de MOSFET eindtrap aan. Deze eindtrap bestaat uit een IRF540N en een IRF9540N die samen het benodigde uitgangsvermogen leveren aan de luidspreker of hoofdtelefoon.

De voeding maakt gebruik van een transformator die 230 V AC omzet naar 12 VDC. Met behulp van gelijkrichters, afvlakcondensatoren en spanningsregelaars worden hieruit stabiele positieve en negatieve voedingsspanningen gemaakt. Deze spanningen worden gebruikt door de eindversterker en de overige elektronica van de synthesizer.

## In- en uitgangssignalen

### Ingangen

Audio input: 5 Vpp
Voeding: 18VAC vanuit trafo wordt gelijkgericht naar 12VDC
Volume regeling: 10 kΩ logaritmische potmeter

### Uitgangen

Speaker output: > 1 W aan 8 Ω
Headphone output: verzwakt audiosignaal voor oordopjes of hoofdtelefoon
Voedingsuitgangen: +12 V, -12 V, +5 V en +3,3 V

# Module 7: LFO (Low Frequency Oscillator)
De LFO is een laagfrequente oscillator voor een modulaire synthesizer. De module genereert variërende spanningen waarmee andere modules gemoduleerd kunnen worden. Hiermee kunnen verschillende effecten, zoals bijvoorbeeld vibrato, gemaakt worden.
## Werking
De LFO is volledig analoog opgebouwd met opamps. Met als basis een simpele oscillator. Hierin laadt en ontlaadt een condensator zichzelf steeds via een weerstand. De opamp schakelt daarbij steeds tussen de positieve en negatieve voedingsspanning, waardoor een oscillerend signaal ontstaat.

Met een potmeter wordt op- en aflaad de weerstand aangepast. Hierdoor verandert de snelheid waarmee de condensator oplaadt en ontlaadt. Daarmee wordt dus ook de frequentie van de LFO geregeld.

De schakeling genereert eerst een blokgolf en een driehoek.
De driehoekgolf wordt daarna via diodes afgerond zodat een sinusachtige golf ontstaat. Door de verschillende paden te kiezen kan je dan dus kiezen tussen een driehoek, blokgolf of sinus- vormig signaal

Na de golfselectie wordt het signaal verder aangepast met amplituderegeling (hoe sterk het signaal is) en offset (het signaal omhoog verschuiven)
De module heeft uiteindelijk drie gebufferde outputs zodat meerdere modules tegelijk aangesloten kunnen worden.
## In- en uitgangssignalen

### Ingangen
Voeding: ±12V
### Uitgangen
Signaal met driehoek, blokgolf of sinus- vorm
Regelbare amplitude: ongeveer 0V tot 2.2V
Offset: 0V tot +1.5V
Maximale uitgangsspanning: ongeveer 5.5V
Drie identieke gebufferde outputs

<img width="1099" height="295" alt="image" src="https://github.com/user-attachments/assets/6f277469-f272-40ef-aa36-1cc43a86ff55" />
	
# Module 8: VCF(Voltage Controlled Filter)
De VCF (Voltage Controlled Filter) is een spanningsgestuurde filtermodule voor een modulaire synthesizer. De module filtert bepaalde frequenties uit een audiosignaal en kan de kantelfrequentie aanpassen met een potmeter een externe control spanning. De module kan zowel als LPF als HDF worden ingesteld.
## Werking
De VCF werkt door bepaalde frequenties uit een audiosignaal weg te filteren. Het ingangssignaal gaat eerst door een diode ladder.(Het ingangssignaal wordt eerst verzwakt en voorzien van een offset zodat de diodeladder correct werkt binnen het juiste spanningsgebied.) In deze schakeling gedragen diodes zich als spanningsafhankelijke weerstanden. Door de spanning over de diodes te veranderen verandert ook de weerstand van het filter.

Hierdoor verschuift de kantelfrequentie van het filter. Bij een low-pass filter worden hoge frequenties verzwakt en blijven lage frequenties over. Bij een high-pass filter gebeurt juist het tegenovergestelde. Dit veranderd de klank heel erg uiteraard.

De kantelfrequentie kan aangepast worden met een potmeter of met een externe control spanning. Daardoor kan het filter automatisch bewegen tijdens het spelen, wat typische synthesizer-effecten zoals sweeps oplevert.

## **Notities Sequencer:** 
De Sequencer werkt niet, dit is nagevraagd aan de originele groep, die heeft vermeld dat deze het wel heeft gedaan maar met een afwijking van 20-30%.
Dit is echter niet wat we hebben kunnen meten. Door te meten op de Gate van de uitgang meet je tijdens het recorden en replacen wel direct de gate. maar niet het signaal (gewoon 0). Tijdens het signaal opslaan lukt het niet om het patroon terug te zien. Dus het lijkt ook niet alsof recorden werkt.

De lampjes werken wel en reageren wel op de bpm die je instelt. Dit is een interresante module om een project over te kunnen doen. Aangezien het nog een mysterie is waarom deze module het niet doet.

### **Algemene notities:**
Wanneer er geen toets wordt gespeeld is er ruis op de ingang van de VCO. Deze ruis lijkt ook al te worden geoscilleerd waardoor je de brom hoort.
Voor de VCO is het misschien een idee om een hoogdoorlaat filter van bijvoorbeeld >5hz te doen om de ruis eruit te filteren / al flink indammen.

### **Notities Filter** 
Het filter werkt exponentieel, echter werken de frequenties die uit de modules komen lineair. Hierdoor is het vrijwel onmogelijk om het filter af te stemmen om ruis of een mooi signaal te maken. Zo is bijvoorbeeld het uitfilteren van de ruis met een hoogdoorlaatfilter erg moeilijk omdat op het moment dat deze afgestemd is om de ruis te filteren, het toetsenbord ook al flink gedempt is. 

### **Notities Eindversterker:**
Eindversterker module lijkt alles aan te kloppen na testen. Kleine demping voor koptelefoon (dat hoort). 

### **Notities Input:**
De input module werkt per 2026 de software door gebruik van Arrays die de ingedrukte toetsen in volgorde bijhoud. Het programmeren van de PCB
die zich rechtsonder (onder de basisplaat) bevind is getest en werkt nog. Hiervoor kun je bij het Lab een UPDI programmeren vragen om de pins te programmeren. 
De gebruikte software is MPlab, voor de meest actuele softwarefiles kijk in het mapje "Software" in het mapje "Documentatie 2026", MAAR! let hier op dat deze software de meest actuele software is die op de PCB staat. Hiervoor is de MIDI functie compleet geschrapt.
Als de Midi dus werkende gemaakt moet worden, zit er in deze software helemaal gedachte daarin gestopt! De switch knop is ook in beide gevallen geprogrammeerd op keyboard modus (er is een fout in of hardwarematig of softwarematig de knop!)

Verder is er een onderzoek gedaan naar parafonie. Dit houd in dat er meerdere CV spanningen uit de input module komen, waardoor je bijvoorbeeld akkoorden kunt spelen. In het kort moet het volgende hiervoor nog gebeuren: 
1. Input module software moet worden aangepast om plekken in de array een aparte CV spanning en Output geven. 
1B: Er is maar 1 DAC 10 bit aanwezig op de AVR128db48. Voor Parafonie heb je meer uitgangen nodig. Dit kan met een PWM met een filter of een extern bordje die je bijvoorbeeld met I2C kan aansturen.
2. PCB input module moet opnieuw gemaakt worden om meer CV outputs te ondersteunen. 
3. VCO module moet meer ingangen ondersteunen om elke CV spanning zijn eigen oscillatie te geven.
4. Deze moeten weer gecombineerd worden in de Mixer module. Deze werkt momenteel niet omdat deze niet gebufferd wordt. Ook is deze module afhankelijk
van de werking van de VCA en de ADSR module, deze werken momenteel ook nog niet. DUS! Het is belangrijk om een de ADSR + Mixer + VCA module werkende te krijgen, voordat er wordt gekeken naar parafonie. Nouja, niks hoeft, maar is wel aangeraden.

Kijk het mapje "Vooronderzoek poly/parafonie" in het mapje "Documentatie 2026" voor meer informatie hierover. 

### **Notities VCO**
**(7-5-2026)** De VCO werkt momenteel alleen via output 2. Output 1 zit aan de onderzijde en is niet aangesloten op het breadboard. Het lijkt erop dat de huidige module onder tijdsdruk is ontworpen, waarbij de focus lag op één werkend kanaal.

Om beide kanalen werkend te krijgen, zijn de meest voor de hand liggende opties:

1. Een tweede vergelijkbaar bordje maken.
2. Een nieuwe PCB ontwerpen met ondersteuning voor beide kanalen.

**Raadpleeg de documentatie van de VCO 2024/2025 om te bepalen welke optie het meest praktisch is.**


### **Notities ADSR:**
ADSR zijn nog onbekende fouten. Hiddeh ff iets bijtypen

### **Notities Mixer:**
Actieve mixer van maken. Hiddeh ff iets bijtypen

### **Notities VCA:**
VCA lijkt geen goeie input te krijgen doordat er niet gebufferd wordt.

### **Notities Power amp + power supply:**
De voeding die binnenkomt op de 18VAC 50Hz, voedingkabels lijken per module te verschillen, als de module niet meer werkt, zekering checken, deze brand snel door (uit ervaring). Mosfet wordt ook wel warm, hierom is het aangeraden om wellicht een extra koellichaampje op deze mosfet te monteren.   
