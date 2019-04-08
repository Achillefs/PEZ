// Tez - A multi-target tempo dispenser
// clock (decimal 248, hex 0xF8)
// start (decimal 250, hex 0xFA)
// continue (decimal 251, hex 0xFB)
// stop (decimal 252, hex 0xFC)
// FATWAVES. Lots of code swiped from Erik Oostveen's Midi Clock
// https://www.erikoostveen.co.uk

/* Actual tempo is tempo value + 30 */
#define  TEMPO_MAX 270
#define  TEMPO_MIN 0
#define  TEMPO_D   1
#define  TEMPO_CENTER 90

int tempoMemAddress = 0;
int beatMemAddress = 2;
int dinRateMemAddress = 4;
int blinkEnableMemAddress = 6;

// Original beats in uS
//int long tempos[] = {83333,80645,78125,75758,73529,71429,69444,67568,65789,64103,62500,60976,59524,58140,56818,55556,54348,53191,52083,51020,
//50000,49020,48077,47170,46296,45455,44643,43860,43103,42373,41667,40984,40323,39683,39063,38462,37879,37313,36765,36232,35714,35211,34722,34247,33784,
//33333,32895,32468,32051,31646,31250,30864,30488,30120,29762,29412,29070,28736,28409,28090,27778,27473,27174,26882,26596,26316,26042,25773,25510,25253,
//25000,24752,24510,24272,24038,23810,23585,23364,23148,22936,22727,22523,22321,22124,21930,21739,21552,21368,21186,21008,20833,20661,20492,20325,20161,
//20000,19841,19685,19531,19380,19231,19084,18939,18797,18657,18519,18382,18248,18116,17986,17857,17730,17606,17483,17361,17241,17123,17007,16892,16779,
//16667,16556,16447,16340,16234,16129,16026,15924,15823,15723,15625,15528,15432,15337,15244,15152,15060,14970,14881,14793,14706,14620,14535,14451,14368,
//14286,14205,14124,14045,13966,13889,13812,13736,13661,13587,13514,13441,13369,13298,13228,13158,13089,13021,12953,12887,12821,12755,12690,12626,12563,
//12500,12438,12376,12315,12255,12195,12136,12077,12019,11962,11905,11848,11792,11737,11682,11628,11574,11521,11468,11416,11364,11312,11261,11211,11161,
//11111,11062,11013,10965,10917,10870,10823,10776,10730,10684,10638,10593,10549,10504,10460,10417,10373,10331,10288,10246,10204,10163,10121,10081,10040,
//10000,9960,9921,9881,9843,9804,9766,9728,9690,9653,9615,9579,9542,9506,9470,9434,9398,9363,9328,9294,9259,9225,9191,9158,9124,9091,9058,9025,8993,8961,
//8929,8897,8865,8834,8803,8772,8741,8711,8681,8651,8621,8591,8562,8532,8503,8475,8446,8418,8389,8361,8333};

