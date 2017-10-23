// teensy 3.6
// Sparkfun RGB LED strips (WS1812)
//
#include <Adafruit_NeoPixel.h>

#ifdef __AVR__
#include <avr/power.h>
#endif

#define LIGHT0_PIN (2)
#define LIGHT1_PIN (3)

#define LED_PIN (13)

#define SW0_PIN (24)
#define SW1_PIN (25)
#define SW2_PIN (26)

#define PIXEL_CNT (60)

#define LED_TOGGLE_DELAY (100)
#define STARTUP_DELAY (500)
#define LOOP_DELAY (500)
#define LED_PIXEL_DELAY (20)

Adafruit_NeoPixel light0 = Adafruit_NeoPixel(PIXEL_CNT, LIGHT0_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel light1 = Adafruit_NeoPixel(PIXEL_CNT, LIGHT1_PIN, NEO_GRB + NEO_KHZ800);

static bool last_red = false;
static bool last_green = false;
static bool last_blue = false;

static bool check_switches(void)
{
    bool changed = false;
    const bool r = !digitalRead(SW0_PIN);
    const bool g = !digitalRead(SW1_PIN);
    const bool b = !digitalRead(SW2_PIN);

    if((r != last_red) || (g != last_green) || (b != last_blue))
    {
        changed = true;

        digitalWrite(LED_PIN, LOW);
        delay(LED_TOGGLE_DELAY);
        digitalWrite(LED_PIN, HIGH);
    }

    last_red = r;
    last_green = g;
    last_blue = b;

    return changed;
}

static void update_lights(void)
{
    const uint8_t r = (last_red == true) ? 255 : 0;
    const uint8_t g = (last_green == true) ? 255 : 0;
    const uint8_t b = (last_blue == true) ? 255 : 0;
    
    uint16_t i;
    for(i = 0; i < PIXEL_CNT; i += 1)
    {
        light0.setPixelColor(i, r, g, b);
        light1.setPixelColor(i, r, g, b);
        
        light0.show();
        light1.show();

        delay(LED_PIXEL_DELAY);
    }
}

void setup()
{
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, HIGH);

    pinMode(SW0_PIN, INPUT_PULLUP);
    pinMode(SW1_PIN, INPUT_PULLUP);
    pinMode(SW2_PIN, INPUT_PULLUP);
       
    light0.begin();
    light1.begin();

    light0.clear();
    light1.clear();

    light0.setBrightness(255);
    light1.setBrightness(255);
    
    light0.show();
    light1.show();

    last_red = true;
    last_green = true;
    last_blue = true;

    delay(STARTUP_DELAY);
    update_lights();
    delay(STARTUP_DELAY);
}

void loop()
{
    const bool switches_changed = check_switches();

    if(switches_changed == true)
    {
        update_lights();
    }

    delay(LOOP_DELAY);
}
