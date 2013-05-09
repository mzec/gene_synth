#include <Tone.h>
#include <avr/pgmspace.h>

#define G  250
#define A  500
#define T  750
#define C  1000
#define X  0

Tone notePlayer[2];

int blueleft = 6;
int blueright = 7;
int greenleft = 4;
int greenright = 5;
int yellowleft = 2;
int yellowright = 3;
int redleft = 0;
int redright = 1;
int allLeds[] = {10, 11, 16, 18, 17};
int selector;
int toneleft;
int toneright;
int pushbutton = 12;
long lastpush;

/* za potrebe testiranja:
int geneHomo[] = {G, C, A, G, G};
int genePan[] = {G, G, G, G, G};
int geneGadus[] = {C, C, C, C, C};
int geneApis[] = {A, A, A, A, A};
int geneSaccharomyces[] = {T, T, T, T, T};
int geneLactobacillus[] = {C, A, C, A, C};*/

// stringovi sa sekvencama, pospremljeni u read-only flash memoriju:
PROGMEM prog_uint16_t geneHomo[] = {G,C,A,G,A,C,C,C,G,A,G,A,G,C,A,T,G,C,C,C,T,T,C,T,G,G,C,T,T,A,C,A,C,A,C,T,G,G,G,T,G,T,G,A,A,A,C,A,A,C,T,A,A,T,T,G,T,C,G,G,T,G,T,T,A,A,C,A,A,A,A,T,G,G,A,T,T,C,C,A,C,T,G,A,G,C,C,A,C,C,C,T,A,C,A,G,C,C,A,G,A,A,G,A,G,A,T,A,T,G,A,G,G,A,A,A,T,T,G,T,T,A,A,G,G,A,A,G,T,C,A,G,C,A,C,T,T,A,C,A,T,T,A,A,G,A,A,A,A,T,T,G,G,C,T,A,C,A,A,C,C,C,C,G,A,C,A,C,A,G,T,A,G,C,A,T,T,T,G,T,G,C,C,A,A,T,T,T,C,T,G,G,T,T,G,G,A,A,T,G,G,T,G,A,C,A,A,C,A,T,G,C,T,G,G,A,G};
PROGMEM prog_uint16_t genePan[] = {G,C,A,G,A,C,C,C,G,A,G,A,G,C,A,T,G,C,C,C,T,T,C,T,G,G,C,T,T,A,C,A,C,A,C,T,G,G,G,T,G,T,G,A,A,A,C,A,A,C,T,A,A,T,T,G,T,C,G,G,T,G,T,T,A,A,C,A,A,A,A,T,G,G,A,T,T,C,C,A,C,T,G,A,G,C,C,A,C,C,C,T,A,C,A,G,C,C,A,G,A,A,G,A,G,A,T,A,T,G,A,G,G,A,A,A,T,T,G,T,T,A,A,G,G,A,A,G,T,C,A,G,C,A,C,T,C,A,C,A,T,T,A,A,G,A,A,A,A,T,T,G,G,C,T,A,C,A,A,C,C,C,C,G,A,C,A,C,A,G,T,A,G,C,A,T,T,T,G,T,G,C,C,A,A,T,T,T,C,T,G,G,T,T,G,G,A,A,T,G,G,T,G,A,C,A,A,C,A,T,G,C,T,G,G,A,G};
PROGMEM prog_uint16_t geneGadus[] = {C,C,A,G,A,C,C,C,G,G,G,A,G,C,A,C,G,C,C,C,T,C,C,T,G,G,C,C,T,T,C,A,C,C,C,T,G,G,G,A,G,T,G,A,A,G,C,A,G,C,T,C,A,T,C,G,T,T,G,G,C,A,T,C,A,A,C,A,A,G,A,T,G,G,A,C,A,A,C,A,C,C,G,A,G,C,C,C,C,C,C,T,A,C,A,G,C,C,A,G,T,C,C,C,G,T,T,A,C,G,A,G,G,A,G,A,T,C,A,C,C,A,A,G,G,A,G,G,T,C,A,G,C,C,A,G,T,A,C,A,T,C,A,A,G,A,A,G,A,T,C,G,G,C,T,A,C,A,A,C,C,C,C,G,C,C,G,C,T,G,T,G,C,C,C,T,T,C,G,T,G,C,C,C,A,T,C,T,C,C,G,G,A,T,G,G,C,A,C,G,G,G,G,A,C,A,A,C,A,T,G,C,T,G,G,A,G};
PROGMEM prog_uint16_t geneApis[] = {A,C,A,A,A,C,T,C,G,T,G,A,G,C,A,T,G,C,T,T,T,G,C,T,C,G,C,T,T,T,T,A,C,T,C,T,T,G,G,T,G,T,G,A,A,A,C,A,A,T,T,G,A,T,T,G,T,T,G,G,T,G,T,T,A,A,T,A,A,G,A,T,G,G,A,C,T,C,C,A,C,T,G,A,A,C,C,A,C,C,G,T,A,T,T,C,T,G,A,A,A,C,C,C,G,A,T,T,T,G,A,A,G,A,A,A,T,T,A,A,A,A,A,A,G,A,A,G,T,G,T,C,A,T,C,T,T,A,C,A,T,T,A,A,A,A,A,A,A,T,T,G,G,T,T,A,T,A,A,T,C,C,A,G,C,T,G,C,A,G,T,T,G,C,A,T,T,T,G,T,G,C,C,A,A,T,T,T,C,T,G,G,T,T,G,G,C,A,T,G,G,A,G,A,T,A,A,T,A,T,G,T,T,G,G,A,A};
PROGMEM prog_uint16_t geneSaccharomyces[] = {T,C,A,A,A,C,C,A,G,A,G,A,A,C,A,C,G,C,T,T,T,G,T,T,G,G,C,T,T,T,C,A,C,C,T,T,G,G,G,T,G,T,T,A,G,A,C,A,A,T,T,G,A,T,T,G,T,T,G,C,T,G,T,C,A,A,C,A,A,G,A,T,G,G,A,C,T,C,C,G,T,C,A,A,A,T,G,G,G,A,C,X,X,X,X,X,X,G,A,A,T,C,C,A,G,A,T,T,C,C,A,A,G,A,A,A,T,T,G,T,C,A,A,G,G,A,A,A,C,C,T,C,C,A,A,C,T,T,T,A,T,C,A,A,G,A,A,G,G,T,T,G,G,T,T,A,C,A,A,C,C,C,A,A,A,G,A,C,T,G,T,T,C,C,A,T,T,C,G,T,C,C,C,A,A,T,C,T,C,T,G,G,T,T,G,G,A,A,C,G,G,T,G,A,C,A,A,C,A,T,G,A,T,T,G,A,A};
PROGMEM prog_uint16_t geneLactobacillus[] = {X,X,X,X,X,X,X,X,X,T,G,A,A,C,A,C,A,T,T,T,T,G,C,T,T,G,C,C,C,G,C,C,A,A,G,T,G,G,G,T,G,T,T,G,A,C,T,A,C,A,T,C,G,T,T,G,T,C,T,T,C,T,T,A,A,A,C,A,A,G,A,C,T,G,A,C,C,T,T,G,T,T,G,A,C,G,A,T,G,A,C,G,A,A,X,X,X,X,X,X,X,X,X,T,T,G,G,T,T,G,A,C,T,T,A,G,T,T,G,A,A,A,T,G,G,A,A,G,T,T,C,G,T,G,A,G,T,T,A,C,T,T,T,C,A,G,A,A,T,A,C,G,A,T,T,A,C,C,C,T,G,G,T,G,A,C,G,A,T,A,T,T,C,C,A,G,T,T,A,T,C,C,G,C,G,G,T,T,C,T,G,C,T,T,T,G,A,A,G,G,C,A,C,T,C,G,A,A,G,G,X,X,X,X,X,X,X};