// Corrected beats in uS (to compensate for uno performance)
int long tempos[] = {
  82151, 79463, 76943, 74576, 72347, 70247, 68262, 66386, 64607, 62921, 61318, 59794, 58342, 56958, 55636, 54374, 53166, 52009, 50901, 49838, 48818, 47838,
  46895, 45988, 45114, 44273, 43461, 42678, 41921, 41191, 40485, 39802, 39141, 38501, 37881, 37280, 36697, 36131, 35583, 35050, 34532, 34029, 33540, 33065, 32602, 32151, 31713,
  31286, 30869, 30464, 30068, 29682, 29306, 28938, 28580, 28230, 27888, 27554, 27227, 26908, 26596, 26291, 25992, 25700, 25414, 25134, 24860, 24591, 24328, 24071, 23818, 23570,
  23328, 23090, 22856, 22628, 22403, 22182, 21966, 21754, 21545, 21341, 21139, 20942, 20748, 20557, 20370, 20186, 20004, 19826, 19651, 19479, 19310, 19143, 18979, 18818, 18659,
  18503, 18349, 18198, 18049, 17902, 17757, 17615, 17475, 17337, 17200, 17066, 16934, 16804, 16675, 16548, 16424, 16301, 16179, 16059, 15941, 15825, 15710, 15597, 15485, 15374,
  15265, 15158, 15052, 14947, 14844, 14742, 14641, 14541, 14443, 14346, 14250, 14155, 14062, 13970, 13878, 13788, 13699, 13611, 13524, 13438, 13353, 13269, 13186, 13104, 13023,
  12942, 12863, 12784, 12707, 12630, 12554, 12479, 12405, 12332, 12259, 12187, 12116, 12046, 11976, 11907, 11839, 11771, 11705, 11639, 11573, 11508, 11444, 11381, 11318, 11256,
  11194, 11133, 11073, 11013, 10954, 10895, 10837, 10780, 10723, 10666, 10610, 10555, 10500, 10446, 10392, 10339, 10286, 10234, 10182, 10130, 10079, 10029, 9979, 9929, 9880, 9831,
  9783, 9735, 9688, 9641, 9594, 9548, 9502, 9456, 9411, 9367, 9322, 9278, 9235, 9191, 9149, 9106, 9064, 9022, 8981, 8939, 8899, 8858, 8818, 8778, 8739, 8699, 8661, 8622, 8584, 8546,
  8508, 8471, 8433, 8397, 8360, 8324, 8288, 8252, 8216, 8181, 8146, 8112, 8077, 8043, 8009, 7976, 7942, 7909, 7876, 7843, 7811, 7779, 7747, 7715, 7683, 7652, 7621, 7590, 7559, 7529,
  7499, 7469, 7439, 7409, 7380, 7350, 7321, 7293, 7264, 7236, 7207, 7179, 7151
};

int ledRed = 8;
int ledGreen = 9;
int ledBlue = 10;

int btnMinusPin = 4; // - button
int btnMainPin = 5; // Start / Stop switch
int btnPlusPin = 6; // + button

// Analog sync out
int ppqOut = A0; // ppq pin
int ppqRate = 12; // CR-78
int ppqCounter = 0; // use this to calculate rate

// - button State
boolean currentMinusState = HIGH;
boolean lastMinusState = HIGH;

// + button State
boolean currentPlusState = HIGH;
boolean lastPlusState = HIGH;

// START / STOP State
boolean currentState = HIGH;
boolean lastState = HIGH;
int long pressCount = 1;

long unsigned clockTracker = 0; // Tracks to send MIDI clock
long unsigned clockTrackerMem = 0; // 'Remembers' when MIDI clock was sent
int flag = 0;

int long blinkTracker = 0;
int long blinkTrackerMem = 0;
int blinkFlag = 0;

boolean playing = LOW;

int oldTempo = -1;
int currentTempo = TEMPO_CENTER;
long unsigned tempoMicro = tempos[currentTempo];

int beatCount = 4;
int currentBeat = 0;

#include <LiquidCrystal.h>
#include <EEPROM.h>

const int rs = 12, en = 11, d4 = 13, d5 = 7, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
  Serial.begin(31250);
  //Serial.begin(38400);

  // load last used tempo if it exists
  if (readInt(tempoMemAddress) > TEMPO_MIN) {
    currentTempo = readInt(tempoMemAddress);
    tempoMicro = tempos[currentTempo];
  }

  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("=======TEZ======");
  lcd.setCursor(1, 1);
  lcd.print("Tempo Dispenser");


  pinMode(btnMinusPin, INPUT);
  pinMode(btnMainPin, INPUT);
  pinMode(btnPlusPin, INPUT);
  pinMode(ppqOut, OUTPUT);

  digitalWrite(btnMinusPin, HIGH); // pull up yo
  digitalWrite(btnMainPin, HIGH); // pull up yo
  digitalWrite(btnPlusPin, HIGH); // pull up yo
  digitalWrite(ppqOut, LOW);

  digitalWrite(ledRed, LOW);
  digitalWrite(ledGreen, LOW);
  digitalWrite(ledBlue, LOW);

  digitalWrite(ledRed, HIGH);
  digitalWrite(ledBlue, HIGH);
  delay(2000);
  digitalWrite(ledRed, LOW);
  digitalWrite(ledBlue, LOW);
  lcd.clear();
  displayPlaybackState();
}

void loop() {
  setTempo();
  writeTempo();
  updateTempoLed();
  updateState();
}

