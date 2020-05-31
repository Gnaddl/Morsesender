/**
 *  Morsesender - Arduino demo for sending morse code
 *
 *  This Arduino sketch receives characters over the USB interface from a
 *  terminal program and converts them into Morse code.
 *
 *  See the Wikipedia articles for more information about Morse code:
 *  English: https://en.wikipedia.org/wiki/Morse_code
 *  German:  https://de.wikipedia.org/wiki/Morsezeichen
 *
 *  Author:      Gnaddl
 *  Date:        31-MAY-2020
 *  Last change: 31-MAY-2020
 */


const char *version = "Morse Sender version 0.0";


/**
 *  Table for converting ASCII code to Morse code. Only the characters
 *  from 30h to 5Fh are included.
 */
const uint8_t morsetab[] =
{
  0b00111111,   // '0'   -----
  0b00111110,   // '1'   .----
  0b00111100,   // '2'   ..---
  0b00111000,   // '3'   ...--
  0b00110000,   // '4'   ....-
  0b00100000,   // '5'   .....
  0b00100001,   // '6'   -....
  0b00100011,   // '7'   --...
  0b00100111,   // '8'   ---..
  0b00101111,   // '9'   ----.
  0b01000111,   // ':'   ---...
  0b01010101,   // ';'   -.-.-.
  0b00000000,   // '<'
  0b00110001,   // '='   -...-
  0b00000000,   // '>'
  0b01001100,   // '?'   ..--..
  0b01010110,   // '@'   .--.-.
  0b00000110,   // 'A'   .-
  0b00010001,   // 'B'   -...
  0b00010101,   // 'C'   -.-.
  0b00001001,   // 'D'   -..
  0b00000010,   // 'E'   .
  0b00010100,   // 'F'   ..-.
  0b00001011,   // 'G'   --.
  0b00010000,   // 'H'   ....
  0b00000100,   // 'I'   ..
  0b00011110,   // 'J'   .---
  0b00001101,   // 'K'   -.-
  0b00010010,   // 'L'   .-..
  0b00000111,   // 'M'   --
  0b00000101,   // 'N'   -.
  0b00001111,   // 'O'   ...
  0b00010110,   // 'P'   .--.
  0b00011011,   // 'Q'   --.-
  0b00001010,   // 'R'   .-.
  0b00001000,   // 'S'   ...
  0b00000011,   // 'T'   -
  0b00001100,   // 'U'   ..-
  0b00011000,   // 'V'   ...-
  0b00001110,   // 'W'   .--
  0b00011001,   // 'X'   -..-
  0b00011101,   // 'Y'   -.--
  0b00010011,   // 'Z'   --..
};


/**
 *  Table of predefined text strings
 */
const char *fixedtext[] =
{
  "CQ CQ CQ DE DL0XYZ DL0XYZ DL0XYZ PSE K",
  "Paris Paris Paris Paris Paris"
};

const int num_fixedtext = sizeof(fixedtext) / sizeof(fixedtext[0]);


/**
 *  Konfiguartion of the output pins for LED and Buzzer
 */
const int ledPin = LED_BUILTIN;
const int buzzerPin = 9;

int dittime = 60;    // 120 = 10 WpM
int index = 0;
char buffer[80];


/**
 *  Set a parameter
 *
 *  Currently selects one of the predefined fixed text strings. More commands
 *  will be added later, e.g. to change the speed.
 */
void setParameter(const char *txt)
{
  Serial.print("Set parameter: ");
  Serial.println(txt + 1);

  if (txt[0] == '!')
  {
    int index = txt[1] - '1';
    if ((index >= 0) && (index < num_fixedtext))
    {
      Serial.println(fixedtext[index]);
      sendMorse(fixedtext[index]);
    }
    else
    {
      Serial.println("Error: Index out of range");
    }
  }
}


/**
 *  sendMorse - sends a character string in Morse code
 */
void sendMorse(const char *txt)
{
  int c;

  while ((c = *txt++) != '\0')
  {
    c = toupper(c);

    if (c == ' ')
    {
      // Pause between two words: 7 dits; 3 already done
      Serial.print("  ");
      delay(4 * dittime);
      continue;
    }

    if ((c < '0') || (c > 'Z'))
      continue;

    uint8_t morsecode = morsetab[c - '0'];
    while (morsecode > 0x01)
    {
      digitalWrite(ledPin, HIGH);
      tone(buzzerPin, 1200);

      if (morsecode & 0x01)
      {
        Serial.print('-');
        delay(3 * dittime);
      }
      else
      {
        Serial.print('.');
        delay(dittime);
      }

      digitalWrite(ledPin, LOW);
      noTone(buzzerPin);
      delay(dittime);

      morsecode >>= 1;
    }

    // Pause between two symbols: 3 dits, 1 already done
    Serial.print(' ');
    delay(2 * dittime);
  }

  Serial.println();
}


void setup()
{
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  pinMode(buzzerPin, OUTPUT);
  digitalWrite(buzzerPin, LOW);

  Serial.begin(115200);
  Serial.println(version);
}


void loop()
{
  int c;

  if (Serial.available())
  {
    c = Serial.read();
    Serial.write(c);

    if (c != '\n')
    {
      buffer[index++] = c;
    }
    else
    {
      buffer[index] = '\0';
      index = 0;

      if ((buffer[0] == ':') || (buffer[0] == '!'))
        setParameter(buffer);
      else
        sendMorse(buffer);
    }
  }
}