void setup() {
    notePlayer[0].begin(9);
    notePlayer[1].begin(8);
  for (int i=0; i<=7; i++) {
    pinMode(i, OUTPUT);
  }
	pinMode(A5, INPUT);
	pinMode(pushbutton, INPUT);
	for (int i=0; i<=4; i++) pinMode(allLeds[i], OUTPUT);
	for (int i=0; i<=4; i++) digitalWrite(allLeds[i], LOW);
	digitalWrite(10, HIGH);
	selector = 1;
}

void loop() {
    for (int nukleotid = 0; nukleotid < 216; nukleotid++) {
	if (digitalRead(pushbutton) == HIGH) {
		lastpush = millis();
	} 
	selector = map(analogRead(A5), 0, 1010, 1, 5);
	toneleft = pgm_read_word_near(geneHomo + nukleotid);
	switch (selector) {
	case 1:
		//	toneright <- genewhatevs[nukleotid]
		toneright = pgm_read_word_near(genePan + nukleotid);
		for (int i=0; i<=4; i++) digitalWrite(allLeds[i], LOW);
		digitalWrite(10, HIGH);
		break;
	case 2:
		toneright = pgm_read_word_near(geneGadus + nukleotid);
		for (int i=0; i<=4; i++) digitalWrite(allLeds[i], LOW);
		digitalWrite(11, HIGH);
		break;
	case 3:
		toneright = pgm_read_word_near(geneApis + nukleotid);
		for (int i=0; i<=4; i++) digitalWrite(allLeds[i], LOW);
		digitalWrite(16, HIGH);
		break;
	case 4:
		toneright = pgm_read_word_near(geneSaccharomyces + nukleotid);
		for (int i=0; i<=4; i++) digitalWrite(allLeds[i], LOW);
		digitalWrite(17, HIGH);
		break;
	case 5:
		toneright = pgm_read_word_near(geneLactobacillus + nukleotid);
		for (int i=0; i<=4; i++) digitalWrite(allLeds[i], LOW);
		digitalWrite(18, HIGH);
		break;
	}   
	long elapsed = millis() - lastpush;
	if (elapsed < 20000) {
		notePlayer[0].play(toneleft, 130);
		notePlayer[1].play(toneright, 130);
	}
    if (toneleft == G) digitalWrite(blueleft, HIGH);
    else if (toneleft == A) digitalWrite(greenleft, HIGH);
    else if (toneleft == T) digitalWrite(yellowleft, HIGH);
    else if (toneleft == C) digitalWrite(redleft, HIGH);
    if (toneright == G) digitalWrite(blueright, HIGH);
    else if (toneright == A) digitalWrite(greenright, HIGH);
    else if (toneright == T) digitalWrite(yellowright, HIGH);
    else if (toneright == C) digitalWrite(redright, HIGH);
    delay(150);
    digitalWrite(blueleft, LOW);
    digitalWrite(greenleft, LOW);
    digitalWrite(yellowleft, LOW);
    digitalWrite(redleft, LOW);
    digitalWrite(blueright, LOW);
    digitalWrite(greenright, LOW);
    digitalWrite(yellowright, LOW);
    digitalWrite(redright, LOW);
    
  }
}