/* writes tempo to MIDI OUT */
void writeTempo() {
  clockTracker = micros();

  if (flag == 0) {
    Serial.write(248); // MIDI Clock (0)
    // hardcoded 12 ppq for now, we'll figure it out later yo
    if ( ppqCounter == 1 ) {
      digitalWrite(ppqOut, HIGH);
      delay(5);
      digitalWrite(ppqOut, LOW);
    }
    ppqCounter++;
    flag = 1;
    if ( ppqCounter == 2 ) { ppqCounter = 0; }
  }

  if (clockTracker > (clockTrackerMem + tempoMicro)) {
    flag = 0;
    clockTrackerMem = clockTracker;
  }
}

/* Handles tempo led blinking */
void updateTempoLed() {
  blinkTracker = micros();
  if (blinkFlag == 0) {
    if (playing) {
      if (currentBeat == 1) {
        digitalWrite(ledGreen, HIGH);
      } else {
        digitalWrite(ledRed, HIGH);
        digitalWrite(ledBlue, HIGH);
      }

      if (currentBeat < beatCount) {
        currentBeat += 1;
      } else {
        currentBeat = 1;
      }
    } else {
      digitalWrite(ledBlue, HIGH);
    }

    blinkFlag = 1;
  }

  if (blinkTracker > (blinkTrackerMem + 12 * tempoMicro)) {
    digitalWrite(ledBlue, LOW);
    digitalWrite(ledRed, LOW);
    digitalWrite(ledGreen, LOW);
  }

  if (blinkTracker > (blinkTrackerMem + 25 * tempoMicro)) {
    blinkFlag = 0;
    blinkTrackerMem = blinkTracker;
  }
}

/* Reads tempo controls and sets tempo */
void setTempo() {
  lastMinusState = currentMinusState;
  lastPlusState = currentPlusState;

  currentMinusState = digitalRead(btnMinusPin);
  currentPlusState = digitalRead(btnPlusPin);

  // PLUS BUTTON
  if (currentPlusState == LOW && lastPlusState == HIGH) {
    lastPlusState = currentPlusState;
    if (currentTempo < TEMPO_MAX) {
      oldTempo = currentTempo;
      currentTempo += 1;
    }
  }

  // MINUS BUTTON
  if (currentMinusState == LOW && lastMinusState == HIGH) {
    lastMinusState = currentMinusState;
    if (currentTempo > TEMPO_MIN) {
      oldTempo = currentTempo;
      currentTempo -= 1;
    }
  }
}

/* Reads playback controls and updates state */
void updateState() {
  currentState = digitalRead(btnMainPin);
  if (currentState == LOW && lastState == HIGH) {
    if ( (pressCount % 2) != 0) {
      Serial.write(250); // MIDI Start (0)
      lastState = currentState;
      playing = HIGH;
      currentBeat = 1;
      displayPlaybackState();
      delay(5);
    }

    if ( (pressCount % 2) == 0) {
      Serial.write(252); // MIDI Stop (0)
      playing = LOW;
      pressCount = 0;
      currentBeat = 0;
      displayPlaybackState();
      delay(5);
    }
    pressCount++;
  }

  lastState = currentState;

  if (oldTempo != currentTempo) {
    oldTempo = currentTempo;
    tempoMicro = tempos[oldTempo];
    persistInt(tempoMemAddress, currentTempo);
    displayTempoState();
  }
}

/* print tempo state to LCD */
void displayTempoState() {
  String bpm = " BPM";
  String beat = "/4";
  lcd.home();
  lcd.print(currentTempo + 30 + bpm + " ");
  lcd.setCursor(16 - 3, 0);
  lcd.print(beatCount + beat);
}

/* print playback state to LCD */
void displayPlaybackState() {
  String playbackState = "STOP";
  if (playing) {
    playbackState = "PLAY";
  }
  lcd.setCursor(0, 1);
  lcd.print(playbackState);
}

void persistInt(int address, int value)
{
  byte two = (value & 0xFF);
  byte one = ((value >> 8) & 0xFF);

  EEPROM.update(address, two);
  EEPROM.update(address + 1, one);
}

int readInt(int address)
{
  long two = EEPROM.read(address);
  long one = EEPROM.read(address + 1);

  return ((two << 0) & 0xFFFFFF) + ((one << 8) & 0xFFFFFFFF);
}
